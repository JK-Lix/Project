

#include "pch.h"
#include "Server.h"
#include "Action.h"

char username_all[16] = { 0 };    //全局变量用户名
int userflag = 0;                 //用户识别参数

//服务器初始化函数
int init_server() {
     printf("Server Start Up\n");

     WORD sockVersion = MAKEWORD(2, 2);    
     WSADATA wsaData;

     WSAStartup(sockVersion, &wsaData);
     //初始化端口和IP
     
     printf("Init Server\n");

     int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
     //创建网络套接字

     if (serverSocket < 0) {     
          //判断网络套接字是否有效

          perror("socket");
          closesocket(serverSocket);
     }

     struct sockaddr_in sockAddr;
     //创建网络连接结构体

     sockAddr.sin_family = AF_INET;     //声明IPv4连接方式
     sockAddr.sin_port = htons(8731);     //声明连接端口 8731
     sockAddr.sin_addr.S_un.S_addr = INADDR_ANY;   //任何IP地址均可连接

     if (bind(serverSocket, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) < 0) {   
          //绑定端口和本机IP，若失败，输出失败信息并关闭套接字

          perror("bind");
          closesocket(serverSocket);
     }

     if (listen(serverSocket, 1) < 0) {
          //创建端口监听，用于监听客户端的连接请求

          perror("listen");
          closesocket(serverSocket);
     }

     return serverSocket;
}


//用户登录函数
int login(int sock_fd) {
     int cnt = 0;
     int flag = 0;       //状态标识符，如果已发送即改变为1
     char password[16] = { 0 };    //识别客户端密码
     char buf[1024] = { 0 };     //TCP报文内容接收字符串
     char path[20] = { 0 };      //环境变量
     char sends[2] = "0";
     FILE* fp;
     errno_t err;

     get_path(3, path);

     if ((err = fopen_s(&fp, path, "r")) != 0) {    //打开文件
          perror("logoin");
          return 0;
     }
       
     recv(sock_fd, username_all, 16, 0);     //接收用户名
     printf("The User %s is Loggining\n", username_all);

     recv(sock_fd, password, 16, 0);        //接收密码
     while ((fgets(buf, 1024, fp)) != NULL) {     //查询文档数据
          int i;

          for (i = 0; i < strlen(buf); i++) {
               if (buf[i] == ' ') {
                    break;
               }
          }

          if (strncmp(buf, username_all, i) == 0) {    //查询用户名
               char pwd[16] = { 0 };
               int c = 0;

               i++;

               for (; i < strlen(buf); i++) {
                    if (buf[i] == ' ')
                         break;
                    pwd[c++] = buf[i];
               }

               pwd[c] = '\0';
                
               if (strncmp(password, pwd, i) == 0) {     //比对密码
                    c = 0;
                    i++;

                    for (; i < strlen(buf); i++) {
                         if (buf[i] == '\n')
                              break;
                         sends[c++] = buf[i];
                    }

                    sends[c] = '\0';

                    if ((sends[0] - '0') == 3) userflag = 1;

                    send(sock_fd, sends, 2, 0);        //登陆成功
                    printf("User %s Login success\n", username_all);    

                    fclose(fp);

                    return 1;
               }
               memset(buf, 0, 1024);
               break;
          }

          memset(buf, 0, 1024);
     }
     send(sock_fd, sends, 2, 0);

     printf("login failed\n");

     closesocket(sock_fd);
     fclose(fp);

     return 0;
}


//获取并处理客户端的操作请求
void get_opt(int sock_fd) {
     while (1) {                        //循环处理
          char _opt[2] = { 0 };

          recv(sock_fd, _opt, 3, 0);    //接收客户端操作指令

          int opt = _opt[0] - '0';

          printf("%c\n", _opt[0]);

          if (opt == -48) break;

          switch (opt) {      //解析指令，并分离操作流
          case 1:
               book(sock_fd, 1, 1, username_all, userflag);
          case 2:
               book(sock_fd, 1, 2, username_all, userflag);
               break;
          case 3:
               record(sock_fd, 3, username_all);
               break;
          case 4:
               record(sock_fd, 4, username_all);
               break;
          case 5:
               book(sock_fd, 2, 5, username_all, userflag);
               break;
          case 6:
               book(sock_fd, 2, 6, username_all, userflag);
               break;
          case 7:
               people(sock_fd, 1, 7, 0);
               break;
          case 8:
               people(sock_fd, 1, 8, 0);
               break;
          case 9:
               people(sock_fd, 1, 9, 1);
               break;
          case 10:
               people(sock_fd, 1, 10, 1);
               break;
          case 11:
               people(sock_fd, 2, 11, 0);
               break;
          case 12:
               people(sock_fd, 2, 12, 0);
               break;
          case 13:
               people(sock_fd, 2, 13, 1);
               break;
          case 14:
               people(sock_fd, 2, 14, 1);
               break;
          }
     }
}