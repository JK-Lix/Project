/*************************************************************************
	> File Name: ServerTc.c
	> Author: Lix
	> Mail: 776579471@qq.com
	> Created Time: 2019年02月23日 星期六 10时18分36秒
 ************************************************************************/

#include "../common/common.h"

typedef struct _Message{
    char from[20];
    int flag;
    char message[1024];
} Message;

typedef struct Node {
    char Name[20];
    int sd;
    struct Node *next;
} Node, *LinkedList;

struct Para {
    int sock_fd;
    int server_listen;
    char Name[20];
};

int count = 0;
char sysmention[] = "System Mention";
LinkedList head = NULL;


void logout(int sig) {
    printf("\033[1m\033[43mServer Shutdown\033[0m \n");
    fclose(stdout);
    _exit(0);
}


LinkedList init(LinkedList head) {
   
}


LinkedList insert_node(LinkedList head, char *Name, int sd) {
    Node *node = (Node *)malloc(sizeof(Node));
    LinkedList current_node = head;
    
    strcpy(node->Name, Name);
    node->sd = sd;
    node->next = NULL;
    
    while (current_node->next != NULL) {
        current_node = current_node->next;
    }

    current_node->next = node;
    count++;
    
    return head;
}


LinkedList find_node(LinkedList head, char Name[]) {
    Node *current_node = head;
    
    while(strcmp(current_node->next->Name, Name) != 0 && current_node->next != NULL) {
        current_node = current_node->next;
    }
    
    if (current_node->next == NULL) return NULL;
    else return current_node->next;
}


int check(LinkedList head, char *Name) {
    Node *node = head;
    
    while (node->next != NULL && strcmp(node->next->Name, Name) != 0) {
        node = node->next;
    }
    
    if (node->next == NULL) return 0;
    
    return 1;
}


void delete_node(char *Name) {
    LinkedList current_node = head;
    
    while(current_node->next != NULL && strcmp(current_node->next->Name, Name) != 0) {
        current_node = current_node->next;
    }
    
    if (current_node->next == NULL) return;
    
    Node *delete_node = current_node->next;
    current_node->next = delete_node->next;
    count--;
    
    free(delete_node);
}


void send_all_message(LinkedList head, char from[], char message[], int flag) {
    Node* current_node = head;
    Message sendmessage;
    memset(&sendmessage, 0, sizeof(Message));
    
    strcpy(sendmessage.from, from);
    sendmessage.flag = flag;
    strcpy(sendmessage.message, message);
    
    if (head != NULL) 
        send(current_node->sd, (char*)&sendmessage, sizeof(Message), 0);
    
    while (current_node->next != NULL) {
        int jud = send(current_node->next->sd, (char*)&sendmessage, sizeof(Message), 0);
        
        if (jud < 0) {
            printf("send fail sys->%s", current_node->next->Name);
        }

        current_node = current_node->next;
    }
}


void send_prv_message(LinkedList head, char *from, char *smessage, char *to, int flag) {
    Message sendmessage;
    
    memset(&sendmessage, 0, sizeof(Message));
    
    strcpy(sendmessage.from, from);
    sendmessage.flag = flag;
    strcpy(sendmessage.message, smessage);
    
    LinkedList current_node = find_node(head, to);
    send(current_node->sd,(char *)&sendmessage, sizeof(Message), 0);
}


void Welcome_Buff(char *Name) {
    char send_buff[1024];
    
    sprintf(send_buff, "\033[1mWelcome User \033[31m%s\033[30m Join Us !\033[0m", Name);
    send_prv_message(head, sysmention, send_buff, Name, 2);
    memset(send_buff, 0, strlen(send_buff));
    
    sprintf(send_buff, "User \033[31m%s\033[30m is online. There are \033[35m%d\033[30m users online.\033[0m", Name, count);
    printf("\033[1m\033[40m--System Mention--:\033[47;30m%s\033[0m\n", send_buff);
    send_all_message(head, sysmention, send_buff, 2);
}


void check_users(char *recv_buff, char *Name) {
    char send_buff[1024];
    Node *node = head;
    
    printf("\033[1m\033[40m--System Mention--:\033[47;30m\033[1mUser \033[31m%s\033[30m check the number of online users!\033[0m\n", Name);
    sprintf(send_buff, "There are \033[35m%d\033[30m users :", count);
    
    while (node->next != NULL) {
        sprintf(send_buff, "%s\033[1m  \033[31m%s", send_buff, node->next->Name);
        node = node->next;
    }

    printf("\033[1m\033[40m--System Mention--:\033[47;30m%s\033[0m\n", send_buff);
    send_prv_message(head, sysmention, send_buff, Name, 2);
}


void prv_users(char *recv_buff, char *Name) {
    char send_buff[1024] = {0};
    char sendto[20];        
    char prvmess[1024] = {0};
    int i, j, cn;
    Node *node = head;

    for (i = 1; i < 20; i++) {
        if (recv_buff[i] != ' ') {
            sendto[i - 1] = recv_buff[i];
        } else {
            break;
        }
    }
    cn = 1024 + i;
    sendto[i - 1] = '\0';
    i++;

    while (node->next != NULL && strcmp(node->next->Name, sendto) != 0) {
        node = node->next;
    }
    if (node->next == NULL) {
        sprintf(send_buff, "The users \033[31m%s\033[30m is OFFLINE or Not Exit!", sendto);
        printf("\033[1m\033[40m--System Mention--:\033[47;30m%s\033[0m\n",send_buff);
        send_prv_message(head, sysmention, send_buff, Name, 2);
    } else {
        node = node->next;
        for (j = 0;i < cn; i++) {
            if (recv_buff[i] == '\0') break;
            prvmess[j++] = recv_buff[i];
        }
        prvmess[j] = '\0';
        
        printf("\033[1m\033[36m%s to %s\033[0m\033[1m:%s\033[0m\n", Name, sendto, prvmess);
        send_prv_message(head, Name, prvmess, sendto, 1);
    }
}


void offline(char * Name) {
    char send_buff[1024];
    
    sprintf(send_buff, "User \033[31m%s \033[30m is OFFLINE\033[0m", Name);
    printf("\033[1m\033[40m--System Mention--:\033[47;30m%s\033[0m\n",send_buff);
    send_all_message(head, sysmention, send_buff, 2);
    delete_node(Name);
}



void *do_recv(void *argv) {
    char recv_buff[1024] = {0};
    struct Para *para = (struct Para *)argv;
    int server_listen = para->server_listen;
    int sock_fd = para->sock_fd;
    char Name[20];

    strcpy(Name, para->Name);
    head = insert_node(head, Name, sock_fd);
    Welcome_Buff(Name);

    while(recv(sock_fd, recv_buff, sizeof(recv_buff), 0) > 0) {
        if (strncmp(recv_buff, "#", 1) == 0) {
            check_users(recv_buff, Name);
        } else if (recv_buff[0] == '@') {
            prv_users(recv_buff, Name);
        } else {
            printf("\033[1m\033[36m%s\033[37m:%s\n", Name, recv_buff);
            send_all_message(head, Name, recv_buff, 0);
        }

        if (strcmp(recv_buff, "exit") == 0) {
            break;
        }

        fflush(stdout);
        memset(recv_buff, 0, sizeof(recv_buff));
    }

    offline(Name);
    close(sock_fd);
}



int main(int argc, char *argv[]) {
    char *config = "./chat.conf";
    char Temp_Port[5] = {0};
    int Server_Port, server_listen;
    pthread_t pid;
    Node *node = (Node *)malloc(sizeof(Node));
    
    node->sd = 0;
    strcpy(node->Name, "init");
    node->next = NULL;
    head = node;
    signal(SIGINT, logout);   
    
    get_conf_value(config, "Server_Port", Temp_Port);
    Server_Port = atoi(Temp_Port);

    server_listen = socket_create(Server_Port, 100);
    if (server_listen > 0) {
        printf("\033[1m\033[43mServer start up success!\033[0m\n");
    } else {
        printf("\033[1m\033[43mServer can't start up!\033[0m\n");
        return -1;
    }

    while (1) {
        char Name[20] = {0};
        pthread_t pid;
        struct Para para;
        int sock_fd;

        sock_fd = accept(server_listen, NULL, NULL);
        if (sock_fd < 0) {
            continue;
        } else {
            para.server_listen = server_listen;
            para.sock_fd = sock_fd;
        }

        recv(sock_fd, Name, 20, 0);
        
        if (check(head, Name)) {
            char send[1024];
            sprintf(send, "The user name \033[31m%s\033[30m has already exist!", Name);
            send_prv_message(head, sysmention, send, Name, 2);
            close(sock_fd);
            continue;
        }

        strcpy(para.Name, Name);
        
        pthread_create(&pid, NULL, do_recv, (void *)&para);
    }

    return 0;
}
