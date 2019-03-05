//
//  Hano.h
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef Hano_h
#define Hano_h

#include "Stack.hpp"

class Disk{
    
};

void move(Stack<Disk> &,Stack<Disk> &){
    
}

//将柱子sx上的n只盘子，借助柱子sy中转，移到柱子sz上
void hanoi(int n, Stack<Disk> &sx,Stack<Disk> &sy,Stack<Disk> &sz){
    if (n > 0) {
        hanoi(n-1, sx, sz, sy); //递归:将sx上的n-1只盘子，借助sz中转，移到sy上
        move(sx, sz);           //直接:将sx上最后一个盘子，移到sz上
        hanoi(n-1, sy, sx, sz); //递归:将sy上的n-1只盘子，借助sx中转，移到sz上
    }
}//σ(2^n)


#endif /* Hano_h */
