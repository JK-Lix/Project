/*************************************************************************
	> File Name: common.c
	> Author: Lix
	> Mail: 776579471@qq.com
	> Created Time: 2019年02月23日 星期六 11时07分53秒
 ************************************************************************/

#include "common.h"

int get_conf_value(char *pathname, char *key_name, char *value) {
    FILE *fd = NULL;
    ssize_t read = 0;
    size_t len = 0;
    char *line = NULL;
    char *substr = NULL;
    fd = fopen(pathname, "r");

    if(fd == NULL) {
        printf("error in opening\n");
        exit(1);
    }

    while((read = getline(&line, &len, fd)) != 1) {
        //printf("%s", line);
        fflush(stdout);
        substr = strstr(line, key_name);
        if(substr == NULL) {
            continue;
        } else {
            int tmp = strlen(key_name);
            if(line[tmp] == '=') {
                strncpy(value, &line[tmp + 1], (int)read - tmp - 1);
                tmp = strlen(value);
                *(value + tmp - 1) = '\0';
                break;
            } else {
                printf("next\n");
                continue;
            }
        }
    }
}

int socket_create(int port, int num) {
    int server_listen;
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((server_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("create socket");
        return -1;
    }

    if (bind(server_listen, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_listen);
        return -1;
    }

    if (listen(server_listen, 5) < 0) {
        perror("listen");
        close(server_listen);
        return -1;
    }
    
    return server_listen;
}

int socket_connect(int port, char *host) {
    struct sockaddr_in ServerAddr = {0};
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(port);
    ServerAddr.sin_addr.s_addr = inet_addr(host);
    
    int sd = socket(AF_INET,SOCK_STREAM,0);
    
    if (sd == -1) {
        perror("create socket");
        return -1;
    
    }
    
    int jud = connect(sd, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
    if (jud == -1) {
        perror("Connect");
        close(sd);
        return -1;
    }

    return sd;
}
