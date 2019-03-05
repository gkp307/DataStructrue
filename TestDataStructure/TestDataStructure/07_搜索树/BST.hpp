//
//  BST.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef BST_hpp
#define BST_hpp

#include "BinTree.hpp"
#include "Entry.hpp"
//二叉搜索树
//任何一个二叉树是二叉搜索树，当且仅当其中序遍历序列单调非降
template <typename T> class BST : public BinTree<T> {
protected:
    BinNodePosi(T) _hot; //"命中"节点的父亲
    BinNodePosi(T) connect34(  //按照"3+4"的结构，连接3个节点及3颗子树
                             BinNodePosi(T),BinNodePosi(T),BinNodePosi(T),
                             BinNodePosi(T),BinNodePosi(T),BinNodePosi(T),BinNodePosi(T));
    BinNodePosi(T) rotateAt(BinNodePosi(T) x); //对x及其父亲、祖父做统一旋转调整
    
public:
    virtual BinNodePosi(T)& search(const T& e); //查找
    virtual BinNodePosi(T) insert(const T& e); //插入
    virtual bool remove(const T& e); //删除
};


#pragma mark - 查找
//在以v为根的(AVL、SPLAY、rbTree等)BST子树中查找关键码e
template <typename T>
static BinNodePosi(T)& searchIn(BinNodePosi(T) & v, const T& e,BinNodePosi(T) & hot) {
    if (!v || (e == v->data)) {
        return v;
    }
    //一般情况：先记下当前节点，然后再深入一层，递归查找
    hot = v;
    
    return searchIn((e < v->data)? v->lc:v->rc, e, hot);
}

//searchIn的迭代版
#define EQUAL(e, v) (!(v) || (e) == (v)->data) //节点v(或假想的通配哨兵)的关键码等于e

template <typename T>
static BinNodePosi(T)& searchIn_I2(BinNodePosi(T) & v, const T& e,BinNodePosi(T) & hot) {
    if (EQUAL(e, v)) {
        return v;    //退化情况:在子树根节点v处命中
    }
    
    hot = v;
    
    while (1) {
        //确定深入方向
        BinNodePosi(T) & c = ( e < hot->data) ? hot->lc : hot->rc;
        
        if (EQUAL(e, c)) {
            return c;
        }
        
        hot = c;
    }//hot始终指向最后一个失败节点
    
}//返回时，返回值指向命中节点(或假想的通配哨兵),hot指向其父亲(退化时为初始值NULL)


template <typename T> BinNodePosi(T) & BST<T>::search(const T &e) {
    return searchIn(this->_root, e, _hot = NULL);
}

#pragma mark - 插入
//将关键码e插入BST树中
template <typename T> BinNodePosi(T) BST<T>::insert(const T &e) {
    BinNodePosi(T) &x = search(e);
    if (x) {
        return x; //确认目标不存在(留意对_hot的设置)
    }
    
    x = new BinNode<T> (e, _hot);  //创建新节点x:以e为关键码，以_hot为父
    this -> _size++;  //更新全树规模
    this -> updateHeightAbove(x);  //更新x及其历代祖先的高度
    return x;   //新插入的节点，必为叶子
}//无论e是否存在于原树中，返回时总有x->data == e

#pragma mark - 删除
template <typename T>
static BinNodePosi(T) removeAt(BinNodePosi(T) &x,BinNodePosi(T) &hot) {
    BinNodePosi(T) w = x;        //实际被摘除的节点，初值同x
    BinNodePosi(T) succ = NULL;  //实际被删除节点的接替者
    
    if ( ! HasLChild(*x) ){
        //若*x的左子树为空，则可直接将*x替换为其右子树
        succ = x = x -> rc;
    }else if ( !HasRChild(*x) ) {
        //若右子树为空，则可对称的处理---注意，此时succ != NULL
        succ = x = x -> lc;
    }else{
        //若左右子树均存在，则选择x的直接后继作为实际被摘除节点，为此需要(在右子树中)找到*x的直接后继*w
        //交换*x和*w的数据元素
        w = w -> succ();
        std::swap(x->data, w->data);
        BinNodePosi(T) u = w -> parent;
        ( (u == x) ? u->rc : u->lc ) = succ = w -> rc;  //隔离节点*w
    }
    
    
    hot = w -> parent; //记录实际被删除节点的父亲
    if (succ) {
        succ -> parent = hot;   //并将被删除节点的接替者与hot相连
    }
    
    delete w -> data;
    delete w;
    
    return succ;//释放被摘除节点，返回接替者
}

//从BST树中删除关键码e
template <typename T> bool BST<T>::remove(const T &e) {
    BinNodePosi(T) &x = search(e);
    if (!x) {   //确认目标存在(留意_hot的设置)
        return false;
    }
    
    removeAt(x, _hot);   //实施删除
    this -> _size--;
    this -> updateHeightAbove(_hot); //更新_hot及其历代祖先的高度
    return true;
}//删除成功与否，由返回值指示


template <typename T> BinNodePosi(T) BST<T>::connect34(BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T)c, BinNodePosi(T) T0, BinNodePosi(T) T1, BinNodePosi(T) T2, BinNodePosi(T) T3) {
    a -> lc = T0;
    if (T0) {
        T0 -> parent = a;
    }
    
    a -> rc = T1;
    if (T1) {
        T1 -> parent = a;
    }
    this -> updateHeight(a);
    
    
    c -> lc = T2;
    if (T2) {
        T2 -> parent = c;
    }
    
    c -> rc = T3;
    if (T3) {
        T3 -> parent = c;
    }
    this -> updateHeight(c);
    
    b -> lc = a;
    a -> parent = b;
    b -> rc = c;
    c -> parent = b;
    this -> updateHeight(b);
    return b;  //该子树新的根节点
}

//v为非空孙辈节点
template <typename T> BinNodePosi(T) BST<T>::rotateAt(BinNodePosi(T) v){
    
    BinNodePosi(T) p = v -> parent;
    BinNodePosi(T) g = p -> parent;
    
    if (IsLChild(*p)) { //zig
        
        if (IsLChild(*v)) { //zig--zig
            p -> parent = g -> parent;//向上联接
            return connect34(v, p, g, v->lc, v->rc, p->rc, g->rc);
        }else{  //zig--zag
            v -> parent = g -> parent;  //向上联接
            return connect34(p, v, g, p->lc, v->lc, v->rc, g->rc);
        }
        
    }else{ //zag
        
        if (IsRChild(*v)) { //zag--zag
            p -> parent = g -> parent;//向上联接
            return connect34(g, p, v, g->lc, p->lc, v->lc, v->rc);
        }else{ //zag---zig
            v -> parent = g -> parent; //向上联接
            return connect34(g, v, p, g->lc, v->lc, v->rc, p->rc);
        }
        
        
    }
    
}

#endif /* BST_hpp */
