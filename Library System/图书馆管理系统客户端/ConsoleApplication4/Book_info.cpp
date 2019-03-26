
#include "pch.h"
#include "Action.h"



void bk_info(int sclient, int flag) {
     printf("                            |----------------- Check books' information -----------------------|\n");
     while (1) {
          printf("                            |       Input:   1   for    check all books' information           |\n");
          printf("                            |                2   for    seek a book's information              |\n");
          printf("                            |                                                                  |\n");
          printf("                            |                0   for    back to last view                      |\n");
          printf("                            |                                                                  |\n");
          printf("                            |Operation number: ");
          int opt = -1;
          scanf_s("%d", &opt);
          char send_opt[2] = { 0 };
          send_opt[0] = opt + '0';
          send_opt[1] = '\0';
          char buffer[50] = { 0 };
          if (opt == 1) {
               printf("                            |------------------- check all books' information -----------------|\n");
               printf("                            |                                                                  |\n");
               send(sclient, send_opt, 2, 0);                 //send 1

               recv(sclient, buffer, 50, 0);
               while (strncmp(buffer, "end", 4) != 0) {
                    printf("                            |                          %-40s|\n", buffer);
                    printf("                            |                                                                  |\n");
                    memset(buffer, 0, 50);
                    recv(sclient, buffer, 50, 0);
               }
          }
          else if (opt == 2) {
               printf("                            |-------------------- seek a book's information -------------------|\n");
               send(sclient, send_opt, 2, 0);                //send 2
               printf("                            |                                                                  |\n");
               printf("                            |Input the book's name : ");
               int sflag = 0;
               char book_name[50] = { 0 };
               scanf_s("%s", book_name, 16);
               send(sclient, book_name, 50, 0);
               recv(sclient, buffer, 50, 0);
               printf("                            |                                                                  |\n");
               while (strcmp(buffer, "end") != 0) {
                    if (strncmp(buffer, "- Not Exit -", 12) == 0) sflag = 1;
                    printf("                            |results:                     %-20s                 |\n", buffer);
                    printf("                            |                                                                  |\n");
                    printf("                            |------------------------------------------------------------------|\n");
                    memset(buffer, 0, 50);
                    recv(sclient, buffer, 50, 0);
               }
               if (flag && !sflag) {
                    printf("                            |                                                                  |\n");
                    printf("                            |             Input 1 for   borrow this book                       |\n");
                    printf("                            |                                                                  |\n");
                    printf("                            |Operation number: ");
                    int sd = 0;
                    scanf_s("%d", &sd);
                    printf("                            |                                                                  |\n");
                    if (sd == 1) {
                         char sd_c[2] = "1";
                         send(sclient, sd_c, 2, 0);
                         printf("                            |---Borrow Success!                                                |\n");
                         printf("                            |                                                                  |\n");
                         recv(sclient, buffer, 50, 0);
                    }
                    else {
                         char sd_c[2] = "0";
                         send(sclient, sd_c, 2, 0);
                    }
               }
          }
          else if (opt == 0) {
               printf("                            |------------------------------------------------------------------|\n");
               break;
          }
          else {
               printf("                            |------------------------------------------------------------------|\n");
               printf("                            |                                                                  |\n");
               printf("                            |*****************  Wrong operation number! ***********************|\n");
               printf("                            |                                                                  |\n");
          }
          printf("                            |------------------------------------------------------------------|\n");
     }
}
