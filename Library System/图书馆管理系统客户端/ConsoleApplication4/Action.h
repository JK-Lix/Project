#pragma once
#ifndef CHANGE_INFO_H
#define CHANGE_INFO_H

// TODO: 添加要在此处预编译的标头
#include "pch.h"

void s_change_info(int sclient, int flag);

void bk_info(int sclient, int flag);

void ck_info(int sclient, int flag);

void change_info(int sclient);

void own_borrow_info(int sclient, char *username);

void borrow_info(int sclient);

void libadm(int sclient, char* username);

void users(int sclient, char* username);

void sysadm(int sclient, char* username);
#endif //CHANGE_INFO_H
