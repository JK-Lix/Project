#pragma once
#ifndef SERVER_H
#define SERVER_H


int init_server();
//�˿ڡ�IP��ʼ�������������׽��֣���IP�Ͷ˿ڣ������˿�

int login(int sock_fd);
//�û���¼��������

void get_opt(int sock_fd);
//��ȡ�û�����ָ���

#endif //SERVER_H