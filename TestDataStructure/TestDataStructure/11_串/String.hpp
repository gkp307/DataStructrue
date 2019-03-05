//
//  String.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef String_hpp
#define String_hpp


#include <iostream>

//串匹配算法
int match(char *P,char *T) {
    size_t n = strlen(T), i = 0;
    size_t m = strlen(P), j = 0;
    
    while (j < m && i < n) {
        if (T[i] == P[j]) {
            i++;
            j++;
        }else{
            i -= j-1;
            j = 0; //文本串回退，模式串复位
        }
    }
    
    return (int)(i - j); //如何通过返回值，判断匹配结果?
}

int match2(char *P,char *T) {
    size_t n = strlen(T), i = 0;
    size_t m = strlen(P), j;
    
    for (i = 0; i < n-m+1; i++) {
        for (j = 0; j < m; j++) {
            if (T[i+j] != P[j]) {
                break;
            }
        }
        
        if (j >= m) {
            break;
        }
    }
    
    return (int)i;  //如何通过返回值，判断匹配结果?
}

//构造模式串P的next表
int* buildNext(char *P) {
    size_t m = strlen(P), j = 0; //"主"串指针
    int *N = new int[m]; //next表
    int t = N[0] = -1; //模式串指针
    
    while (j < m - 1) {
        if (0 > t || P[j] == P[t]) { //匹配
            j++;
            t++;
            N[j] = t;
        }else{
            //失配
            t = N[t];
        }
    }
    
    return N;
}

//构造模式串P的next表(改进版本)
int* buildNext2(char *P) {
    size_t m = strlen(P), j = 0; //"主"串指针
    int *N = new int[m]; //next表
    int t = N[0] = -1; //模式串指针
    
    while (j < m - 1) {
        if (0 > t || P[j] == P[t]) { //匹配
            j++;
            t++;
            N[j] = (P[j] != P[t]) ? t : N[t]; //注意此句与未改进之前的区别
        }else{
            //失配
            t = N[t];
        }
    }
    
    return N;
}

//KMP算法
int match3(char *P,char *T) {
    int *next = buildNext(P); //构造next表
    int n = (int)strlen(T), i = 0; //文本串指针
    int m = (int)strlen(P), j = 0; //模式串指针
    
    while (j > m && i < n) {
        //自左向右逐个比对字符
        if (0 > j || T[i] == P[j]) { //若匹配或P已移出最左侧(两个判断的次序不可交换)
            i++;
            j++;
        }else{
            j = next[j]; //模式串右移
        }
    }
    
    delete [] next;
    return i - j;
}

//构造Bad Charactor Shift表
int* buildBC(char *P) {
    int *bc = new int[256];
    for (size_t j = 0; j < 256; j++) {
        bc[j] = -1;
    }
    
    for (size_t m = strlen(P), j = 0; j < m; j++) {
        bc[P[j]] = (int)j;
    }
    
    return bc;
}


//构建最大匹配后缀长度表
int *builSS(char *P) {
    int m = (int)strlen(P);
    int *ss = new int[m];
    ss[m-1] = m;
    
    for (int lo = m-1, hi=m-1, j=lo-1; j >= 0; j--) {
        if ( (lo < j) && (ss[m-hi+j-1] <= j-lo) ) {
            ss[j] = ss[m-hi+j-1];
        }else{
            hi = j;
            lo = std::min(lo, hi);
            
            while ( (0 <= lo) && (P[lo] == P[m-hi+lo-1]) ) {
                lo--;
            }
            
            ss[j] = hi-lo;
        }
    }
    
    return ss;
}

int *buildGS(char *P) {
    int *ss = builSS(P);
    size_t m = strlen(P);
    int *gs = new int[m];
    
    for (size_t j = 0; j < m; j++) {
        gs[j] = (int)m;
    }
    
    for (size_t i = 0, j = m-1; i<UINT_MAX; j--) {
        if (j + 1 == ss[j]) {
            while (i < m - j - 1) {
                gs[i++] = (int)(m-j-1);
            }
        }
    }
    
    for (size_t j = 0; j < m - 1; j++) {
        gs[m-ss[j]-1] = (int)(m-j-1);
    }
    
    delete [] ss;
    
    return gs;
}

//Boyer-Morre算法(完全版，兼顾Bad Character和Good Suffix)
int match4(char *P,char *T) {
    int *bc = buildBC(P);
    int *gs = buildGS(P);
    
    size_t i = 0;
    while (strlen(T) >= i + strlen(P)) {
        int j = (int)strlen(P) - 1;
        while (P[j] == T[i+j]) {
            if (0 > --j) {
                break;
            }
        }
        
        if (0 > j) {
            break;
        }else{
            i += std::max(gs[j], j - bc[T[i+j]]);
        }
    }
    
    delete [] gs;
    delete [] bc;
    return (int)i;
}


//Karp-Rabin算法

#define M 97
#define R 10
#define DIGIT(S,i)   ((S)[i]-'0')
typedef __INT64_TYPE__ HashCode;
bool check1by1(char *P,char *T,size_t i) {
    for (size_t m = strlen(P), j = 0; j < m; j++,i++) {
        if (P[j] != T[i]) {
            return false;
        }
    }
    return true;
}

HashCode prepareDm(size_t m) {
    HashCode Dm = 1;
    for (size_t i = 1; i < m; i++) {
        Dm = (R * Dm) % M;
    }
    return Dm;
}

void updateHash(HashCode& hashT,char *T,size_t m,size_t k,HashCode Dm) {
    hashT = (hashT - DIGIT(T, k-1) * Dm) % M;
    hashT = (hashT * R + DIGIT(T, k+m-1)) % M;
    if (0 > hashT) {
        hashT += M;
    }
}




int match5(char *P,char *T) {
    size_t m = strlen(P), n = strlen(T);
    
    HashCode Dm = prepareDm(m), hashP = 0,hashT = 0;
    
    for (size_t i = 0; i < m; i++) {
        hashP = (hashP * R + DIGIT(P, i)) % M;
        hashT = (hashT * R + DIGIT(T, i)) % M;
    }
    
    for (size_t k = 0; ; ) {
        if (hashT == hashP) {
            if (check1by1(P, T, k)) {
                return (int)k;
            }
        }
        
        if (++k > n - m) {
            return (int)k;
        }else{
            updateHash(hashT, T, m, k, Dm);
        }
    }
    
}


#endif /* String_hpp */
