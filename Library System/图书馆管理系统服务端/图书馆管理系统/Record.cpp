#include "pch.h"
#include "Action.h"

//���ļ�¼���������ľ���ʵ��

void record(int sock_fd, int opt, char *username) {
     char buf[50] = { 0 };
     char end[4] = "end";
     char user_name[16] = { 0 };
     char success[10] = "success";
     char path[20] = { 0 };
     //����������ʼ��

     FILE* fp;
     errno_t err;

     get_path(4, path);//��ȡ����·��

     if (opt == 4) recv(sock_fd, user_name, 16, 0);

     if ((err = fopen_s(&fp, path, "a+")) != 0) {   //���ļ�
          perror("record");
          return;
     }

     while ((fgets(buf, 50, fp)) != NULL) {    //��ȡ�ĵ�����

          int len = strlen(buf);

          if (buf[len - 1] == '\n') buf[len - 1] = '\0';   //���ݴ���

          if (opt == 3) {
               printf("%s\n", buf);
               send(sock_fd, buf, strlen(buf), 0);    //�������Ͳ�ѯ����
               Sleep(3);
          }
          else if (opt == 4) {
               int i;

               for (i = 0; i < strlen(buf); i++) {
                    if (buf[i] == ' ') {
                         break;
                    }
               }

               if (strncmp(buf, user_name, i) == 0) {      /
                    printf("%s ��ѯ����  ----   ����� %s\n", username, buf);
                    send(sock_fd, buf, strlen(buf), 0);
                    Sleep(3);

               }
          }
     }

     fclose(fp);
     Sleep(5);
     send(sock_fd, end, 4, 0);
}
