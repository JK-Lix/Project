/*************************************************************************
	> File Name: master.h
	> Author: 
	> Mail: 
	> Created Time: 2019年03月09日 星期六 18时24分43秒
 ************************************************************************/

#ifndef _MASTER_H
#define _MASTER_H
#include<stdio.h>
#include <arpa/inet.h>
#include<sys/stat.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <pthread.h>
#include <sys/epoll.h>
#endif
pthread_rwlock_t rwlock;

typedef struct Node{
    int heart_fd;
    int fd;
    struct sockaddr_in cl_addr;
    struct Node *next;
}Node, *LinkedList;


LinkedList init_link(LinkedList head){
    Node *p = (Node *)malloc(sizeof(Node));
    p->heart_fd = -1;
    p->fd = -1;
    p->cl_addr.sin_family = AF_INET;
    p->cl_addr.sin_port = htons(8731);
    p->cl_addr.sin_addr.s_addr = htons(INADDR_ANY);
    p->next = NULL;
    head = p;
    return head;
}


LinkedList insert(LinkedList head, Node *node){
    //pthread_rwlock_wrlock(&rwlock);
    Node *current_node;
    if(head == NULL){
        head = node;
        //pthread_rwlock_unlock(&rwlock);
        return head;
    }
    current_node = head;
    while(current_node->next != NULL){
        current_node = current_node->next;
    }
    current_node->next = node;
    //pthread_rwlock_unlock(&rwlock);
    return head;
}

LinkedList delete(LinkedList head, char ip[20]){
    Node *current_node = head;
    while(current_node->next != NULL){
        char *p = strdup(inet_ntoa(current_node->next->cl_addr.sin_addr));
        if(strcmp(ip, p) != 0){
            current_node = current_node->next;
            continue;
        }
        break;
    }
    if(current_node->next == NULL){
        return head;
    }
    Node *delete_node = current_node->next;
    current_node->next = delete_node->next;
    //free(delete_node);
    printf("\033[35mdelete %s success \033[0m\n", ip);
    fflush(stdout);
    return head;
}

void new_delete(LinkedList head, char *ip){
    Node *current_node = head;
    while(current_node->next != NULL){
        if(strcmp(ip, inet_ntoa(current_node->next->cl_addr.sin_addr)) != 0){
            current_node = current_node->next;
        }else{
            break;
        }
    }
    if(current_node->next == NULL){
        return;
    }
    Node *delete_node = current_node->next;
    current_node->next = delete_node->next;
    free(delete_node);
    printf("\033[35mdelete %s success \033[0m\n", ip);
    return;
}


void output(LinkedList head){
    Node *current_node = head;
    while(current_node->next != NULL){
        printf("ip: %s  \033[34m online\033[0m\n", inet_ntoa(current_node->next->cl_addr.sin_addr));
        current_node = current_node->next;
    }
}


int counter(LinkedList head){
    Node *current_node = head;
    int cnt = 0;
    while(current_node->next != NULL){
        cnt++;
        current_node = current_node->next;
    }
    return cnt;
}

char *get_ip(LinkedList head, int index){
    Node *current_node = head;
    int n = index;
    while(n){
        current_node = current_node->next;
        n--;
    }
    return inet_ntoa(current_node->cl_addr.sin_addr);
}

int check(LinkedList head, in_addr_t check_ip){
    pthread_rwlock_rdlock(&rwlock);
    Node *current_node = head;
    while(current_node->next != NULL){
        if(current_node->next->cl_addr.sin_addr.s_addr == check_ip){
            return 1;
        }
        current_node = current_node->next;
    }
    pthread_rwlock_unlock(&rwlock);
    return 0;
}

void initall(char *from, char *to, LinkedList cl_link){
    struct in_addr addrfrom;
    struct in_addr addrto;
    inet_aton(from, &addrfrom);
    inet_aton(to, &addrto);
    unsigned int nfrom = ntohl(addrfrom.s_addr);
    unsigned int nto = ntohl(addrto.s_addr);
    for(unsigned i = nfrom; i <= nto; i++){
        Node *node = (Node *)malloc(sizeof(Node));
        node->next = NULL;
        node->cl_addr.sin_addr.s_addr = htonl(i);
        insert(cl_link, node);
    }
}

int get_min(LinkedList *head, int index){
    int tmp = 0;
    int cnt = counter(head[0]);
    for(int i = 0; i < index; i++){
        if(counter(head[i]) < cnt){
            tmp = i;
            cnt = counter(head[i]);
        }
    }
    return tmp;
}


int socket_non_blocking(int fd){
    int flags, s;
    flags = fcntl(fd, F_GETFL, 0);
    if(flags < 0){
        perror("fcntl");
        return -1;
    }
    
    flags |= O_NONBLOCK;

    s = fcntl(fd, F_SETFL, flags);
    if(s < 0){
        perror("fcntl2");
        return -1;
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


