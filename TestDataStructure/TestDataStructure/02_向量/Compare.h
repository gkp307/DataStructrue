//
//  Compare.h
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef Compare_h
#define Compare_h

#include "Vector.hpp"

//随机置乱向量
template <typename T> void permute(Vector<T> &V) {
    for (int i = V.size(); i > 0; i--) {
        std::swap(V[i-1], V[rand()%i]);  //V[i-1]与V[0,i)中某一随机元素交换
    }
}

template <typename T> static bool lt(T *a,T *b) {
    return lt(*a, *b);
}

template <typename T> static bool lt(T &a,T &b) {
    return a < b;
}

template <typename T> static bool eq(T *a,T *b) {
    return eq(*a, *b);
}

template <typename T> static bool eq(T &a,T &b) {
    return a == b;
}

#endif /* Compare_h */
