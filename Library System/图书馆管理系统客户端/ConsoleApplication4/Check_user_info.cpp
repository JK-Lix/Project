
#include "pch.h"
#include "Action.h"


void ck_info(int sclient, int flag) {
     if (flag) printf("                            |----------------- check library administrator --------------------|\n");
     else      printf("                            |------------------- check users information ----------------------|\n");
     printf("                            |                                                                  |\n");
     while (1) {
          printf("                            |       Input:   1   for    all information                        |\n");
          printf("                            |                2   for    seek information                       |\n");
          printf("                            |                                                                  |\n");
          printf("                            |                0   for    back to last view                      |\n");
          printf("                            |                                                                  |\n");
          printf("                            |Operation number: ");
          int opt = -1;
          scanf_s("%d", &opt);
          char buffer[50] = { 0 };
          if (opt == 1) {
               printf("                            |------------------ check all information -------------------------|\n");
               printf("                            |                                                                  |\n");
               if (flag) {
                    char send_opt[2] = { 0 };
                    send_opt[0] = opt + 6 + '0';            // send 7
                    send_opt[1] = '\0';
                    send(sclient, send_opt, 2, 0);
               }
               else {
                    char send_opt[2] = { 0 };              // send 9
                    send_opt[1] = '\0';
                    send_opt[0] = opt + 8 + '0';
                    send(sclient, send_opt, 2, 0);
               }
               printf("                            |                          name password                           |\n");

               recv(sclient, buffer, 50, 0);
               while (strncmp(buffer, "end", 4) != 0) {
                    printf("                            |                                                                  |\n");
                    printf("                            |                          %-40s|\n", buffer);
                    memset(buffer, 0, 50);
                    recv(sclient, buffer, 50, 0);
               }
               printf("                            |                                                                  |\n");
               memset(buffer, 0, 50);
          }
          else if (opt == 2) {
               printf("                            |-------------------------- seek information ----------------------|\n");
               printf("                            |                                                                  |\n");
               if (flag) {
                    char send_opt[2] = { 0 };
                    send_opt[0] = opt + 6 + '0';
                    send(sclient, send_opt, 2, 0);            // send 8
                    send_opt[1] = '\0';
               }
               else {
                    char send_opt[2] = { 0 };
                    send_opt[0] = opt + 8 + '0';           // send 10
                    send_opt[1] = '\0';
                    send(sclient, send_opt, 1, 0);
               }
               printf("                            |Input the UserName : ");
               char user_name[16] = { 0 };
               scanf_s("%s", user_name, 16);
               send(sclient, user_name, 16, 0);
               recv(sclient, buffer, 50, 0);
               printf("                            |                name password                                     |\n");
               while (strcmp(buffer, "end") != 0) {
                    printf("                            |                                                                  |\n");
                    printf("                            |                %-45s     |\n", buffer);
                    memset(buffer, 0, 50);
                    recv(sclient, buffer, 50, 0);
               }
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
               printf("                            |*****************  Wrong operation number! ***********************|\n");
               printf("                            |                                                                  |\n");
          }
          printf("                            |------------------------------------------------------------------|\n");
     }
}