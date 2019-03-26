

#include "pch.h"
#include "Action.h"

char path[20] = { 0 };      //环境变量

//书本类，用于实现服务端对“书籍”文本数据的操作

//flag变量用于识别操作类型， opt是从客户端获取的操作数，username是客户端的用户名

//为了减少代码量，增强函数可用性，将一个函数设置多个参数以便重复利用，在C++中可以利用类的继承和重载实现这项功能

void book(int sock_fd, int flag, int opt, char* username, int userflag) {
     char buf[50] = { 0 };     //接用于网络数据接收的字符串
     char end[4] = "end";
     char success[10] = "success";
     char bookname[50] = { 0 };
     //声明各类参数

     FILE* fp;
     errno_t err;

     get_path(2, path);    //获取环境变量

     if ((err = fopen_s(&fp, path, "a+")) != 0) {     //打开文本文件
          perror("book");
          return;
     }

     if (flag == 1) {   //查找
          int stat = 0;
          char notexit[] = "- Not Exit -";

          if (opt == 2)
               recv(sock_fd, bookname, 50, 0);   
          //网络编程，TCP报文接收函数
          //将接受到的TCP报文内容存到字符串数组内

          printf("正在查找：%s\n", bookname);

          while ((fgets(buf, 50, fp)) != NULL) {  //读取文本文档的内容
               int len = strlen(buf);

               if (buf[len - 1] == '\n')
                    buf[len - 1] = '\0';   //获取文本内容后进行修改以便后续操作

               if (opt == 1) {  //操作数为1，查找所有书籍
                    Sleep(2);       
                    //主进程休眠，防止报文收发堵塞

                    send(sock_fd, buf, strlen(buf), 0); 
                    //网络编程发送函数，将字符串封装成TCP报文发送给客户端

                    printf("%s\n", buf);
                    memset(buf, 0, 50);     //初始化字符串数组
               }
               else if (opt == 2) {  //操作数为2，查找指定书籍
                    int i;

                    for (i = 0; i < strlen(buf); i++) {
                         if (buf[i] == ' ') {
                              break;
                         }
                    }

                    if (strncmp(buf, bookname, i) == 0) {
                         stat = 1;    //状态位置数，用于之后的借阅操作

                         printf("已搜索到：%s\n", buf);/////////////

                         send(sock_fd, buf, strlen(buf), 0);    //发送已查找到的内容
                         send(sock_fd, end, 4, 0);              //发送查询结束符

                         if (userflag) {   //查找到书籍后
                              char sd[2] = { 0 };

                              recv(sock_fd, sd, 2, 0);   //接收信息

                              if (sd[0] - '0' == 1) {
                                   char t[28];
                                   char t_path[] = "record.txt";       //自行定义路径
                                   struct tm *tm = (struct tm *)malloc(sizeof(struct tm));

                                   time_t t2;     //C语言时间类型结构体，用与获取借阅时间并存档
                                   FILE* frp;
                                   errno_t err;

                                   time(&t2);           //获取时间结构体内的各项数值
                                   localtime_s(tm, &t2); 
                                   asctime_s(t, tm);      //时间数值转化

                                   printf("正在收录：%s\n", buf);/////////////
                                    
                                   if ((err = fopen_s(&frp, t_path, "a+")) != 0) {     //打开t_path所指向的record.txt文件
                                        perror("record");
                                        return;
                                   }

                                   printf("借阅流程开始asdf\n");

                                   fputs(username, frp);
                                   fputs(" 借阅了《", frp);
                                   fputs(bookname, frp);
                                   fputs("》  ", frp);
                                   fputs(t, frp);
                                   //将指定内容写入到文本文档中

                                   fclose(frp);
                                   //取消文件占用，关闭并保存文本文件
                                   
                                   printf("借阅完毕\n");
                              }
                         }
                         fclose(fp);
                         break;
                    }
               }
          }

          if (!stat && opt == 2) {       //如果状态数未改变，则发送书籍不存在的预定字符串
               Sleep(2);
               send(sock_fd, notexit, strlen(notexit), 0);
          }
          printf("\n\nfuck\n\n");
          Sleep(5);

          send(sock_fd, end, 4, 0);      //发送查询结束符，告知客户端结束指令

          fclose(fp);
     }
     else if (flag = 2) { //    管理员对书籍的添加、删除操作
          Sleep(1);

          recv(sock_fd, bookname, 50, 0);     //接受管理员指定的书籍名称

          if (opt == 5) {  //如果指令位为5，则进行添加书籍操作
               char type[20] = { 0 };
               int stat = 0; //查看是否发送

               recv(sock_fd, type, 20, 0);    //接受管理员指定的书籍类型字符串

               while ((fgets(buf, 1024, fp)) != NULL) {//同上，获取文件内容
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

                         send(sock_fd, exit, strlen(exit), 0);    //查询到该书籍已存在，则发送预定字符串，书记已存在

                         stat = 1;
                         break;
                    }
               }

               if (!stat) {
                    fputs(bookname, fp);
                    fputs(" ", fp);
                    fputs(type, fp);
                    fputs("\n", fp);
                    //添加新书籍后将书籍信息放入文本文档

                    send(sock_fd, success, strlen(success), 0);
               }
               fclose(fp);
          }
          else if (opt == 6) {
               int stat = 0; //查看是否发送
               int cnt = 0;  //记录要删除的数在哪行
               char t_path[] = "temp.txt";    //同上，设置路径
               FILE *ftp;

               if ((err = fopen_s(&ftp, t_path, "a+")) != 0) {     //同上，打开文件
                    perror("tmp1\n");
                    return;
               }

               while ((fgets(buf, 1024, fp)) != NULL) {    //同上，获取文档内容
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

                    if (strncmp(buf, bookname, i) == 0) {    //获取查询内容，进行数据比较
                         stat = 1;
                         continue;
                    }

                    fputs(buf, ftp);
                    fputs("\n", ftp);
               }

               if (!stat) {
                    char noexit[] = "This book is not exit";

                    send(sock_fd, noexit, strlen(noexit), 0);      //查询结果不存在，发送预定字符串

                    return;
               }

               fclose(ftp);   
               fclose(fp);    //解除文件占用，保存并关闭文件
               remove(path);   //移除原文件
               rename(t_path, path);   //保存已修改的新文件

               send(sock_fd, success, strlen(success), 0);    //发送操作成功字符串，结束操作。
          }
     }
}
