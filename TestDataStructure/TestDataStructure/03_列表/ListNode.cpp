//
//  ListNode.cpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#include "ListNode.hpp"

template <typename T>
ListNodePosi(T) ListNode<T>::insertAsPred(T const&e){
    ListNodePosi(T) x = new ListNode(e, pred, this);
    pred -> succ = x;
    pred = x;
    return x;
}


template <typename T>
ListNodePosi(T) ListNode<T>::insertAsSucc(T const&e){
    ListNodePosi(T) x = new ListNode(e, this, succ);
    succ -> pred = x;
    succ = x;
    return x;
}
