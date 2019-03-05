//
//  Hailstone.h
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef Hailstone_h
#define Hailstone_h


/*
                    ---- {1}                   (若n=1)
                   |
                   |
 Hailstone(n) =     ---- {n} U Hailstone(n/2)  (若n为偶数)
                   |
                   |
                    ---- {n} U Hailstone(3n+1)  (若n为奇数)
 
 例如
 Hailstone(7) = {7,22,11,34,17,52,26,13,40,20,10,5,16,8,4,2,1}
 
 
 
 
 Hailstone(7)  = {7,  Hailstone(22)}
 Hailstone(22) = {22, Hailstone(11)}
 Hailstone(11) = {11, Hailstone(34)}
 Hailstone(34) = {34, Hailstone(17)}
 Hailstone(17) = {17, Hailstone(52)}
 Hailstone(52) = {52, Hailstone(26)}
 Hailstone(26) = {26, Hailstone(13)}
 Hailstone(13) = {13, Hailstone(40)}
 Hailstone(40) = {40, Hailstone(20)}
 Hailstone(20) = {20, Hailstone(10)}
 Hailstone(10) = {10, Hailstone(5)}
 Hailstone(5)  = {5,  Hailstone(16)}
 Hailstone(16) = {16, Hailstone(8)}
 Hailstone(8)  = {8,  Hailstone(4)}
 Hailstone(4)  = {4,  Hailstone(2)}
 Hailstone(2)  = {2,  Hailstone(1)}
 Hailstone(1)  = {1}
 */

template <typename T> struct Hailstone{ //函数对象：按照Hailstone规则转化一个T类对象
    
    virtual void operator()(T& e){ //假设T可直接做算术运算
        int step = 0;        //转换所需步骤
        while(1 != e){       //按奇偶逐步转换，直至为1
            ( e % 2) ? e = 3 * e + 1: e/=2;
            step++;
        }
        
        e = step;
    }
};



#endif /* Hailstone_h */
