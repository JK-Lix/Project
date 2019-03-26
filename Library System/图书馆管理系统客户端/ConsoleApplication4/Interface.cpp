



#include "pch.h"
#include "Action.h"


void libadm(int sclient, char* username) {
     printf("                            |          Welcome Library Administrator %-26s|\n", username);
     while (1) {
          printf("                            |                                                                  |\n");
          printf("                            |       Input:   1   for    check books' information               |\n");
          printf("                            |                2   for    change book's information              |\n");
          printf("                            |                3   for    check borrowing information            |\n");
          printf("                            |                                                                  |\n");
          printf("                            |                0   for    exit                                   |\n");
          printf("                            |                                                                  |\n");
          printf("                            |Operation Number: ");
          int opt = -1;
          scanf_s("%d", &opt);
          while (opt == -1) {
               printf("                            |Operation Number: ");
          }
          printf("                            |------------------------------------------------------------------|\n");
          switch (opt) {
          case 1:
               bk_info(sclient, 0);
               break;
          case 2:
               change_info(sclient);
               break;
          case 3:
               borrow_info(sclient);
               break;
          case 0:
               printf("                            |---------------------- Thank you for using -----------------------|\n");
               break;
          default:
               printf("                            |------------------------------------------------------------------|\n");
               printf("                            |                                                                  |\n");
               printf("                            |*****************  Wrong operation number! ***********************|\n");
               printf("                            |                                                                  |\n");
               printf("                            |------------------------------------------------------------------|\n");

          }
          if (opt == 0) break;
     }
}

void users(int sclient, char* username) {
     printf("                            |                     Welcome Users  %-30s|\n", username);
     while (1) {
          printf("                            |                                                                  |\n");
          printf("                            |       Input:   1   for    seek books' information                |\n");
          printf("                            |                2   for    check your borrowing information       |\n");
          printf("                            |                                                                  |\n");
          printf("                            |                0   for    exit                                   |\n");
          printf("                            |                                                                  |\n");
          printf("                            |Operation: ");

          int opt = -1;
          scanf_s("%d", &opt);
          while (opt != 0 && opt != 2 && opt != 1) {
               printf("              |Operation Number: ");
          }
          printf("                            |------------------------------------------------------------------|\n");
          switch (opt) {
          case 1:
               bk_info(sclient, 1);
               break;
          case 2:
               own_borrow_info(sclient, username);
               break;
          case 0:
               printf("                            |---------------------- Thank you for using -----------------------|\n");
               break;
          default:
               printf("                            |------------------------------------------------------------------|\n");
               printf("                            |                                                                  |\n");
               printf("                            |*****************  Wrong operation number! ***********************|\n");
               printf("                            |                                                                  |\n");
               printf("                            |------------------------------------------------------------------|\n");
          }
          if (opt == 0) break;
     }
}

void sysadm(int sclient, char* username) {
     printf("                            |           Welcome System Administrator %16s          |\n", username);
     while (1) {
          printf("                            |                                                                  |\n");
          printf("                            |       Input:   1   for    check library administrator            |\n");
          printf("                            |                2   for    check users information                |\n");
          printf("                            |                3   for    add/delete a user                      |\n");
          printf("                            |                4   for    add/delete a library administrator     |\n");
          printf("                            |                                                                  |\n");
          printf("                            |                0   for    exit                                   |\n");
          printf("                            |                                                                  |\n");
          printf("                            |Operation Number: ");

          int opt = -1;
          scanf_s("%d", &opt);
          while (opt == -1) {
               printf("|Operation: ");
          }
          printf("                            |------------------------------------------------------------------|\n");
          switch (opt) {
          case 1:
               ck_info(sclient, 1);
               break;
          case 2:
               ck_info(sclient, 0);
               break;
          case 3:
               s_change_info(sclient, 0);
               break;
          case 4:
               s_change_info(sclient, 1);
               break;
          case 0:
               printf("                            |---------------------- Thank you for using -----------------------|\n");
               break;
          default:
               printf("                            |------------------------------------------------------------------|\n");
               printf("                            |                                                                  |\n");
               printf("                            |*****************  Wrong operation number! ***********************|\n");
               printf("                            |                                                                  |\n");
               printf("                            |------------------------------------------------------------------|\n");
          }
          if (opt == 0) break;
     }
}