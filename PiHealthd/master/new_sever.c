#include"master.h"
#include"common/common.h"


LinkedList client_link[20];
int INS;
int master_port;
int msg_port;
int client_port;
int udp_port;
int data_port;
char *config = "./pihealthd.conf";
char *pilog = "./pi.log";

void *MONITOR(void *client_link){
    LinkedList now_link = *(LinkedList *)client_link;
    while(1){
        usleep(2000000);
        Node *p = (Node *)malloc(sizeof(Node));
        p = now_link;
        int cnt = counter(now_link);
        //pthread_rwlock_rdlock(&rwlock);
        while(p->next != NULL){
            char nowip[20];
            strcpy(nowip, inet_ntoa(p->next->cl_addr.sin_addr));
            int msgfd;
            if((msgfd = socket_connect(nowip, msg_port)) > 0){
                char dirname[50] = {0};
                strcpy(dirname, "./ip/");
                strcat(dirname, nowip);
                mkdir(dirname, 0777);
                char signal[10] = {0};
                for(int i = 100; i < 106; i++){
                    int tmp = i;
                    memset(signal, 0, sizeof(signal));
                    sprintf(signal, "%d", tmp);
                    if(send(msgfd, signal, strlen(signal), 0) < 0){
                        //printf("send signal FAILED\n");
                    }
                    char logname[10] = {0};
                    switch(i){
                        case 100:
                            strcpy(logname, "cpu");
                            break;
                        case 101:
                            strcpy(logname, "mem");
                            break;
                        case 102:
                            strcpy(logname, "disk");
                            break;
                        case 103:
                            strcpy(logname, "pro");
                            break;
                        case 104:
                            strcpy(logname, "sys");
                            break;
                        case 105:
                            strcpy(logname, "user");
                            break;
                    }
                    char logad[50] = {0};
                    sprintf(logad, "%s/", dirname);
                    strcat(logad, logname);
                    strcat(logad, ".log");
                    FILE *file;
                    file = fopen(logad, "a+");
                   
                    char buf[10240] = {0};
                    //printf("recving file\n");
                    while(recv(msgfd, buf, sizeof(buf), 0) > 0){
                        if(strlen(buf) < 10)continue;
                        if(strcmp(buf, "end") == 0)break;
                        write_Pi_log(logad, "%s", buf);
                        write_Pi_log(pilog, "Recv %d Files From %s", strlen(buf), nowip);
                        memset(buf, 0, sizeof(buf));
                    }
                    fclose(file);
                    usleep(500000);
                }
                close(msgfd);
            }
            p = p->next;
        }
    }
}


void *HEART(){
    int n = 0;
    while(1){
        n++;
        pthread_rwlock_wrlock(&rwlock);
        fd_set rd;
        FD_ZERO(&rd);
        int maxfd = 0;
        struct timeval tm;
        tm.tv_sec = 0;
        tm.tv_usec = 300000;
        for(int i = 0; i < INS; i++){
            Node *p = client_link[i];
            while(p->next != NULL){
                struct sockaddr_in addr;
                addr.sin_family = AF_INET;
                addr.sin_addr.s_addr = p->next->cl_addr.sin_addr.s_addr;
                addr.sin_port = htons(client_port);
                int len = sizeof(addr);
                //printf("ip: %s port: %d\n", inet_ntoa(p->next->cl_addr.sin_addr), client_port); 
                int clientfd;
                if((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
                    write_Pi_log(pilog, "Socket Create Failed");
                }
        
                unsigned long ul = 1;
                ioctl(clientfd, FIONBIO, &ul);
                connect(clientfd, (struct sockaddr *)&addr, sizeof(addr));
                //printf("%s->%d\n", inet_ntoa(addr.sin_addr), clientfd);
                p->next->fd = clientfd;
                FD_SET(clientfd, &rd);
                if(maxfd < clientfd){
                    maxfd = clientfd;
                }
                p = p->next;
            }
        }
        
        usleep(1000000);
        
        int selectid;
        if((selectid = (select(maxfd + 100, NULL, &rd, NULL, &tm))) < 0){
            write_Pi_log(pilog, "Select: %s", strerror(errno));
        }else{
            for(int i = 0; i < INS; i++){
                Node *q = client_link[i];
                while(q->next != NULL){
                    char nowip[20] = {0};
                    strcpy(nowip, inet_ntoa(q->next->cl_addr.sin_addr));
                    if(FD_ISSET(q->next->fd, &rd) == 0){
                        close(q->next->fd);
                        printf("FD_ISSET == 0\n");
                        delete(q, nowip);
                        //q = client_link[i];
                    }else{
                        int erro = -1;
                        int len = sizeof(erro);
                        getsockopt(q->next->fd, SOL_SOCKET, SO_ERROR, &erro, (socklen_t *)&len);
                        if(erro != 0){
                            close(q->next->fd);
                            printf("HEART FAILED ERRO = %d\n", erro);
                            delete(q, nowip);
                            //q = client_link[i];
                        }else{
                            close(q->next->fd);
                            q = q->next;
                        }
                    }
                }
            }
        }
        pthread_rwlock_unlock(&rwlock);
        int cnt = 0;
        for(int i = 0; i < INS; i++){
            cnt += counter(client_link[i]);
        }
        printf("\033[33m%d\033[0m times select->cnt: %d\n", n, cnt);
        for(int i = 0; i < INS; i++){
            output(client_link[i]);
        }  
        usleep(20000);
    }
}

void *database(){
    int datafd = socket_create(udp_port);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int len = sizeof(addr);
    while(1){
        int accfd = accept(datafd, (struct sockaddr *)&addr, &len);
        char data[256] = {0};
        if(recv(accfd, data, sizeof(data), 0) > 0){
            char ip[20] = {0};
            char nowtime[30] = {0};
            int types;
            char ty[5] = {0};
            char details[128] = {0};
            char level[10] = {0};
            time_t logtime = time(NULL);
            struct tm *tmp = localtime(&logtime);
            sprintf(nowtime, "%d.%02d.%02d %02d:%02d:%02d ", (1900 + tmp->tm_year), (1 + tmp->tm_mon), \
    tmp->tm_mday, tmp->tm_hour, tmp->tm_min, tmp->tm_sec);
            strcpy(ip, inet_ntoa(addr.sin_addr));
            strncat(ty, data, 3);
            types = atoi(ty);
            int len = strlen(data);
            char temp[10] = {0};
            strncat(temp, &data[len - 5], 4);
            if(strcmp(temp, "note") == 0){
                strcpy(level, temp);
            }else{
                strcpy(level, "warning");
            }
            strncat(details, &data[3], len - 3);
            add_to_database(nowtime, ip, types, details, level);
        }
    }
}

void server(){
    int listenfd = init_epoll(master_port);  //建立鏈接
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);

    
    char start_ip[20], end_ip[20], thread[5];
    get_conf_value(config, "thread", thread);
    get_conf_value(config, "start_ip", start_ip);
    get_conf_value(config, "end_ip", end_ip);
    INS = atoi(thread);
    printf("end : %s start : %s\n", start_ip, end_ip);
    
    pthread_t pthread[INS];
    for(int i = 0; i < INS; i++){
        client_link[i] = init_link(client_link[i]);
        pthread_create(&pthread[i], NULL, MONITOR, (void*)&client_link[i]);
    }
        
    struct in_addr addrfrom;
    struct in_addr addrto;
    inet_aton(start_ip, &addrfrom);
    inet_aton(end_ip, &addrto);
    unsigned int nfrom = ntohl(addrfrom.s_addr);
    unsigned int nto = ntohl(addrto.s_addr);
    for(unsigned i = nfrom; i <= nto; i++){
        Node *node = (Node *)malloc(sizeof(Node));
        node->heart_fd = -1;
        node->next = NULL;
        node->cl_addr.sin_addr.s_addr = htonl(i);
        insert(client_link[get_min(client_link, INS)], node);
    }

    pthread_t pheart;
    pthread_create(&pheart, NULL, HEART, NULL);
   
    pthread_t pdata;
    pthread_create(&pdata, NULL, database, NULL);
    while(1){
        int n, i;
        int max = 1000;
        n = epoll_wait(event_fd, events, max, -1);
        for(int i = 0; i < n; i++){
            if((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN))){
                printf("epoll erro\n");
                close(events[i].data.fd);
                continue;
            }else if(listenfd == events[i].data.fd){
                int acceptfd;
                if((acceptfd = accept(listenfd, (struct sockaddr *)&client_addr, &len)) < 0){
                    //write_Pi_log(pilog, "Accept: %s", strerror(errno));
                    //perror("accept");
                    break;
                }
                char logbuf[128] = {0};
                event.events = EPOLLIN;
                if(event.events & EPOLLIN){
                    recv(acceptfd, logbuf, sizeof(logbuf), 0);
                    printf("--------->%s\n", logbuf);
                    memset(logbuf, 0, sizeof(logbuf));
                    //usleep(2000);
                    event.events = EPOLLOUT;   
                }
                if(event.events & EPOLLOUT){
                    strcpy(logbuf, "OK");
                    send(acceptfd, logbuf, strlen(logbuf), 0);
                }
            }
        }

        Node *p = (Node *)malloc(sizeof(Node));
        p->cl_addr = client_addr;
        p->next = NULL;
        
        int mark = 0;
        for(int i = 0; i < INS; i++){
            if(check(client_link[i], client_addr.sin_addr.s_addr) == 1){
                mark = 1;
            }
        }

        if(mark != 1){
            printf("%s insert to client_link[%d]\n", inet_ntoa(client_addr.sin_addr), get_min(client_link, INS));
            //pthread_rwlock_wrlock(&rwlock);
            insert(client_link[get_min(client_link, INS)], p);
            //pthread_rwlock_unlock(&rwlock);
            printf("%s login\n", inet_ntoa(client_addr.sin_addr));
        }
    }
}



int main(){
    char mport[5] = {0};
    char msport[5] = {0};
    char cport[6] = {0};
    char uport[5] = {0};
    char dport[5] = {0};
    get_conf_value(config, "master_port", mport);
    get_conf_value(config, "msg_port", msport);
    get_conf_value(config, "client_port", cport);
    get_conf_value(config, "UDP_port", uport);
    master_port = atoi(mport);
    msg_port = atoi(msport);
    client_port = atoi(cport);
    udp_port = atoi(uport);
    data_port = atoi(dport);
    write_Pi_log(pilog, "Server Start");
    server();
    return 0;
}
