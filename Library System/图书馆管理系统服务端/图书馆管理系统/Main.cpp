// 图书馆管理系统.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "Server.h"
#include "Action.h"


int main() {
     int serverSocket = init_server();     //初始化服务器
     struct sockaddr_in client_sin;         //定义网络连接结构体
     int len = sizeof(client_sin);

     printf("Server is Ready!\n");

     while (1) {
          char key[10] = { 0 };    //初始化密钥字符串

          int sock_fd = accept(serverSocket, (struct sockaddr*)&client_sin, &len);
          //调用accept函数，用于接受客户端的连接请求

          if (sock_fd < 0) {    
               //判断用户是否连接上服务器

               perror("accept");
               closesocket(sock_fd);     
               //未连接上服务器则关闭网路套接字，继续监听端口等待连接

               continue;
          }

          printf("Connection Accept\n");

          recv(sock_fd, key, 10, 0);     
          // 接收通讯密文，判断为正常客户端还是恶意网络攻击

          if (strcmp(key, "A16C8") != 0) {          
               //对通讯密钥进行比对

               printf("Network Attack !\n");
               closesocket(sock_fd);
          }
          else {
               char skey[] = "K31B6";       //发送服务端密钥，防止非法服务器

               send(sock_fd, skey, sizeof(skey), 0);

               printf("Connection Success\n");
          }

          int jud = login(sock_fd);      //调用登录函数，用于识别客户端的身份

           //若身份未识别，则关闭套接字，关闭TCP网络连接，初始化服务
          if (jud == 0) {
               closesocket(sock_fd);
               continue;      
          }
          
          get_opt(sock_fd);     
          //进入操作数循环，获取并处理操作客户端操作数
          //此处可改写成多线程，以便实现多客户端同时连接
          //但也要处理高并发问题
          //再加上冲突阻断和读写锁，保护数据完整性
          //用以完善本系统功能,由于时间问题，后续完善。

          printf("Server Continue Waiting for Connection!\n");
          closesocket(sock_fd);       //结束服务，关闭套接字，初始化服务。
     }
     return 0;
}

