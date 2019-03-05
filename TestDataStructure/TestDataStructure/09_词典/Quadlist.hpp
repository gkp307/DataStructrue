//
//  Quadlist.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef Quadlist_hpp
#define Quadlist_hpp

#include "QuadlistNode.hpp"
#include "ListNode.hpp"

template <typename T> class Quadlist {
private:
    //规模，头哨兵，尾哨兵
    int _size;
    QlistNodePosi(T) header;
    QlistNodePosi(T) trailer;
    
protected:
    void init();
    int clear();
    
public:
    Quadlist() {
        init();
    }
    
    ~Quadlist() {
        clear();
        delete header;
        delete trailer;
    }
    
    //只读访问接口
    Rank size() const {
        return _size;
    }
    
    bool empty() const {
        return _size <= 0;
    }
    
    QlistNodePosi(T) first() {
        return header -> succ;
    }
    
    QlistNodePosi(T) last() {
        return trailer -> pred;
    }
    
    bool valid(QlistNodePosi(T) p) {
        return p && (trailer != p) && (header != p);
    }
    
    //可写访问接口
    T remove(QlistNodePosi(T) p); //删除(合法)位置p处的节点，返回被删除节点的数值
    QlistNodePosi(T) insertAfterAbove(T const& e,QlistNodePosi(T) p,QlistNodePosi(T) b = NULL);
    
    //遍历
    void traverse(void (*)(T&)); //遍历各节点，一次实施指定操作(函数指针，只读或局部修改)
    template <typename VST> void traverse(VST&); //操作器
    
};


template <typename T> void Quadlist<T>::init() {
    header = new QuadlistNode<T>;
    trailer = new QuadlistNode<T>;
    header->succ = trailer;
    header->pred = NULL;
    
    trailer->pred = header;
    trailer->succ = NULL;
    
    header->above = trailer->above = NULL;
    header->below = trailer->below = NULL;
    
    _size = 0;
}

template <typename T> QlistNodePosi(T) Quadlist<T>::insertAfterAbove(const T &e, QlistNodePosi(T) p,QlistNodePosi(T) b) {
    _size++;
    return p -> insertAsSuccAbove(e, b);
}

template <typename T> T Quadlist<T>::remove(QlistNodePosi(T) p) {
    //assert: p为Quadlist中的合法位置
    p -> pred -> succ = p -> succ;
    p -> succ -> pred = p -> pred;
    _size--;
    
    T e = p -> entry;
    delete p;  //备份词条，释放节点
    return e; //返回词条
}

template <typename T> int Quadlist<T>::clear() {
    //清空Quadlist
    int oldSize = _size;
    while (0 < _size) {
        remove(header -> succ);  //逐个删除所有节点
    }
    
    return oldSize;
}

#endif /* Quadlist_hpp */
