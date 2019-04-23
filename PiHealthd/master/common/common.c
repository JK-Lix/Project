/*************************************************************************
	> File Name: common.c
	> Author: 
	> Mail: 
	> Created Time: 2019年03月05日 星期二 18时47分01秒
 ************************************************************************/

#include "common.h"


//tcp socket_create
int socket_create(int port){
        int serve_lesten;
    if((serve_lesten = socket(AF_INET, SOCK_STREAM, 0)) < 0){
                perror("creat_socket");
                exit(-1);
            
    }

        struct sockaddr_in servce_addr;
        servce_addr.sin_family = AF_INET;
        servce_addr.sin_port = htons(port);
        servce_addr.sin_addr.s_addr = htonl(INADDR_ANY);

        int yes = 1;
    if(setsockopt(serve_lesten, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
                close(serve_lesten);
                perror("setsockopt");
                return 1;
            
    }
    if(bind(serve_lesten, (struct sockaddr *)&servce_addr, sizeof(servce_addr)) < 0){
                perror("bind");
                close(serve_lesten);
                exit(-1);
            
    }

    if(listen(serve_lesten, 100) < 0){
                perror("listen");
                exit(-1);
            
    }
    //printf("tcp bind success\n");
        return serve_lesten;

}


//tcp socket_connect
int socket_connect(char *ip, int port){
    struct sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr(ip);
    client_addr.sin_port = htons(port);

    int client_socket;
    if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
                //perror("socket");
                return -1;
            
    }

    if(connect(client_socket, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0){
                //perror("connect");
                return -1;
            
    }

        return client_socket;

}


//ioctl connect
int check_connect(char *ip, int port, int timeout){
    struct sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr(ip);
    client_addr.sin_port = htons(port);
        
    int erro = -1, len;
    len = sizeof(int);

    int client_socket;
    if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket");
        return 0;
            
    }
    struct timeval tm;
    fd_set rd;
    unsigned long ul = 1;
    ioctl(client_socket, FIONBIO, &ul);
        
    int ret = 0;
    if(connect(client_socket, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0){
        tm.tv_sec = 0;
        tm.tv_usec = timeout;
        FD_ZERO(&rd);
        FD_SET(client_socket, &rd);
        if(select(client_socket + 1, NULL, &rd, NULL, &tm) > 0){
            getsockopt(client_socket, SOL_SOCKET, SO_ERROR, &erro, (socklen_t *)&len);
            if(erro == 0){
                ret = 1;     
            }else ret = 0;       
        } 
    }else{
        ret = 1;
    }
    close(client_socket);
    FD_ZERO(&rd);
    return ret;
}

//udp socket_create
int socket_udp(int port){
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_fd < 0){
        perror("UDP socket");
    }

    if(bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0){
        perror("UDP bind");
    }
    //printf("UDP bind success\n");
    return socket_fd;
}

//get the value of config file
int get_conf_value(char *pathname, char *key_name, char *value){
    FILE *fd = NULL;
    char *line = NULL;
    char *substr = NULL;
    ssize_t read = 0;
    size_t len = 0;
    

    fd = fopen(pathname, "r");
    if(fd == NULL){
        perror("Error to open");
        exit(1);
    }

    while((read = getline(&line, &len, fd)) != -1){
        DBG("%s", line);
        substr = strstr(line, key_name);
        if(substr == NULL){
            continue;
        }else{
            int tmp = strlen(key_name);
            if(line[tmp] == '='){
                strncpy(value, &line[tmp + 1], (int)read - tmp + 1);
                tmp = strlen(value);
                *(value + tmp - 1) = '\0';
                break;
            }else{
                continue;
            }
        }
    }
}


//write log to PiMonitorLog
int write_Pi_log(char *PiMonitorLog, char *format, ...){

    time_t logtime = time(NULL);
    struct tm *tmp = localtime(&logtime);
    
    int ret;
    FILE *fp;
    fp = fopen(PiMonitorLog, "a+");
    flock(fp->_fileno, LOCK_EX);
    va_list arg;
    va_start(arg, format);
    fprintf(fp, "%d.%02d.%02d %02d:%02d:%02d->log:\n", (1900 + tmp->tm_year), (1 + tmp->tm_mon), \
    tmp->tm_mday, tmp->tm_hour, tmp->tm_min, tmp->tm_sec);
    ret = vfprintf(fp, format, arg);
    fprintf(fp, "\n");
    va_end(arg);
    fflush(fp);
    fclose(fp);
    flock(fp->_fileno, LOCK_UN);
    return ret;
}

//write massege to mysql
void add_to_database(char *time, char *ip, int types, char *details, char *level){
        char database[20] = {0};
        char table[20] = {0};
        strcpy(database, "Pihealth");
    if(strcmp(level, "note") == 0){
                strcpy(table, "notes_events");
            
    }else{
                strcpy(table, "warning_events");
            
    }

        MYSQL *mysql;
        mysql = mysql_init(NULL);
        mysql_real_connect(mysql, "127.0.0.1", "root", "root", database, 0, NULL, 0);
    if(mysql){
                //printf("Connect MYSQL Success\n");
            
    }else{
                //printf("Connect MYSQL Failed\n");
            
    }
        char query[1024] = {0};
        sprintf(query, "insert into %s (time, ip, types, details)VALUES('%s', '%s', %d, '%s');", table, time, ip, types, details);
        int t = mysql_real_query(mysql, query, strlen(query));
    if(t == 0){
                //printf("Insert Success\n");
            
    }else{
                //printf("Insert Failed\n");
            
    }
    mysql_close(mysql);
}


/*int socket_non_blocking(int fd){
    int flags, s;
    flags = fcntl(fd, F_GETFL, 0);
    if(flags < 0){
        perror("fcntl");
        return 0;
    }
    
    flags |= O_NONBLOCK;

    s = fcntl(fd, F_SETFL, flags);
    if(s < 0){
        perror("fcntl2");
        return 0;
    }

    return 1;
}


#define MAX_EVENT 1000
struct epoll_event event;
struct epoll_event *events;
int event_fd;
//epoll
int init_epoll(int port){
    int serve_lesten;
    if((serve_lesten = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("creat_socket");
        exit(-1);
            
    }

    struct sockaddr_in servce_addr;
    servce_addr.sin_family = AF_INET;
    servce_addr.sin_port = htons(port);
    servce_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int yes = 1;
    if(setsockopt(serve_lesten, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
        close(serve_lesten);
        perror("setsockopt");
        return -1;
            
    }

    if(bind(serve_lesten, (struct sockaddr *)&servce_addr, sizeof(servce_addr)) < 0){
        perror("bind");
        close(serve_lesten);
        exit(-1);
            
    }
    
    if(serve_lesten < 0){
        exit(-1);
    }

    int s = socket_non_blocking(serve_lesten);
    if(s < 0){
        printf("socket_non_blocking Failed\n");
        return -1;
    }
    s = listen(serve_lesten, 10000);
    if(s < 0){
        perror("listen");
        return -1;
    }
    
    event_fd = epoll_create(MAX_EVENT);
    if(event_fd < 0){
        perror("epoll_create");
        return -1;
    }
    event.data.fd = serve_lesten;
    event.events = EPOLLIN;
    s = epoll_ctl(event_fd, EPOLL_CTL_ADD, serve_lesten, &event);
    if(s < 0){
        perror("epoll_ctl");
        return -1;
    }
    events = calloc(MAX_EVENT, sizeof event);
    //printf("tcp bind success\n");
    return serve_lesten;
}
*/
