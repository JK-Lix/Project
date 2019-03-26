

#include "pch.h"
#include "Server.h"
#include "Action.h"

char username_all[16] = { 0 };    //ȫ�ֱ����û���
int userflag = 0;                 //�û�ʶ�����

//��������ʼ������
int init_server() {
     printf("Server Start Up\n");

     WORD sockVersion = MAKEWORD(2, 2);    
     WSADATA wsaData;

     WSAStartup(sockVersion, &wsaData);
     //��ʼ���˿ں�IP
     
     printf("Init Server\n");

     int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
     //���������׽���

     if (serverSocket < 0) {     
          //�ж������׽����Ƿ���Ч

          perror("socket");
          closesocket(serverSocket);
     }

     struct sockaddr_in sockAddr;
     //�����������ӽṹ��

     sockAddr.sin_family = AF_INET;     //����IPv4���ӷ�ʽ
     sockAddr.sin_port = htons(8731);     //�������Ӷ˿� 8731
     sockAddr.sin_addr.S_un.S_addr = INADDR_ANY;   //�κ�IP��ַ��������

     if (bind(serverSocket, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) < 0) {   
          //�󶨶˿ںͱ���IP����ʧ�ܣ����ʧ����Ϣ���ر��׽���

          perror("bind");
          closesocket(serverSocket);
     }

     if (listen(serverSocket, 1) < 0) {
          //�����˿ڼ��������ڼ����ͻ��˵���������

          perror("listen");
          closesocket(serverSocket);
     }

     return serverSocket;
}


//�û���¼����
int login(int sock_fd) {
     int cnt = 0;
     int flag = 0;       //״̬��ʶ��������ѷ��ͼ��ı�Ϊ1
     char password[16] = { 0 };    //ʶ��ͻ�������
     char buf[1024] = { 0 };     //TCP�������ݽ����ַ���
     char path[20] = { 0 };      //��������
     char sends[2] = "0";
     FILE* fp;
     errno_t err;

     get_path(3, path);

     if ((err = fopen_s(&fp, path, "r")) != 0) {    //���ļ�
          perror("logoin");
          return 0;
     }
       
     recv(sock_fd, username_all, 16, 0);     //�����û���
     printf("The User %s is Loggining\n", username_all);

     recv(sock_fd, password, 16, 0);        //��������
     while ((fgets(buf, 1024, fp)) != NULL) {     //��ѯ�ĵ�����
          int i;

          for (i = 0; i < strlen(buf); i++) {
               if (buf[i] == ' ') {
                    break;
               }
          }

          if (strncmp(buf, username_all, i) == 0) {    //��ѯ�û���
               char pwd[16] = { 0 };
               int c = 0;

               i++;

               for (; i < strlen(buf); i++) {
                    if (buf[i] == ' ')
                         break;
                    pwd[c++] = buf[i];
               }

               pwd[c] = '\0';
                
               if (strncmp(password, pwd, i) == 0) {     //�ȶ�����
                    c = 0;
                    i++;

                    for (; i < strlen(buf); i++) {
                         if (buf[i] == '\n')
                              break;
                         sends[c++] = buf[i];
                    }

                    sends[c] = '\0';

                    if ((sends[0] - '0') == 3) userflag = 1;

                    send(sock_fd, sends, 2, 0);        //��½�ɹ�
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


//��ȡ������ͻ��˵Ĳ�������
void get_opt(int sock_fd) {
     while (1) {                        //ѭ������
          char _opt[2] = { 0 };

          recv(sock_fd, _opt, 3, 0);    //���տͻ��˲���ָ��

          int opt = _opt[0] - '0';

          printf("%c\n", _opt[0]);

          if (opt == -48) break;

          switch (opt) {      //����ָ������������
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