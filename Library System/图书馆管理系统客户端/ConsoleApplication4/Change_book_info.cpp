
#include "pch.h"
#include "Action.h"


void change_info(int sclient) {
     printf("                            |------------------- change book's information --------------------|\n");
     while (1) {
          printf("                            |       Input:   1   for    add a new book                         |\n");
          printf("                            |                2   for    delete a book                          |\n");
          printf("                            |                                                                  |\n");
          printf("                            |                0   for    back to last view                      |\n");
          printf("                            |                                                                  |\n");
          printf("                            |Operation number: ");
          int opt = -1;
          scanf_s("%d", &opt);
          char buffer[50] = { 0 };
          if (opt == 1) {
               printf("                            |------------------------ add a new book --------------------------|\n");
               char send_opt[2] = { 0 };
               send_opt[0] = opt + 4 + '0';            // send 5
               send_opt[1] = '\0';
               send(sclient, send_opt, 2, 0);
               printf("                            |                                                                  |\n");
               printf("                            |Input the book's name : ");
               char book_name[50] = { 0 };
               char type[20] = { 0 };
               scanf_s("%s", book_name, 50);
               send(sclient, book_name, 50, 0);
               printf("                            |                                                                  |\n");
               printf("                            |Input the book's type : ");
               scanf_s("%s", type, 20);
               send(sclient, type, 20, 0);
               recv(sclient, buffer, 50, 0);
               printf("                            |                                                                  |\n");
               printf("                            |---- add A Book -- %-47s|\n", buffer);
               printf("                            |                                                                  |\n");
               printf("                            |------------------------------------------------------------------|\n");
               printf("                            |                                                                  |\n");
               memset(buffer, 0, 50);
          }
          else if (opt == 2) {
               printf("                            |------------------------ delete a book ---------------------------|\n");
               printf("                            |                                                                  |\n");
               char send_opt[2];
               send_opt[0] = opt + 4 + '0';            // send 6
               send_opt[1] = '\0';
               send(sclient, send_opt, 2, 0);
               printf("                            |Input the book's name : ");
               char book_name[50] = { 0 };
               scanf_s("%s", book_name, 50);
               send(sclient, book_name, 50, 0);
               recv(sclient, buffer, 50, 0);
               printf("                            |                                                                  |\n");
               printf("                            |---- Delete A Book -- %-44s|\n", buffer);
               printf("                            |                                                                  |\n");
               memset(buffer, 0, 50);
               printf("                            |------------------------------------------------------------------|\n");
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