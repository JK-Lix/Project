/*************************************************************************
	> File Name: DIYOS.h
	> Author: Lix
	> Mail: 776579471@qq.com
	> Created Time: 2019年01月20日 星期日 19时02分17秒
 ************************************************************************/

#ifndef _DIYOS_H
#define _DIYOS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <grp.h>
#include <signal.h>

#define max 1024
#define MAXROWLEN 80

void dateStyle(char*);
void showOFF(struct stat,char*, int);
void beforeShowOff(char*, int);
void LS(DIR*, struct dirent, int);
void orderLs(char*);
void showHead();
void getInput(char*);
void cmdOperation(char*);
int inputForm();
void Ctrl_C(int);

int getMonth(char *mon, int pmon){
    if (strcmp(mon, "Jan") == 0) pmon = 1;
    if (strcmp(mon, "Feb") == 0) pmon = 2;
    if (strcmp(mon, "Mar") == 0) pmon = 3;
    if (strcmp(mon, "Apr") == 0) pmon = 4;
    if (strcmp(mon, "May") == 0) pmon = 5;
    if (strcmp(mon, "Jun") == 0) pmon = 6;
    if (strcmp(mon, "Jul") == 0) pmon = 7;
    if (strcmp(mon, "Aug") == 0) pmon = 8;
    if (strcmp(mon, "Sep") == 0) pmon = 9;
    if (strcmp(mon, "Oct") == 0) pmon = 10;
    if (strcmp(mon, "Nov") == 0) pmon = 11;
    if (strcmp(mon, "Dec") == 0) pmon = 12;
    return pmon;
}

void printFirstLine(mode_t st_mode) { 
    if (S_ISLNK(st_mode)) printf("l");
    else if (S_ISREG(st_mode)) printf("-");
    else if (S_ISDIR(st_mode)) printf("d");
    else if (S_ISCHR(st_mode)) printf("c");
    else if (S_ISBLK(st_mode)) printf("b");
    else if (S_ISFIFO(st_mode)) printf("f");
    else if (S_ISSOCK(st_mode)) printf("s");

    if (st_mode & S_IRUSR) printf("r");
    else printf("-");
    if (st_mode & S_IWUSR) printf("w");
    else printf("-");
    if (st_mode & S_IXUSR) printf("x");
    else printf("-");

    if (st_mode & S_IRGRP) printf("r");
    else printf("-");
    if (st_mode & S_IWGRP) printf("w");
    else printf("-");
    if (st_mode & S_IXGRP) printf("x");
    else printf("-");

    if (st_mode & S_IROTH) printf("r");
    else printf("-");
    if (st_mode & S_IWOTH) printf("w");
    else printf("-");
    if (st_mode & S_IXOTH) printf("x");
    else printf("-");
    printf(" ");
}
#endif
