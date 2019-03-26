


#include "pch.h"
#include "Action.h"


//获取指定的文件路径

void get_path(int n,char* path) {
     switch (n) {
     case 1:
          strcpy_s(path, 12, "admin.txt");
          break;
     case 2:
          strcpy_s(path, 12, "book.txt");
          break;
     case 3:
          strcpy_s(path, 12, "login.txt");
          break;
     case 4:
          strcpy_s(path, 12, "record.txt");
          break;
     case 5:
          strcpy_s(path, 12, "superadmin.txt");
          break;
     case 6:
          strcpy_s(path, 12, "user.txt");
          break;
     }
}
