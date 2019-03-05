//
//  QuadlistNode.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef QuadlistNode_hpp
#define QuadlistNode_hpp


#include <iostream>

#define QlistNodePosi(T) QuadlistNode<T>*   //跳转表节点位置


template <typename T> struct QuadlistNode {
    T entry; //所存词条
    QlistNodePosi(T) pred;  //前驱
    QlistNodePosi(T) succ;  //后继
    QlistNodePosi(T) above;  //上邻
    QlistNodePosi(T) below;  //下邻
    
    QuadlistNode(T e = T(),QlistNodePosi(T) p = NULL,QlistNodePosi(T) s = NULL,QlistNodePosi(T) a = NULL,QlistNodePosi(T) b = NULL)
    : entry(e),pred(p),succ(s),above(a),below(b) {}
    
    QlistNodePosi(T) insertAsSuccAbove(T const& e,QlistNodePosi(T) b = NULL); //插入新节点，以当前节点为前驱，以节点b为下邻
    
};

template <typename T> QlistNodePosi(T) QuadlistNode<T>::insertAsSuccAbove(const T &e,QlistNodePosi(T) b ) {
    QlistNodePosi(T) x = new QuadlistNode<T>(e, this, succ, NULL, b);
    succ->pred = x;
    succ = x;    //设置水平逆向链接
    if (b) {
        b -> above = x;   //设置垂直逆向链接
    }
    
    return x; //返回新节点的位置
}

#endif /* QuadlistNode_hpp */
