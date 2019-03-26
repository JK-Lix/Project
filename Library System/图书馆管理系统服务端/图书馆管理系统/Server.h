#pragma once
#ifndef SERVER_H
#define SERVER_H


int init_server();
//端口、IP初始化，创建网络套接字，绑定IP和端口，监听端口

int login(int sock_fd);
//用户登录操作函数

void get_opt(int sock_fd);
//获取用户操作指令函数

#endif //SERVER_H