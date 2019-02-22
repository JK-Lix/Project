/*************************************************************************
	> File Name: homework3.c
	> Author: JOKER
	> Mail: 
	> Created Time: 2019年01月14日 星期一 10时30分31秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include"Lix.h"

int ck_us_hs(char *env[]) {
    char **p;
    p = env;
    p += 17;
    char hostname[20];
    gethostname(hostname, sizeof(hostname));
    if (strcmp(p[0], user)) {
        printf("Wrong User\n");
        return 0;
    }
    if (strcmp(hostname, host)) {
        printf("Wrong host\n");
        return 0;
    }
    return 1;
}

int ck_pwd(char *argv[]) {
    setpwd();
    change2(argv);
    return strcmp(pwd, argv[1]);
}

int judge_pm(int argc) {
    if (argc < 2) {
        printf("You should input your password after command\n");
        return 0;
    }
    if (argc > 2) {
        printf("Illegal input\n");
        return 0;
    }
    return 1; 
}

int change_pwd() {
    char check1[6];
    printf("Input your password(no more than 6 characters):");
    scanf("%s",check1);
    printf("%s", check1);
    while(1){
        if (strlen(check1) > 6) printf("\npassword must be no more than 6 characters\n Input:");  
        else if (!strlen(check1)) printf("\npassword cannot be empty");
        else break;
        scanf("%s", check1);
    }
    FILE *w = fopen("homework3pwd", "w");
    assert(w != NULL);
    fscanf(w, "%s", check1);
    change1(check1);
    fprintf(w, "%s", check1);
    fclose(w);
    printf("\nChange Successfully! And the Program will end.\n");
    return 0;
}

void Welcome(){
    printf("////////////////////////Welcome//////////////////////////\n");
    printf("//          Input 1 to Change the Password             //\n");
    printf("//          Input 2 tp Output the Consequence          //\n");
    printf("//          Input others to close the Program          //\n");   
    printf("/////////////////////////////////////////////////////////\n");
}

int main(int argc, char *argv[], char *env[]) {
    if (!judge_pm(argc)) return 0;
    if (!ck_us_hs(env)) {
        return 0;
    }
    if (ck_pwd(argv)) {
        printf("Wrong Password!\n");
        return 0;
    }
    Welcome();
    int n = 0;
    scanf("%d", &n);
    if (n == 1) return change_pwd();
    else if (n == 2) {
        printf("/////////////////////////////////////////////////////////\n");
        printf("//          There is nothing to show you!              //\n");
        printf("//          Haha!                                      //\n");
        printf("/////////////////////////////////////////////////////////\n");
        return 0;
    }
    return 0;
}
