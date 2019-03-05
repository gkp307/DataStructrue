//
//  BinNode.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef BinNode_hpp
#define BinNode_hpp

#include <iostream>
#include "Queue.hpp"

#define BinNodePosi(T) BinNode<T>*
#define stature(p)  ( (p) ? (p)->height : -1)  //节点高度
typedef enum {
    RB_RED, RB_BLACK
}RBColor;  //节点颜色



//二叉树节点模版类
template <typename T> struct BinNode {
    
    T data;  //数值
    BinNodePosi(T) parent;
    BinNodePosi(T) lc;
    BinNodePosi(T) rc;  //父节点及左右孩子
    
    int height; //高度(通用)
    int npl; //Null Path Length(左式堆，也可直接用height代替)
    RBColor color; //颜色(红黑树)
    
    //构造函数
    BinNode(): parent(NULL), lc(NULL), rc(NULL),height(0),npl(1),color(RB_RED) {}
    
    BinNode(T e,BinNodePosi(T) p = NULL,BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL, int h = 0,int l = 1, RBColor c = RB_RED) : data(e), parent(p), lc(lc),rc(rc),height(h),npl(l),color(c) {}
    
    //操作接口
    int size();
    BinNodePosi(T) insertAsLC(T const&);
    BinNodePosi(T) insertAsRC(T const&);
    BinNodePosi(T) succ();
    
    template <typename VST> void travLevel(VST&);  //子树层次遍历
    template <typename VST> void travPre(VST&);  //子树先序遍历
    template <typename VST> void travIn(VST&); //子树中序遍历
    template <typename VST> void travPost(VST&); //子树后续遍历
    
    bool operator<(BinNode const& bn) {
        return data < bn.data;
    }
    
    bool operator==(BinNode const& bn) {
        return data == bn.data;
    }
    
    
};


//BinNode状态与性质的判断
#define IsRoot(x)       (! ((x).parent))
#define IsLChild(x)     ( !IsRoot(x) && ( & (x) == (x).parent->lc ) )
#define IsRChild(x)     ( !IsRoot(x) && ( & (x) == (x).parent->rc ) )
#define HasParent(x)    (!IsRoot(x))
#define HasLChild(x)    ( (x).lc )
#define HasRChild(x)    ( (x).rc )
#define HasChild(x)     ( HasLChild(x) || HasRChild(x) )
#define HasBothChild(x) ( HasLChild(x) && HasRChild(x) )
#define IsLeaf(x)       ( !HasChild(x) )


//与BinNode具有特定关系的节点及指针
#define sibling(p)      ( IsLChild( *(p) ) ?  (p)->parent->rc : (p)->parent->lc  )
#define uncle(x)        ( IsLChild( *( (x)->parent) ) ? (x)->parent->parent->rc : (x)->parent->parent->lc )
//来自父亲的引用
#define FromParentTo(x,_root) ( IsRoot(x) ? _root : ( IsLChild(x) ? (x).parent->lc : (x).parent->rc ) )


template <typename T> BinNodePosi(T) BinNode<T>::insertAsLC(T const& e) {
    return lc = new BinNode(e, this);
}

template <typename T> BinNodePosi(T) BinNode<T>::insertAsRC(T const& e) {
    return rc = new BinNode(e, this);
}

//定位节点V的直接后继(中序遍历序列)
template <typename T>
BinNodePosi(T) BinNode<T>::succ(){  //定位节点v的直接后继
    BinNodePosi(T) s = this;  //记录后继的临时变量
    if (rc) {   //若有右孩子，则直接后继必在右子树中，具体的就是
        s = rc;   //右子树中
        while (HasLChild(*s)) {
            s = s -> lc;       //最靠左最小的节点
        }
    }else{  //否则，直接后继应该是"将当前节点包含于其左子树中的最低祖先",具体地就是
        while (IsRChild(*s)) {
            s = s -> parent;  //逆向地沿右向分支，不断朝左上方移动
        }
        s = s -> parent; //最后再朝右上方移动一步，即抵达直接后继(如果存在)
    }
    
    return s;
}


template <typename T> template <typename VST>
void BinNode<T>::travLevel(VST&visit){  //子树层次遍历
    
    Queue<BinNodePosi(T)> Q;
    Q.enqueue(this);  //根节点入队
    
    while (!Q.empty()) {  //在队列再次变空之前，反复迭代
        BinNodePosi(T) x = Q.dequeue();
        visit(x -> data);
        if (HasLChild(*x)) {
            Q.enqueue(x -> lc);
        }
        if (HasRChild(*x)) {
            Q.enqueue(x -> rc);
        }
    }
    
}

template <typename T> template <typename VST>
void BinNode<T>::travPre(VST&visit){  //子树先序遍历
    travPre_I1(this, visit);
}

template <typename T> template <typename VST>
void BinNode<T>::travIn(VST&visit){ //子树中序遍历
    switch ( rand() % 5) {
        case 1:
            travIn_I1(this, visit);
            break;
            
        case 2:
            travIn_I2(this, visit);
            break;
            
        case 3:
            travIn_I3(this, visit);
            break;
            
        case 4:
            travIn_I4(this, visit);
            break;
            
        default:
            travIn_R(this, visit);
            break;
    }
}

template <typename T> template <typename VST>
void BinNode<T>::travPost(VST&visit){ //子树后续遍历
    travPost_I(this, visit);
}

#endif /* BinNode_hpp */
