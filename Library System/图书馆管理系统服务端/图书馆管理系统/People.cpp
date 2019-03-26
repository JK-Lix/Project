#include "pch.h"
#include "Action.h"


//系统管理员对用户和图书管理员操作函数的具体实现

void people(int sock_fd, int flag, int opt, int mod) {
     char path[20] = { 0 };


     FILE* fp;
     char buf[50] = { 0 };
     char end[4] = "end";
     char success[10] = "success";
     char name[16] = { 0 };
     errno_t err;
     //初始化函数变量

     if (mod == 0)  get_path(1, path);
     else if (mod == 1) get_path(6, path);     //根据操作模式来获取不同的路径

     if ((err = fopen_s(&fp, path, "a+")) != 0) {    //打开文件
          perror("admin/user");
          printf("%s\n", path);
          return;
     }

     if (flag == 1) {   //操作数为1，执行查找操作
          int stat = 0;

          if ((opt == 8 && mod == 0) || (opt == 10 && mod == 1))
               recv(sock_fd, name, 16, 0);

          while ((fgets(buf, 1024, fp)) != NULL) {
               int len = strlen(buf);

               if (buf[len - 1] == '\n')
                    buf[len - 1] = '\0';

               if ((opt == 7 && mod == 0) || (opt == 9 && mod == 1)) {
                    Sleep(1);
                    send(sock_fd, buf, strlen(buf), 0);
               }
               else if (opt == 8 || opt == 10) {
                    int i;

                    for (i = 0; i < strlen(buf); i++) {
                         if (buf[i] == ' ') {
                              break;
                         }
                    }

                    if (strncmp(buf, name, i) == 0) {
                         send(sock_fd, buf, strlen(buf), 0);
                         stat = 1;
                         break;
                    }
               }
          }

          fclose(fp);
          Sleep(1);

          if (!stat && (opt == 8 || opt == 10)) {
               char notexit[] = "- Not Exit -";
               send(sock_fd, notexit, strlen(notexit), 0);
          }

          send(sock_fd, end, 4, 0);
     }
     else if (flag = 2) { //    添加、删除

          recv(sock_fd, name, 50, 0);

          if ((opt == 11 && mod == 0) || (opt == 13 && mod == 1)) {  //添加
               char password[20] = { 0 };
               int stat = 0; //查看是否发送

               recv(sock_fd, password, 20, 0);
               while ((fgets(buf, 1024, fp)) != NULL) {
                    int i;
                    int len = strlen(buf);

                    if (buf[len - 1] == '\n')
                         buf[len - 1] = '\0';

                    for (i = 0; i < strlen(buf); i++) {
                         if (buf[i] == ' ') {
                              break;
                         }
                    }

                    if (strncmp(buf, name, i) == 0) {
                         char exit[50] = { 0 };

                         stat = 1;

                         if (mod == 0) strcpy_s(exit, 25, "This admin is exit");
                         else if (mod == 1) strcpy_s(exit, 25, "This user is exit");

                         send(sock_fd, exit, strlen(exit), 0);

                         fclose(fp);
                         break;
                    }
               }

               if (!stat) {

                    fputs(name, fp);
                    fputs(" ", fp);
                    fputs(password, fp);
                    fputs("\n", fp);

                    FILE* fip;
                    char i_path[] = "login.txt";
                    char _mod[2];

                    if ((err = fopen_s(&fip, i_path, "a+")) != 0) {
                         perror("login");
                         return;
                    }

                    fputs(name, fip);
                    fputs(" ", fip);
                    fputs(password, fip);
                    fputs(" ", fip);

                    _mod[0] = mod + 2 + '0';
                    _mod[1] = '\0';

                    fputs(_mod, fip);
                    fputs("\n", fip);

                    send(sock_fd, success, strlen(success), 0);

                    fclose(fip);
               }

               fclose(fp);
               return;
          }
          else if ((opt == 12 && mod == 0) || (opt == 14 && mod == 1)) {   //删除
               char t_path[] = "temp.txt";
               int stat = 0; //查看是否发送
               FILE *ftp;

               if ((err = fopen_s(&ftp, t_path, "a+")) != 0) {
                    perror("tmp1\n");
                    return;
               }

               while ((fgets(buf, 1024, fp)) != NULL) {
                    int i;
                    int len = strlen(buf);

                    if (buf[len - 1] == '\n')
                         buf[len - 1] = '\0';

                    for (i = 0; i < strlen(buf); i++) {
                         if (buf[i] == ' ') {
                              break;
                         }
                    }

                    if (strncmp(buf, name, i) == 0) {
                         stat = 1;
                         continue;
                    }

                    fputs(buf, ftp);
                    fputs("\n", ftp);
               }
               if (!stat) {
                    char noexit[50];

                    if (mod == 0) strcpy_s(noexit, 25, "This admin is not exit");
                    else if (mod == 1) strcpy_s(noexit, 25, "This user is not exit");

                    send(sock_fd, noexit, strlen(noexit), 0);

                    fclose(fp);
                    fclose(ftp);
                    remove(t_path);

                    Sleep(1);
                    return;
               }

               fclose(ftp);
               fclose(fp);

               remove(path);
               rename(t_path, path);

               get_path(3, path);

               if ((err = fopen_s(&fp, path, "a+")) != 0) {
                    perror("login");
                    return;
               }

               if ((err = fopen_s(&ftp, t_path, "a+")) != 0) {
                    perror("temp");
                    return;
               }

               while ((fgets(buf, 1024, fp)) != NULL) {
                    int i;
                    int len = strlen(buf);

                    if (buf[len - 1] == '\n') buf[len - 1] = '\0';

                    for (i = 0; i < strlen(buf); i++) {
                         if (buf[i] == ' ') {
                              break;
                         }
                    }

                    if (strncmp(buf, name, i) == 0) {
                         stat = 1;
                         continue;
                    }

                    fputs(buf, ftp);
                    fputs("\n", ftp);
               }
               fclose(ftp);
               fclose(fp);
               remove(path);
               rename(t_path, path);

               send(sock_fd, success, strlen(success), 0);

               Sleep(1);
               return;
          }
     }
}
