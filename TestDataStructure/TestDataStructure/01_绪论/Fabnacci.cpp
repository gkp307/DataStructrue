//
//  Fabnacci.cpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#include "Fabnacci.hpp"


//Fibonacci数:二分递归
int fib(int n){
    return (2 > n) ? n : fib(n-1)+fib(n-2);
}//σ(2^n)

//Fibonacci数:线性递归
int fib1(int n, int &prev){
    if (0 == n) {
        prev = 1;
        return 0;
    }else{
        int prevPrev;
        prev = fib1(n-1, prevPrev);
        return prevPrev + prev;
    }
}//σ(n)

//Fibonacci数:迭代
int fib2(int n){
    int f = 1,g = 0;
    while (n--) {
        g += f;
        f = g - f;
    }
    return g;
}



