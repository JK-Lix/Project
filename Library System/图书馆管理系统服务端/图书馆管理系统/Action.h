#pragma once
#ifndef ACTION_H
#define ACTION_H


void book(int sock_fd, int flag, int opt, char* username, int userflag);
//书籍类，用于对书籍文本文档进行操作

void record(int sock_fd, int opt, char *username);
//借阅记录类，用于对记录文本文档进行操作

void people(int sock_fd, int flag, int opt, int mod);
//用户和图书管理员类，用于人事调动，主要是系统管理员进行动作的类

void get_opt(int sock_fd);
//获取客户端操作数，循环处理用户操作请求

void get_path(int n, char* path);
//获取操作的文本文档路径

#endif //ACTION_H