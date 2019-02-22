/*************************************************************************
	> File Name: OJ.cpp
	> Author: 
	> Mail: 
	> Created Time: 2019年01月15日 星期二 16时06分31秒
 ************************************************************************/
#include <stdio.h>
#include "OJhead.h"

int multiple(int a, int b) {
    return a * b;
}

int add(int a, int b) {
    return a + b;
}

int is_prime(int x) {
    if (x <= 1) return 0;
    for (int i = 2; i * i <= x; i++) {
        if (x % i == 0) return 0;
    }
    return 1;
}

TEST(test, prime_func) {
    EXPECT(is_prime(2), 0);
    EXPECT(is_prime(-2), 0);
    EXPECT(is_prime(15), 0) ;
    EXPECT(is_prime(9973), 1);
}

TEST(test, add_func) {
    EXPECT(add(1, 2), 3);
    EXPECT(add(3, 4), 7);
    EXPECT(add(999, 1), 1000);
}

TEST(test, multiple_func) {
    EXPECT(multiple(2, 3), 6);
    EXPECT(multiple(3, 4), 12);
    EXPECT(multiple(3, 5), 13);
    EXPECT(multiple(1, 6), 5);
}

int main() {
    
    return RUN_ALL_TEST();
}
