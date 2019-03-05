//
//  Reverse.h
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef Reverse_h
#define Reverse_h

//2)递归模式
void reverse(int *,int,int);
void reverse(int *A, int n){  //数组倒置
    reverse(A,0,n-1);
}

//尾递归
//在线性递归算法中，若递归调用在递归实例中恰好以最后一步操作的形式出现，则称作尾递归。
void reverse(int *A,int lo,int hi){
    if (lo < hi) {
        swap(A[lo], A[hi]);
        reverse(A, lo + 1, hi - 1);
    }
}//σ(hi-lo+1)

//3)递归消除
//尾递归及其消除
void reverse1(int *A,int lo,int hi){
    while (lo < hi) {
        swap(A[lo++], A[hi--]);
    }
}//σ(hi-lo+1)


#endif /* Reverse_h */
