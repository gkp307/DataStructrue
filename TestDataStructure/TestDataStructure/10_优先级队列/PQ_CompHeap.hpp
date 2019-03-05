//
//  PQ_CompHeap.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef PQ_CompHeap_hpp
#define PQ_CompHeap_hpp

#include "Vector.hpp"
#include "PQ.hpp"


#define InHeap(n, i)            ( ((-1) < (i)) && ( (i)<(n) ) )  //判断PQ[i]是否合法
#define Parent(i)               ( (i-1) >> 1) //PQ[i]的父节点
#define LastInternal(n)         Parent(n-1) //最后一个内部节点(即末节点的父亲)
#define LChild(x)               (1 + ( (i) << 1)) //PQ[i]的左孩子
#define RChild(x)               ( (1 + (i)) << 1)//PQ[i]的右孩子
#define ParentValid(i)          (0 < i) //判断PQ[i]是否有父亲
#define LChildValid(n, i)       InHeap(n, LChild(i)) //判断PQ[i]是否有一个(左)孩子
#define RChildValid(n, i)       InHeap(n, RChild(i)) //判断PQ[i]是否有一个(右)孩子
#define Bigger(PQ, i, j)        ( lt(PQ[i], PQ[j]) ? j : i ) //取大者(等时前者优先)
#define ProperParent(PQ, n, i)  /*父子(至多)三者中的大者*/ \
( RChildValid(n,i) ? Bigger(PQ, Bigger(PQ, i, LChild(i) ),RChild(i) ) : \
( LChildValid(n, i) ? Bigger(PQ, i, LChild(i) ) : i\
)\
) //相等时父节点优先，如此可避免不必要的交换


//完全二叉堆
template <typename T> class PQ_ComplHeap : public PQ<T>, public Vector<T> {
protected:
    Rank percolateDown(Rank n,Rank i); //下滤
    Rank percolateUp(Rank i); //上滤
    void heapify(Rank n); //Floyd建堆算法
    
public:
    PQ_ComplHeap() {}
    
    PQ_ComplHeap(T* A,Rank n) {
        this->copyFrom(A, 0, n);
        heapify(n);
    }
    
    void insert(T); //按照比较器确定的优先级次序，插入词条
    T getMax();//读取优先级最高的词条
    T delMax(); //删除优先级最高的词条
    
};

//优先级最高的词条总是位于堆顶
template <typename T> T PQ_ComplHeap<T>::getMax() {
    return this->_elem[0];
}

//将词条插入完全二叉堆中
template <typename T> void PQ_ComplHeap<T>::insert(T e) {
    Vector<T>::insert(e);  //首先将新词条接至向量末尾
    percolateUp(this->_size - 1);//在对该词条实施上滤调整
}

//对向量中的第i个词条实施上滤操作,i < _size
template <typename T> Rank PQ_ComplHeap<T>::percolateUp(Rank i) {
    while (ParentValid(i)) {
        //只要i有父亲(尚未抵达堆顶)，则将i之父记作j
        Rank j = Parent(i);
        
        if ( lt(this->_elem[i], this->_elem[j])) {
            break;  //一旦当前父子不再逆序，上滤旋即完成
        }
        
        //否则，父子交换位置，并继续考察上一层
        std::swap(this->_elem[i], this->_elem[j]);
        i = j;
    }
    
    return i; //返回上滤最终抵达的位置
}

//删除非空完全二叉堆中优先级最高的词条
template <typename T> T PQ_ComplHeap<T>::delMax() {
    //摘除堆顶(首词条)，代之以末词条
    T maxElem = this->_elem[0];
    this->_elem[0] = this->_elem[--this->_size];
    
    percolateDown(this->_size, 0); //对新堆顶实施下滤
    return maxElem; //返回此前备份的最大词条
}

//对向量前n个词条中的第i个实施下滤，i < n
template <typename T> Rank PQ_ComplHeap<T>::percolateDown(Rank n, Rank i) {
    Rank j;  //i及其(至多两个)孩子中，勘为父者
    while (i != (j = ProperParent(this->_elem, n, i))) {
        //只要i非j，则二者换位，并继续考察下降后的i
        std::swap(this->_elem[i], this->_elem[j]);
        i = j;
    }
    
    return i; //返回下滤抵达的位置(亦i亦j)
}

//Floyd建堆算法,O(n)时间
template <typename T> void PQ_ComplHeap<T>::heapify(Rank n) {
    for (int i = LastInternal(n); InHeap(n, i); i--) {
        //自底向上，依次下滤各内部节点
        percolateDown(n, i);
    }
}

template <typename T> void Vector<T>::heapSort(Rank lo, Rank hi) {
    //0 <= lo < hi <= size
    PQ_ComplHeap<T> H(this->_elem + lo, hi - lo); //将待排序区间建成一个完全二叉堆
    while (! H.empty()) { //反复地摘除最大元并归入已排序的后缀，直至堆空
        this->_elem[--hi] = H.delMax(); //等效于堆顶与末元素对换后下滤
    }
    
    
}

#endif /* PQ_CompHeap_hpp */
