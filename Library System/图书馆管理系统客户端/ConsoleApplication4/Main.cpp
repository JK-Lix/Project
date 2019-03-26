

#include "pch.h"
#include "Action.h"


int main()
{
     WORD sockVersion = MAKEWORD(2, 2);
     WSADATA data;
     if (WSAStartup(sockVersion, &data) != 0)
     {
          return 0;
     }

     SOCKET sclient;
     

     struct sockaddr_in serAddr;
     serAddr.sin_family = AF_INET;
     serAddr.sin_port = htons(8731);
     inet_pton(AF_INET, "127.0.0.1", &serAddr.sin_addr.S_un.S_addr);
     //serAddr.sin_addr.S_un.S_addr = inet_addr("192.198.1.184")
    
     char username[16] = { 0 };
     char password[16] = { 0 };
     int user = 0;
     while (1) {
          SOCKET _sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
          if (_sclient == INVALID_SOCKET)
          {
               printf("invalid socket !");
               return 0;
          }
          sclient = _sclient;
          if (connect(sclient, (struct sockaddr *)&serAddr, sizeof(serAddr))) {
               printf("connect error !");

               closesocket(sclient);
               return 0;
          }

          char  sendData[] = "A16C8";
          int ad = 0;
          if ((ad = send(sclient, sendData, strlen(sendData), 0)) < 0) {
               printf("Connection Failed 1, Try again!\n");
          } 
          char key[10] = { 0 };
          
          if ((ad = recv(sclient, key, 10, 0)) < 0) {
               printf("Connection Failed 2, Try again!\n");
          }
          if (strcmp(key, "K31B6") == 0) {
               printf("                            Connection Success!\n");
          }
          else {
               printf("                            Connection Failed!\n");
               closesocket(sclient);
               return 0;
          }
          printf("                            Please Input Your UserName: ");
          scanf_s("%s", username, 16);
          send(sclient, username, 16, 0);

          printf("                            Password: ");
          scanf_s("%s", password, 16);
          send(sclient, password, 16, 0);
          char user_t[2] = { 0 };
          recv(sclient, user_t, 2, 0);
          user = user_t[0] - '0';
          if (user == 0) {
               printf("                            UserName or PassWord is wrong!\n");
               printf("                            Retry or Exit? input 0 to EXIT, anything to RETRY.\n");
               printf("                            ");
               int a = 1;
               scanf_s("%d", &a);
               if (a == 0) return 0;
               closesocket(sclient);
          }
          else if (user > 0){
               printf("                            Login Success!\n");
               break;
          }
     }
     printf("                            ====================================================================\n");
     switch (user) {
     case 1:
          sysadm(sclient, username);
          break;
     case 2:
          libadm(sclient, username);
          break;
     case 3:
          users(sclient, username);
          break;
     }
     printf("                            ====================================================================\n");
     closesocket(sclient);
     WSACleanup();
     Sleep(2000);
     return 0;
}



