/*************************************************************************
	> File Name: proccal.c
	> Author: Lix
	> Mail: 776579471@qq.com
	> Created Time: 2019年02月21日 星期四 15时01分38秒
 ************************************************************************/

#include "../common/common.h"

typedef struct Message{
    char from[20];
    int flag;
    char message[1024];
} Message;

void receive_message(int sd) {
    struct Message smessage;
    while (recv(sd, (char *) &smessage, sizeof(smessage), 0) > 0) {
        freopen("../chat.log", "a+", stdout);
        if (smessage.flag == 0) {
            printf("\033[1m\033[34m%s\033[0m:%s\n", smessage.from, smessage.message);
        } else if (smessage.flag == 2) {
            printf("\033[1m\033[40m--System Mention--\033[47;30m%s\033[0m\n", smessage.message);
        } else if (smessage.flag == 1){
            printf("\033[1m\033[34m%s\033[33m*\033[34m:\033[37m%s\033[0m\n", smessage.from, smessage.message);
        }
        fflush(stdout);
        fclose(stdout);
        memset(&smessage, 0, sizeof(Message));
    }
    close(sd);
}

void screen_print() {
    system("clear");
    system("tail -f ../chat.log");
    system("echo ' ';echo '          '`date +'%Y-%m-%d %H:%M'` >> ../recording.log");
    system("echo `cat ../chat.log >> ../recording.log`");
    system("rm -f ../chat.log;touch ../chat.log");
}

int main(int argc, char *argv[]) {
    char My_Name[20] = {0};
    char *config = "./chat.conf";
    char Temp_Ports[5] = {0};
    int pid = 0, Client_Port;
    char IP[16];
    
    get_conf_value(config, "User_Name", My_Name);
    get_conf_value(config, "IP", IP);
    get_conf_value(config, "Server_Port", Temp_Ports);

    Client_Port = atoi(Temp_Ports);

    if ((pid = fork()) < 0) {
        printf("Error in fork!\n");
        exit(0);
    }
    if (pid == 0) {
        screen_print();
        exit(0);
    } else {
        int sd;
        char buffer[1024];
        sd = socket_connect(Client_Port, IP);
        if (sd != -1) {
            printf("Connection success!\n");
            if (-1 == send(sd, My_Name, sizeof(My_Name), 0)) {
                perror("send");
            }
        } else {
            system("ps -ef|grep tail|awk '{print $2}'|head -2|xargs -i kill -9 {}");
            perror("Error");
            return 0;
        }
        char sav_buffer[1024];
        
        int mpid = 0;
        if ((mpid = fork()) < 0) {
            printf("Error in fork!\n");
            exit(0);
        }
        if (mpid == 0) {
            while (1) {
                int i = 0;
                scanf("%c", &buffer[i]);
                while (buffer[i] != '\n') {
                    i++;
                    scanf("%c", &buffer[i]);
                }
                buffer[i] = '\0';
                if (buffer[0] == '\0') {
                    printf("Can't be empty!\n");
                    continue;
                }
                //while(1) {
                if (-1 == send(sd, buffer, strlen(buffer), 0)) {
                    printf("massage send failed!\n");
                    continue;
                }
                //}     //用于服务器的暴力测试循环
                if (strcmp(buffer, "exit") == 0) {
                    close(sd);
                    break;
                }
                memset(buffer, 0, sizeof(buffer));
            }
            close(sd);
            system("ps -ef|grep tail|awk '{print $2}'|head -2|xargs -i kill -9 {}");
            printf("\033[1m\033[47;30mThanks for Using!\033[0m\n");
            exit(0);
        } else {
            int mpid = 0;
            if ((mpid = fork()) < 0) {
                printf("error\n");
            }
            if (mpid == 0) {
                receive_message(sd);
            } else {
                receive_message(sd);
            }
            close(sd);
            exit(0);
        }
        close(sd);
        printf("\033[1m\033[47;30mThanks for Using!\033[0m\n");
        exit(0);
    }
    return 0;
}
