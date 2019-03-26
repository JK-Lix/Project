
#include "pch.h"
#include "Action.h"



void own_borrow_info(int sclient, char *username) {
     printf("                            |------------------ check borrowing information -------------------|\n");
     char buffer[50] = { 0 };
     char send_opt[2] = { 0 };
     send_opt[0] = '4';            // send 4
     send_opt[1] = '\0';
     send(sclient, send_opt, 2, 0);
     send(sclient, username, 16, 0);
     recv(sclient, buffer, 50, 0);
     while (strcmp(buffer, "end") != 0) {
          printf("                            |      %-55s     |\n", buffer);
          memset(buffer, 0, 50);
          recv(sclient, buffer, 50, 0);
     }
     printf("                            |------------------------------------------------------------------|\n");
}

void borrow_info(int sclient) {
     printf("                            |------------------ check borrowing information -------------------|\n");
     while (1) {
          printf("                            |       Input:   1   for    check all information                  |\n");
          printf("                            |                2   for    seek information                       |\n");
          printf("                            |                                                                  |\n");
          printf("                            |                0   for    back to last view                      |\n");
          printf("                            |                                                                  |\n");
          printf("                            |Operation number: ");
          int opt = -1;
          scanf_s("%d", &opt);
          char buffer[50] = { 0 };
          if (opt == 1) {
               printf("                            |---------------------- check all information ---------------------|\n");
               printf("                            |                                                                  |\n");
               char send_opt[2] = { 0 };
               send_opt[1] = '\0';
               send_opt[0] = opt + 2 + '0';             // send 3
               send(sclient, send_opt, 1, 0);

               recv(sclient, buffer, 50, 0);
               while (strcmp(buffer, "end") != 0) {
                    printf("                            |      %-55s     |\n", buffer);
                    printf("                            |                                                                  |\n");
                    recv(sclient, buffer, 50, 0);
               }
               printf("                            |                                                                  |\n");
               memset(buffer, 0, 50);
          }
          else if (opt == 2) {
               printf("                            |------------------------- seek information -----------------------|\n");
               printf("                            |                                                                  |\n");
               char send_opt[2] = { 0 };            // send 4
               send_opt[0] = opt + 2 + '0';
               send_opt[1] = '\0';
               send(sclient, send_opt, 1, 0);
               printf("                            |Input the user's UserName : ");
               char username[16] = { 0 };
               scanf_s("%s", username, 16);
               printf("                            |                                                                  |\n");
               send(sclient, username, 16, 0);
               recv(sclient, buffer, 50, 0);
               while (strcmp(buffer, "end") != 0) {
                    printf("                            |      %-55s     |\n", buffer);
                    printf("                            |                                                                  |\n");
                    memset(buffer, 0, 50);
                    recv(sclient, buffer, 50, 0);
               }
               printf("                            |                                                                  |\n");
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
