//
//  ListNode.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef ListNode_hpp
#define ListNode_hpp

#include <iostream>

typedef int Rank;
#define ListNodePosi(T) ListNode<T>*    //列表节点位置

template <typename T> struct ListNode {
    T data;
    ListNodePosi(T) pred;
    ListNodePosi(T) succ;
    
    //构造函数
    ListNode() {}
    ListNode(T e,ListNodePosi(T) p = NULL,ListNodePosi(T) s = NULL):data(e),pred(p),succ(s){}
    
    //操作接口
    ListNodePosi(T) insertAsPred(T const& e);
    ListNodePosi(T) insertAsSucc(T const& e);
};

#endif /* ListNode_hpp */
