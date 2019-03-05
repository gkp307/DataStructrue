//
//  Fabnacci.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef Fabnacci_hpp
#define Fabnacci_hpp

//Fibonacci数:二分递归
int fib(int n);

//Fibonacci数:线性递归
int fib1(int n, int &prev);

//Fibonacci数:迭代
int fib2(int n);


class Fib{
public:
    Fib(int n) {
        f = 1;
        g = 0;
        while (g < n) {
            next();
        }
    }
    
    int get() {
        return g;
    }
    
    int next() {
        g += f;
        f = g - f;
        return g;
    }
    
    int prev() {
        f = g - f;
        g -= f;
        return g;
    }
    
private:
    int f,g;
};

#endif /* Fabnacci_hpp */
