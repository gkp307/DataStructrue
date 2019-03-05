//
//  Max.h
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef Max_h
#define Max_h

#include <iostream>

int maxI(int A[],int n){
    int m = INT_MIN;
    for (int i = 0; i < n; i++) {
        m = max(m, A[i]);
    }
    return m;
}//σ(1)+σ(n)*σ(1) = σ(n+2) = σ(n)

int maxR(int A[],int n){
    if (2 > n) {
        return A[n-1];
    }else{
        return max(maxR(A, n-1), A[n-1]);
    }
}//σ(1)*递归深度 = σ(1)*(n+1) = σ(n)

int maxR(int A[],int lo,int hi){ //[lo, hi)
    if (lo + 1 == hi) {
        return A[lo];
    }else{
        int mi = (lo + hi) >> 1;
        return max(maxR(A, lo, mi), maxR(A, mi, hi));
    }
}//σ(hi-lo)


#endif /* Max_h */
