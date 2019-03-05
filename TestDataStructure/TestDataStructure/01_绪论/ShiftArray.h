//
//  ShiftArray.h
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef ShiftArray_h
#define ShiftArray_h

#include "Reverse.h"

//将数组A[0,n)中的元素整体循环左移k位
int shift2(int *A,int n,int k) { //借助倒置算法，将数组循环左移k位
    
    k %= n; //确保k<=n
    reverse(A, k); //将区间A[0,k)倒置
    reverse(A+k, n-k); //将区间A[k,n)倒置
    reverse(A, n); //倒置整个数组A[0,n)
    
    return 3*n; //返回累计操作次数
}


#endif /* ShiftArray_h */
