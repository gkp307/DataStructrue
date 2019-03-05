//
//  Pow.h
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef Pow_h
#define Pow_h

//指数σ(2^n)
//幂运算2^n算法(蛮力迭代版)
int power2BF_I(int n){
    int pow = 1;
    while (0 < n--) {
        pow <<= 1;
    }
    return pow;
}//σ(n) = σ(2^r),r为输入指数n的比特位数

//实现幂函数的多项递归版本
//square---平方
inline int sqr(int a){
    return a * a;
}

int power2(int n){
    if (0 == n) {
        return 1;
    }
    return (n & 1) ? sqr(power2( n >> 1)) << 1 : sqr(power2( n >> 1));
}//σ(logn) = σ(r),r为输入指数n的比特位数

//2、幂运算
//递归版
int power2BF(int n){
    return ( 1> n) ? 1 : power2BF(n-1) << 1;
}//σ(n) = σ(2^r),r为输入指数n的比特位数

//迭代版
int power2_I(int n){
    int pow = 1;
    int p = 2;
    
    while (0 < n) {
        if (n & 1) {
            pow *= p;
        }
        
        n >>= 1;
        p *= p;
    }
    return pow;
}//σ(logn) = σ(r),r为输入指数n的比特位数

//推广到一般情况
int pow(int a,int n){
    int pow = 1;
    int p = a;
    while (0 < n) {
        if (n & 1) {
            pow *= p;
        }
        
        n >>= 1;
        p *= p;
    }
    return pow;
}

#endif /* Pow_h */
