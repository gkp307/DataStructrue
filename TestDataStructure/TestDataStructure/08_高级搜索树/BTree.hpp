//
//  BTree.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef BTree_hpp
#define BTree_hpp

#include "Vector.hpp"

#define BTNodePosi(T) BTNode<T>*    //B-树节点位置

//B-树节点模版类
template <typename T> struct BTNode{
    BTNodePosi(T) parent; //父节点
    Vector<T> key;  //关键码向量
    Vector<BTNodePosi(T)> child; //孩子向量(其长度总比key多一)
    
    BTNode() {
        parent = NULL;
        child.insert(0, NULL);
    }
    
    BTNode(T e,BTNodePosi(T) lc = NULL,BTNodePosi(T) rc = NULL) {
        parent = NULL;
        key.insert(0, e);
        child.insert(0, lc);
        child.insert(1, rc); //一个关键码，两个孩子
        
        if (lc) {
            lc->parent = this;
        }
        
        if (rc) {
            rc -> parent = this;
        }
    }
};

//B-树模版类
template <typename T> class BTree{
protected:
    int _size;  //存放的关键码总数
    int _order;  //B-树的阶次，至少为3----创建时指定，一般不能修改
    
    BTNodePosi(T) _root;   //根节点
    BTNodePosi(T) _hot;  //BTree::search()最后访问的非空(除非树空)的节点位置
    
    void solveOverflow(BTNodePosi(T)); //因插入而上溢之后的分裂处理
    void solveUnderflow(BTNodePosi(T));  //因删除而下溢之后的合并处理
    
public:
    
    BTree(int order = 3) : _order(order), _size(0) {
        _root = new BTNode<T>();
    }
    
    ~BTree() {
        if (_root) {
            delete _root;
        }
    }
    
    int const order() {
        return _order;
    }
    
    int const size() {
        return _size;
    }
    
    BTNodePosi(T) & root() {
        return _root;
    }
    
    bool empty() const {
        return !_root;
    }
    
    BTNodePosi(T) search(const T& e);
    bool insert(const T& e);
    bool remove(const T& e);
    
};

//在B-树中查找关键码e
template <typename T> BTNodePosi(T) BTree<T>::search(const T &e) {
    BTNodePosi(T) v = _root;
    _hot = NULL;
    
    while (v) {
        Rank r = v->key.search(e);//在当前节点中，找到不大于e的最大关键码
        
        if ( (0 <= r) && (e == v->key[r]) ) {
            return v;  //成功:在当前节点中命中目标关键码
        }
        
        _hot = v;
        v = v->child[r+1]; //否则，转入对应子树(_hot指向其父)---需做I/O,最费时间
    }//这里在向量内是二分查找，但对通常的_order可直接顺序查找
    
    return NULL;//失败:最终抵达外部节点
}

//将关键码e插入B树中
template <typename T> bool BTree<T>::insert(const T &e) {
    BTNodePosi(T) v = search(e);
    if (v) {
        return false;
    }
    
    Rank r = _hot->key.search(e);
    _hot->key.insert(r+1, e);  //将关键码插至对应的位置
    _hot->child.insert(r+2, NULL); //创建一个空子树指针
    _size++;  //更新全树规模
    solveOverflow(_hot);  //如有必要，需做分裂
    return true; //插入成功
}

//关键码插入后若节点上溢，则做节点分裂处理
template <typename T>
void BTree<T>::solveOverflow(BTNodePosi(T) v) {
    if (_order >= v->child.size()) {
        return;  //递归基:当前节点并为上溢
    }
    
    Rank s = _order / 2; //轴点(此时应有_order=key.size()=child.size()-1)
    BTNodePosi(T) u = new BTNode<T>(); //注意:新节点已有一个空孩子
    
    for (Rank j = 0; j < _order-s-1; j++) {
        //v右侧_order-s-1个孩子及关键码分裂为右侧节点
        u -> child.insert(j, v->child.remove(s+1)); //逐个移动效率低
        u -> key.insert(j, v->key.remove(s+1) ); //此策略可改进
    }
    
    u -> child[_order-s-1] = v->child.remove(s+1); //移动v最靠右的孩子
    
    if (u -> child[0]) {
        //若u的孩子们非空，则令他们的父节点统一
        for (Rank j = 0; j < _order-s; j++) {
            u -> child[j]->parent = u; //指向u
        }
    }
    
    BTNodePosi(T) p = v -> parent; //v当前的父节点p
    if (!p) {
        _root = p = new BTNode<T>();
        p -> child[0] = v;
        v -> parent = p;
    }
    
    Rank r = 1 + p -> key.search(v -> key[0]); //p中指向u的指针的秩
    p -> key.insert(r, v -> key.remove(s));  //轴点关键码上升
    p -> child.insert(r + 1, u);
    u -> parent = p;  //新节点u与父节点p互联
    solveOverflow(p);  //上升一层，如有必要则继续分裂-----至多递归O(logn)层
    
}

//从BTree树中删除关键码e
template <typename T> bool BTree<T>::remove(const T &e) {
    BTNodePosi(T) v = search(e);
    if (!v) {
        return false;
    }
    
    Rank r = v -> key.search(e); //确定目标关键码在节点v中的秩
    
    if (v -> child[0]) {
        //若v非叶子，则e的后继必属于某叶节点
        BTNodePosi(T) u = v->child[r+1];  //在右子树中一直向左，即可找出e的后继,并与之交换位置
        
        while ( u->child[0] ) {
            u = u -> child[0];
        }
        
        v -> key[r] = u -> key[0];
        v = u;
        r = 0;
    }//至此，v必然位于最底层，且其中第r个关键码就是待删除者
    
    //删除额，以及其下两个外部节点之一
    v -> key.remove(r);
    v -> child.remove(r+1);
    _size--;
    
    solveUnderflow(v);  //如有必要，需做旋转或合并
    return true;
}

//关键码删除后若节点下溢，则做节点旋转或合并处理
template <typename T>
void BTree<T>::solveUnderflow(BTNodePosi(T) v){
    
    if ( (_order + 1 ) / 2 <= v->child.size() ) {
        return;
    }
    
    BTNodePosi(T) p = v -> parent;
    if (!p) { //递归基:已到根节点，没有孩子的下限
        if (!v->key.size() && v->child[0]) {
            //但倘若作为树根的v已不包含关键码，却有(唯一的)非空孩子，则这个节点可被跳过,并因不再有用而销毁
            _root = v -> child[0];
            _root -> parent = NULL;
            v -> child[0] = NULL;
            delete v;
        }
        return;
    }
    
    Rank r = 0;
    while (p->child[r] != v) {
        r++;
    }
    
    //情况1:向左兄弟借关键码
    if (0 < r) {
        BTNodePosi(T) ls = p -> child[r-1]; //左兄弟必存在
        
        if ( (_order + 1) /2 < ls->child.size()) {
            //若该兄弟足够胖，则p借出一个关键码给v(作为最小关键码)
            v -> key.insert(0, p->key[r-1]);
            p -> key[r-1] = ls -> key.remove(ls->key.size()-1);
            v -> child.insert(0, ls->child.remove(ls->child.size() - 1));
            
            //同时ls的最右侧孩子过继给v
            if (v->child[0]) {
                v->child[0]->parent = v;  //作为v的最左侧孩子
            }
            return; //至此，通过右旋已完成当前层(以及所有层)的下溢处理
        }
    }
    
    
    //情况2:向右兄弟借关键码
    if (p -> child.size() - 1 > r) {
        //若v不是p的最后一个孩子，则右兄弟必存在，若该兄弟足够胖
        //则p借出一个关键码给v
        BTNodePosi(T) rs = p -> child[r+1];
        if ( (_order+1)/2 < rs->child.size()) {
            v -> key.insert(v->key.size(), p->key[r]);
            p -> key[r] = rs -> key.remove(0); //ls的最小关键码转入p
            v -> child.insert(v->child.size(), rs->child.remove(0));
            
            //同时rs的最左侧孩子过继给v
            if (v->child[v->child.size()-1]) {
                v->child[v->child.size()-1] -> parent = v;
            }
            return;//至此，通过左旋已完成当前层(以及所有层)的下溢处理
        }
    }
    
    
    //情况3:左右兄弟要么为空(但不可能同时),要么都太"瘦" ----- 合并
    if (0 < r) {  //与左兄弟合并
        BTNodePosi(T) ls = p->child[r-1]; //左兄弟必存在
        ls->key.insert(ls->key.size(), p->key.remove(r-1));
        p->child.remove(r);
        
        //p的第r-1个关键码转入ls，v不再是p的第r个孩子
        ls -> child.insert(ls->child.size(), v->child.remove(0));
        
        
        //v的最左侧孩子过继给ls做最右侧孩子
        if (ls->child[ls->child.size()-1]) {
            ls->child[ls->child.size()-1] -> parent = ls;
        }
        
        //v剩余的关键码和孩子，一次转入ls
        while (!v->key.empty()) {
            ls->key.insert(ls->key.size(), v->key.remove(0));
            ls->child.insert(ls->child.size(), v->child.remove(0));
            
            if (ls->child[ls->child.size() - 1]) {
                ls->child[ls->child.size() - 1] -> parent = ls;
            }
        }
        
        delete v;
        
    }else{  //与右兄弟合并
        BTNodePosi(T) rs = p -> child[r+1]; //右兄弟必存在
        rs->key.insert(0, p->key.remove(r));
        p->child.remove(r);
        
        //p的第r个关键码转入rs，v不再是p的第r个孩子
        rs->child.insert(0, v->child.remove(v->child.size() - 1));
        
        
        if (rs->child[0]) {
            rs->child[0] -> parent = rs;
        }
        
        while (!v->key.empty()) {
            //v剩余的关键码和孩子，一次转入rs
            rs->key.insert(0, v->key.remove(v->key.size()-1 ));
            rs->child.insert(0, v->child.remove(v->child.size()-1));
            
            if (rs->child[0]) {
                rs->child[0] -> parent = rs;
            }
        }
        
        delete v;
    }
    
    solveUnderflow(p); // 上升一层，如有必要则继续分裂---至多递归O(logn)层
    return;
    
}



#endif /* BTree_hpp */
