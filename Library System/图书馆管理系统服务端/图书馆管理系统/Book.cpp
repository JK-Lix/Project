

#include "pch.h"
#include "Action.h"

char path[20] = { 0 };      //��������

//�鱾�࣬����ʵ�ַ���˶ԡ��鼮���ı����ݵĲ���

//flag��������ʶ��������ͣ� opt�Ǵӿͻ��˻�ȡ�Ĳ�������username�ǿͻ��˵��û���

//Ϊ�˼��ٴ���������ǿ���������ԣ���һ���������ö�������Ա��ظ����ã���C++�п���������ļ̳к�����ʵ�������

void book(int sock_fd, int flag, int opt, char* username, int userflag) {
     char buf[50] = { 0 };     //�������������ݽ��յ��ַ���
     char end[4] = "end";
     char success[10] = "success";
     char bookname[50] = { 0 };
     //�����������

     FILE* fp;
     errno_t err;

     get_path(2, path);    //��ȡ��������

     if ((err = fopen_s(&fp, path, "a+")) != 0) {     //���ı��ļ�
          perror("book");
          return;
     }

     if (flag == 1) {   //����
          int stat = 0;
          char notexit[] = "- Not Exit -";

          if (opt == 2)
               recv(sock_fd, bookname, 50, 0);   
          //�����̣�TCP���Ľ��պ���
          //�����ܵ���TCP�������ݴ浽�ַ���������

          printf("���ڲ��ң�%s\n", bookname);

          while ((fgets(buf, 50, fp)) != NULL) {  //��ȡ�ı��ĵ�������
               int len = strlen(buf);

               if (buf[len - 1] == '\n')
                    buf[len - 1] = '\0';   //��ȡ�ı����ݺ�����޸��Ա��������

               if (opt == 1) {  //������Ϊ1�����������鼮
                    Sleep(2);       
                    //���������ߣ���ֹ�����շ�����

                    send(sock_fd, buf, strlen(buf), 0); 
                    //�����̷��ͺ��������ַ�����װ��TCP���ķ��͸��ͻ���

                    printf("%s\n", buf);
                    memset(buf, 0, 50);     //��ʼ���ַ�������
               }
               else if (opt == 2) {  //������Ϊ2������ָ���鼮
                    int i;

                    for (i = 0; i < strlen(buf); i++) {
                         if (buf[i] == ' ') {
                              break;
                         }
                    }

                    if (strncmp(buf, bookname, i) == 0) {
                         stat = 1;    //״̬λ����������֮��Ľ��Ĳ���

                         printf("����������%s\n", buf);/////////////

                         send(sock_fd, buf, strlen(buf), 0);    //�����Ѳ��ҵ�������
                         send(sock_fd, end, 4, 0);              //���Ͳ�ѯ������

                         if (userflag) {   //���ҵ��鼮��
                              char sd[2] = { 0 };

                              recv(sock_fd, sd, 2, 0);   //������Ϣ

                              if (sd[0] - '0' == 1) {
                                   char t[28];
                                   char t_path[] = "record.txt";       //���ж���·��
                                   struct tm *tm = (struct tm *)malloc(sizeof(struct tm));

                                   time_t t2;     //C����ʱ�����ͽṹ�壬�����ȡ����ʱ�䲢�浵
                                   FILE* frp;
                                   errno_t err;

                                   time(&t2);           //��ȡʱ��ṹ���ڵĸ�����ֵ
                                   localtime_s(tm, &t2); 
                                   asctime_s(t, tm);      //ʱ����ֵת��

                                   printf("������¼��%s\n", buf);/////////////
                                    
                                   if ((err = fopen_s(&frp, t_path, "a+")) != 0) {     //��t_path��ָ���record.txt�ļ�
                                        perror("record");
                                        return;
                                   }

                                   printf("�������̿�ʼasdf\n");

                                   fputs(username, frp);
                                   fputs(" �����ˡ�", frp);
                                   fputs(bookname, frp);
                                   fputs("��  ", frp);
                                   fputs(t, frp);
                                   //��ָ������д�뵽�ı��ĵ���

                                   fclose(frp);
                                   //ȡ���ļ�ռ�ã��رղ������ı��ļ�
                                   
                                   printf("�������\n");
                              }
                         }
                         fclose(fp);
                         break;
                    }
               }
          }

          if (!stat && opt == 2) {       //���״̬��δ�ı䣬�����鼮�����ڵ�Ԥ���ַ���
               Sleep(2);
               send(sock_fd, notexit, strlen(notexit), 0);
          }
          printf("\n\nfuck\n\n");
          Sleep(5);

          send(sock_fd, end, 4, 0);      //���Ͳ�ѯ����������֪�ͻ��˽���ָ��

          fclose(fp);
     }
     else if (flag = 2) { //    ����Ա���鼮����ӡ�ɾ������
          Sleep(1);

          recv(sock_fd, bookname, 50, 0);     //���ܹ���Աָ�����鼮����

          if (opt == 5) {  //���ָ��λΪ5�����������鼮����
               char type[20] = { 0 };
               int stat = 0; //�鿴�Ƿ���

               recv(sock_fd, type, 20, 0);    //���ܹ���Աָ�����鼮�����ַ���

               while ((fgets(buf, 1024, fp)) != NULL) {//ͬ�ϣ���ȡ�ļ�����
                    int i;
                    int len = strlen(buf);

                    if (buf[len - 1] == '\n')
                         buf[len - 1] = '\0';

                    for (i = 0; i < strlen(buf); i++) {
                         if (buf[i] == ' ') {
                              break;
                         }
                    }

                    if (strncmp(buf, bookname, i) == 0) {
                         char exit[50] = "The book is already exit!";

                         send(sock_fd, exit, strlen(exit), 0);    //��ѯ�����鼮�Ѵ��ڣ�����Ԥ���ַ���������Ѵ���

                         stat = 1;
                         break;
                    }
               }

               if (!stat) {
                    fputs(bookname, fp);
                    fputs(" ", fp);
                    fputs(type, fp);
                    fputs("\n", fp);
                    //������鼮���鼮��Ϣ�����ı��ĵ�

                    send(sock_fd, success, strlen(success), 0);
               }
               fclose(fp);
          }
          else if (opt == 6) {
               int stat = 0; //�鿴�Ƿ���
               int cnt = 0;  //��¼Ҫɾ������������
               char t_path[] = "temp.txt";    //ͬ�ϣ�����·��
               FILE *ftp;

               if ((err = fopen_s(&ftp, t_path, "a+")) != 0) {     //ͬ�ϣ����ļ�
                    perror("tmp1\n");
                    return;
               }

               while ((fgets(buf, 1024, fp)) != NULL) {    //ͬ�ϣ���ȡ�ĵ�����
                    int i;
                    int len = strlen(buf);

                    cnt++;

                    if (buf[len - 1] == '\n')
                         buf[len - 1] = '\0';

                    for (i = 0; i < strlen(buf); i++) {
                         if (buf[i] == ' ') {
                              break;
                         }
                    }

                    if (strncmp(buf, bookname, i) == 0) {    //��ȡ��ѯ���ݣ��������ݱȽ�
                         stat = 1;
                         continue;
                    }

                    fputs(buf, ftp);
                    fputs("\n", ftp);
               }

               if (!stat) {
                    char noexit[] = "This book is not exit";

                    send(sock_fd, noexit, strlen(noexit), 0);      //��ѯ��������ڣ�����Ԥ���ַ���

                    return;
               }

               fclose(ftp);   
               fclose(fp);    //����ļ�ռ�ã����沢�ر��ļ�
               remove(path);   //�Ƴ�ԭ�ļ�
               rename(t_path, path);   //�������޸ĵ����ļ�

               send(sock_fd, success, strlen(success), 0);    //���Ͳ����ɹ��ַ���������������
          }
     }
}
