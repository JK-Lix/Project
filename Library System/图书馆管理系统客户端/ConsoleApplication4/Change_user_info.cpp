
#include "pch.h"
#include "Action.h"


void s_change_info(int sclient, int flag) {
     if (flag) printf("                            |---------------- change library administrator --------------------|\n");
     else  printf("                            |--------------------- add/delete a users -------------------------|\n");
     while (1) {
          printf("                            |       Input:   1   for    add a new one                          |\n");
          printf("                            |                2   for    delete an old one                      |\n");
          printf("                            |                                                                  |\n");
          printf("                            |                0   for    back to last view                      |\n");
          printf("                            |                                                                  |\n");
          printf("                            |Operation number: ");
          int opt = -1;
          scanf_s("%d", &opt);
          char buffer[50] = { 0 };
          if (opt == 1) {

               printf("                            |                                                                  |\n");
               printf("                            |------------------------- add a new one --------------------------|\n");
               if (flag) {
                    char send_opt[2] = { 0 };
                    send_opt[0] = opt + 10 + '0';             // send 11
                    send_opt[1] = '\0';
                    send(sclient, send_opt, 2, 0);
               }
               else {
                    char send_opt[2] = { 0 };
                    send_opt[0] = opt + 12 + '0';            // send 13
                    send_opt[1] = '\0';
                    send(sclient, send_opt, 2, 0);
               }

               printf("                            |                                                                  |\n");
               printf("                            |Input the name : ");
               char user_name[16] = { 0 };
               char c;
               scanf_s("%s", user_name, 16);
               send(sclient, user_name, 16, 0);
               printf("                            |Input the password : ");
               char password[16] = { 0 };
               scanf_s("%s", password, 16);
               send(sclient, password, 16, 0);
               recv(sclient, buffer, 50, 0);
               printf("                            |                                                                  |\n");
               printf("                            |      %-50s          |\n", buffer);
               memset(buffer, 0, 50);

               printf("                            |                                                                  |\n");
          }
          else if (opt == 2) {
               printf("                            |------------------------ delete an old ---------------------------|\n");
               if (flag) {
                    char send_opt[2] = { 0 };
                    send_opt[0] = opt + 10 + '0';             // send 12
                    send_opt[1] = '\0';
                    send(sclient, send_opt, 1, 0);
               }
               else {
                    char send_opt[2] = { 0 };
                    send_opt[0] = opt + 12 + '0';             // send 14
                    send_opt[1] = '\0';
                    send(sclient, send_opt, 1, 0);
               }

               printf("                            |                                                                  |\n");
               printf("                            |Input the name : ");
               char user_name[16] = { 0 };
               scanf_s("%s", user_name, 16);
               send(sclient, user_name, 16, 0);

               recv(sclient, buffer, 50, 0);
               printf("                            |      %20s                                        |\n", buffer);
               printf("                            |                                                                  |\n");
               memset(buffer, 0, 50);
          }
          else if (opt == 0) {
               printf("                            |------------------------------------------------------------------|\n");
               break;
          }
          else {
               printf("                            |------------------------------------------------------------------|\n");
               printf("                            |                                                                  |\n");
               printf("                            |***************** operation number! ***********************|\n");
               printf("                            |                                                                  |\n");
          }
          printf("                            |------------------------------------------------------------------|\n");
     }
}

