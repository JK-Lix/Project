/*************************************************************************
	> File Name: OJhead.h
	> Author: 
	> Mail: 
	> Created Time: 2019年01月15日 星期二 16时07分20秒
 ************************************************************************/
#ifndef _OJHEAD_H
#define _OJHEAD_H

#include<stdlib.h>

typedef struct {
    int total_num;
    int bingo_num;
} TestInfo;

typedef struct funcs{
    void (*pfunc)();
    struct funcs *next;
}TestFuncs;

TestInfo ti;
TestFuncs *temp;
TestFuncs *head;
//全局定义的链表结构开辟空间的时候无法赋值
//（或者是在attribute执行的函数之后）
int times = 0;

#define TF(a, b) {\
            float change = 0.0;\
            float bi = ti.bingo_num, to =ti.total_num; \
            change = bi / to * 100.0;\
            if (change != 100) printf("\33[1m\033[35mComplete:[ \33[1m\033[31m%0.1lf%%\33[1m\033[35m ], Total: %d, Expect: %d\33[0m\n",change, ti.total_num, ti.bingo_num);\
            else printf("\33[1m\033[35mComplete:[ \33[1m\033[32m%0.1lf%%\33[1m\033[35m ], Total: %d, Expect: %d\33[0m\n",change, ti.total_num, ti.bingo_num);\
        }

#define EXPECT(a, b){\
            ti.total_num += 1;\
            if(a == b) {\
                ti.bingo_num += 1;\
                printf("%s = %d ---- \033[1m\033[32mTrue\033[0m\n",#a, b);\
           }else printf("%s = %d ---- \033[1m\033[31mFalse\033[0m\n",#a,b);\
        }

#define _FUNC_NAME(a, b) a##b
#define FUNC_NAME(a, b) _FUNC_NAME(a, b)
#define TEST(a, b)\
        void FUNC_NAME(test,b)();\
        void FUNC_NAME(start_, b)();\
        __attribute__((constructor)) void FUNC_NAME(getfunc_, b)(){\
            if(times == 0) {\
                temp = (TestFuncs *)malloc(sizeof(TestFuncs));\
                head = temp;\
            }\
            TestFuncs *new_node = (TestFuncs *)malloc(sizeof(TestFuncs));\
            new_node->pfunc = FUNC_NAME(start_,b);\
            temp->next = new_node;\
            temp = new_node;\
            times++;\
        }\
        void FUNC_NAME(start_,b)() {\
            printf("\n\33[1m\033[35m[This is \033[34m%s\033[35m test]\033[0m\n", #b);\
            FUNC_NAME(test, b) ();\
            return;\
        }\
        void FUNC_NAME(test, b)() 

int RUN_ALL_TEST() {
    head = head->next;
    for (int i = 0; i < times; i++) {
        ti.total_num = 0;
        ti.bingo_num = 0;
        head->pfunc();
        head = head->next;
        TF(a, b)
    }
    return 0;
}
#endif
