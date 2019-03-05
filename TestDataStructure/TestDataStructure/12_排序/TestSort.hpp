//
//  TestSort.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/8/23.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef TestSort_hpp
#define TestSort_hpp

#include "Vector.hpp"


//验证候选者是否的确为众数
template <typename T> bool majEleCheck(Vector<T> A,T maj) {
    int occurrence = 0;  //maj在A[]中出现的次数
    for (int i = 0; i < A.size(); i++) {
        if (A[i] == maj) {
            occurrence++;
        }
    }
    return 2 * occurrence > A.size(); //根据最终的计数值，即可判断是否的确当选
}

//选出具备必要条件的众数候选者
template <typename T> T majEleCandidate(Vector<T> A) {
    T maj;
    
    for (int c = 0, i = 0; i < A.size(); i++) {
        if (0 == c) { //每当c归0，都意味着此时的前缀P可以剪除
            maj = A[i];
            c = 1;//众数候选者改为新的当前元素
        }else{  //否则
            maj == A[i] ? c++ : c--;  //相应的更新差额计数器
        }
    }
    return maj; //至此，原向量的众数若存在，则只能是maj
}

//众数查找算法: T可比较可判等
template <typename T> bool majority(Vector<T> A,T &maj) {
    maj = majEleCandidate(A);  //必要性:选出候选者maj
    return majEleCheck(A, maj); //充分性:验证maj是否的确当选
}

//中位算法蛮力版
template <typename T>
T trivialMedian(Vector<T>& S1,int lo1,int n1,Vector<T>& S2,int lo2,int n2) {
    int hi1 = lo1 + n1, hi2 = lo2 + n2;
    Vector<T> S;
    
    while ( (lo1 < hi1) && (lo2 < n2)) {
        while ( (lo1 < hi1) && (S1[lo1] <= S2[lo2])) {
            S.insert(S1[lo1++]);
        }
        
        while ( (lo2 < hi2) && (S2[lo2] <= S1[lo1])) {
            S.insert(S2[lo2++]);
        }
    }
    
    
    while (lo1 < hi1) {
        S.insert(S1[lo1++]);
    }
    
    while (lo2 < hi2) {
        S.insert(S1[lo2++]);
    }
    
    return S[(n1+n2)/2];
}

template <typename T>
T median(Vector<T>& S1,int lo1,Vector<T>& S2,int lo2,int n) {
    
    if (n < 3) {
        trivialMedian(S1, lo1, n, S2, lo2, n);
    }
    
    int mi1 = lo1 + n/2,mi2 = lo2 + (n-2)/2;
    
    if (S1[mi1] < S2[mi2]) {
        return median(S1, mi1, S2, lo2, n);
    }else if (S1[mi1] > S2[mi2]) {
        return median(S1, lo1, S2, mi2, n+lo2-mi2);
    }else{
        return S1[mi1];
    }
    
}


template <typename T>
T median(Vector<T>& S1,int lo1,int n1,Vector<T>& S2,int lo2,int n2) {
    if (n1 > n2) {
        return median(S2, lo2, n2, S1, lo1, n1);
    }
    
    if (n2 < 6) {
        return trivialMedian(S1, lo1, n1, S2, lo2, n2);
    }
    
    if (2 * n1 < n2) {
        return median(S1, lo1, n1, S2, lo2+(n2-n1-1)/2, n1+2-(n2-n1)%2);
    }
    
    int mi1 = lo1 + n1/2;
    int mi2a = lo2 + (n1-1)/2;
    int mi2b = lo2 + n2-1-n1/2;
    
    if (S1[mi1] > S2[mi2b]) {
        return median(S1, lo1, n1/2+1, S2, mi2a, n2-(n1-1)/2);
    }else if (S1[mi1] < S2[mi2a]) {
        return median(S1, mi1, (n1+1)/2, S2, lo2, n2-n1/2);
    }else{
        return median(S1, lo1, n1, S2, mi2a, n2-(n1-1)/2*2);
    }
    
    
}

template <typename T> void quickSelect(Vector<T>& A,Rank k) {
    for (Rank lo = 0,hi = A.size()-1; lo < hi; ) {
        Rank i = lo, j = hi;
        T pivot = A[lo];
        
        while (i < j) {
            while ( (i < j) && (pivot <= A[j])) {
                j--;
            }
            
            A[i] = A[j];
            
            
            while ( (i < j) && (A[i] <= pivot)) {
                i++;
            }
            
            A[j] = A[i];
            
        }
        
        A[i] = pivot;
        
        if (k <= i) {
            hi = i-1;
        }
        
        if (i <= k) {
            lo = i + 1;
        }
        
    }
}




#endif /* TestSort_hpp */
