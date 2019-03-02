/*************************************************************************
	> File Name: common.h
	> Author: Lix
	> Mail: 776579471@qq.com
	> Created Time: 2019年02月23日 星期六 11时03分41秒
 ************************************************************************/
#include "head.h"
#ifndef _COMMON_H
#define _COMMON_H

int get_conf_value(char* pathname, char* key_name, char* value);

int socket_create(int port, int num);

int socket_connect(int port, char *host);

#endif
