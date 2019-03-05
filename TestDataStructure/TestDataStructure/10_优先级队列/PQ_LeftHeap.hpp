//
//  PQ_LeftHeap.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef PQ_LeftHeap_hpp
#define PQ_LeftHeap_hpp

#include "PQ.hpp"
#include "BinTree.hpp"


//左式堆
template <typename T> class PQ_LeftHeap : public PQ<T>, public BinTree<T> {
public:
    PQ_LeftHeap() {}
    
    PQ_LeftHeap(T* E,int n) {
        //批量构造，可改进为Floyd建堆算法
        for (int i = 0; i < n; i++) {
            insert(E[i]);
        }
    }
    
    void insert(T) override;
    T getMax() override;
    T delMax() override;
};

//根据相对优先级确定适宜的方式，合并以a和b为根节点的两个左式堆
template <typename T>
static BinNodePosi(T) merge(BinNodePosi(T) a, BinNodePosi(T) b) {
    if (!a) {
        return b;
    }
    
    if (!b) {
        return a;
    }
    
    if (lt(a->data, b->data)) {
        std::swap(a, b); //一般情况：首先确保b不大
    }
    
    a->rc = merge(a->rc, b); //将a的右子堆与b合并
    a->rc->parent = a;  //并更新父子关系
    
    if (!a->lc || a->lc->npl < a->rc->npl) {
        std::swap(a->lc, a->rc); //若有必要，交换a的左右子堆，以确保右子堆的npl不大
    }
    
    a->npl = a->rc ? a->rc->npl + 1 : 1; //更新a的npl
    return a; //返回合并后的堆顶
}//本算法实现结构上的合并，堆的规模须由上层调用者负责更新


template <typename T> T PQ_LeftHeap<T>::delMax() {
    BinNodePosi(T) lHeap = this->_root->lc; //左子堆
    BinNodePosi(T) rHeap = this->_root->rc; //右子堆
    T e = this->_root->data;
    delete this->_root;
    this->_size--; //删除根节点
    
    this->_root = merge(lHeap, rHeap); //原左右子堆合并
    
    if (this->_root) {
        this->_root->parent = NULL; //若堆非空，还需相应设置父子链接
    }
    
    return e; //返回原根节点的数据项
}

template <typename T> void PQ_LeftHeap<T>::insert(T e) {
    //基于合并操作的词条插入算法
    BinNodePosi(T) v = new BinNode<T>(e);
    this->_root = merge(this->_root, v);
    this->_root->parent = NULL;
    this->_size++; //更新规模
}


#endif /* PQ_LeftHeap_hpp */
