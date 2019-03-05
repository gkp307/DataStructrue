//
//  Sum.h
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef Sum_h
#define Sum_h


//线性σ(n)
int sumI(int A[], int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += A[i];
    }
    return sum;
}//σ(1)+σ(n)*σ(1)+σ(1) = σ(n+2) = σ(n)


//递归
//1)线性递归
int sum(int A[],int n){
    if (1 > n) {
        return 0;
    }else{
        return sum(A, n-1) + A[n-1];
    }
}//σ(1)*递归深度 = σ(1)*(n+1) = σ(n)


//4)二分递归
int sum1(int A[],int lo,int hi){
    if (lo == hi) {
        return A[lo];
    }else{
        int mi = (lo + hi) >> 1;
        return sum1(A, lo, mi) + sum1(A, mi+1, hi);
    }
}//σ(hi-lo+1)


#endif /* Sum_h */
