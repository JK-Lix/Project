#pragma once
#ifndef ACTION_H
#define ACTION_H


void book(int sock_fd, int flag, int opt, char* username, int userflag);
//�鼮�࣬���ڶ��鼮�ı��ĵ����в���

void record(int sock_fd, int opt, char *username);
//���ļ�¼�࣬���ڶԼ�¼�ı��ĵ����в���

void people(int sock_fd, int flag, int opt, int mod);
//�û���ͼ�����Ա�࣬�������µ�������Ҫ��ϵͳ����Ա���ж�������

void get_opt(int sock_fd);
//��ȡ�ͻ��˲�������ѭ�������û���������

void get_path(int n, char* path);
//��ȡ�������ı��ĵ�·��

#endif //ACTION_H