//
//  RedBlack.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef RedBlack_hpp
#define RedBlack_hpp

/*
 由红黑两色节点组成的二叉搜索树若满足以下条件，即为"红黑树(red-black tree)"
 (1)树根始终为黑色
 (2)外部节点均为黑色
 (3)其余节点若为红色，则其孩子节点必为黑色
 (4)从任一外部节点到根节点的沿途，黑节点的数目相等
 */


#include "BST.hpp"

template <typename T> class RedBlack : public BST<T> {
protected:
    void solveDoubleRed(BinNodePosi(T) x); //双红修正
    void solveDoubleBlack(BinNodePosi(T) x); //双黑修正
    int updateHeight(BinNodePosi(T) x); //更新节点x的高度
public:
    BinNodePosi(T) insert(const T& e) override;
    bool remove(const T& e) override;
    
    //BST::search()等其余接口可直接沿用
};

#define IsBlack(p)  (!(p) || (RB_BLACK == (p)->color))       //外部节点也视做黑节点
#define IsRed(p) (!IsBlack(p))   //非黑即红
#define BlackHeightUpdated(x)  (/*RedBlack高度更新条件*/\
(stature((x).lc) == stature((x).rc) ) && \
((x).height == ( IsRed(& x) ? stature( (x).lc ) : stature( (x).rc ) + 1 ) ) \
)


//更新节点高度(这里的height指红黑树的黑高度)
template <typename T> int RedBlack<T>::updateHeight(BinNodePosi(T) x) {
    x->height = std::max(stature(x->lc), stature(x->rc));  //孩子一般黑高度相等，除非出现双黑
    return IsBlack(x) ? x->height++ : x->height; //若当前节点为黑，则计入黑深度
}//因统一定义stature(NULL)=-1,故height比黑高度少一，好在不至于影响到各种算法的比较判断

template <typename T> BinNodePosi(T) RedBlack<T>::insert(const T &e) {
    BinNodePosi(T) & x = this -> search(e);
    if (x) {
        return x;  //确认目标不存在(留意对_hot的设置)
    }
    
    x = new BinNode<T>(e, this->_hot,NULL,NULL,-1); //创建红节点x;以_hot为父，黑高度-1
    this->_size++;
    
    solveDoubleRed(x);
    return x ? x : this->_hot->parent;  //经双红修正后，即可返回
}//无论e是否存在于原树中，返回时总有x->data==e


template <typename T> void RedBlack<T>::solveDoubleRed(BinNodePosi(T) x) {
    //x当前必为红
    if ( IsRoot(*x)) {
        //若已(递归)转至树根，则将其转黑，整树黑高度也随之递增
        this->_root->color = RB_BLACK;
        this->_root->height++;
        return;
    }//否则，x的父亲p必存在
    
    BinNodePosi(T) p = x -> parent;
    if (IsBlack(p)) {
        return;//若p为黑，则可终止调整
    }
    
    BinNodePosi(T) g = g->parent;  //既然p为红，则x的祖父必存在，且必为黑色
    BinNodePosi(T) u = uncle(x);  //以下，视x叔父u的颜色分别处理
    
    if (IsBlack(u)) {  //u为黑色，含NULL时
        if (IsLChild(*x) == IsLChild(*p)) { //若x与p同侧(即zig-zig或zag-zag)，则p由红转黑，x保持红
            p->color = RB_BLACK;
        }else{  //若x与p异侧(即zig-zag或zag-zig)，则x由红转黑，p保持红
            x->color = RB_BLACK;
        }
        
        g->color = RB_RED; //g必定由黑转红
        //以上虽保证总共两次染色，但因增加了判断而得不偿失
        //在旋转后将根值黑、孩子置红，虽需三次染色但效率更高
        BinNodePosi(T) gg = g->parent;
        BinNodePosi(T) r = FromParentTo(*g, this->_root) = this->rotateAt(x); //调整后的子树根节点
        r -> parent = gg; //与原曾祖父相连
    }else{
        //若u为红色
        p->color = RB_BLACK;
        p->height++;
        u->color = RB_BLACK;
        u->height++;
        
        if (!IsRoot(*g)) {
            g->color = RB_RED; //g若非根，则转红
        }
        solveDoubleRed(g); //继续调整g(类似于尾递归，可优化为迭代形式)
    }
    
}

//从红黑树中删除关键码
template <typename T> bool RedBlack<T>::remove(const T &e) {
    BinNodePosi(T) & x = this->search(e);
    if (!x) {
        return false;
    }
    
    BinNodePosi(T) r = removeAt(x, this->_hot);
    if (!(--this->_size)) {
        return true;
    }
    
    //assert:_hot某一孩子刚被删除，且被r所指节点(可能是NULL)接替。以下检查时候失衡，并做必要调整
    if (!this->_hot) {
        //若刚被删除的是根节点，则将其置黑，并更新黑高度
        this->_root->color = RB_BLACK;
        updateHeight(this->_root);
        return true;
    }
    
    //assert:以下，原x(现r)必非根，_hot必非空
    if (BlackHeightUpdated(*this->_hot)) {
        return true;
    }
    
    if (IsRed(r)) { //否则，若r为红，则只需令其转黑
        r->color = RB_BLACK;
        r->height++;
        return true;
    }
    
    //assert: 以下,原x(现r)均为黑色
    solveDoubleBlack(r);
    return true;       //经双黑调整后返回
}//若目标节点存在且被删除，返回true；否则返回false


template <typename T> void RedBlack<T>::solveDoubleBlack(BinNodePosi(T) r) {
    BinNodePosi(T) p = r ? r->parent : this->_hot;
    if (!p) {
        return;
    }
    
    BinNodePosi(T) s = ( r == p->lc) ? p->rc : p->lc;  //r的兄弟
    
    if (IsBlack(s)) {
        //兄弟s为黑
        BinNodePosi(T) t = NULL; //s的红孩子(若左右孩子皆红，左者优先；皆黑时为NULL)
        
        if (IsRed(s->rc)) {
            t = s->rc;  //右子
        }
        if (IsRed(s->lc)) {
            t = s->lc;//左子
        }
        
        if (t) {  //黑s由红孩子:BB-1
            RBColor oldColor = p->color;//备份原子树根节点p的颜色，并对t及其父亲、祖父
            //以下，通过旋转重平衡，并将新子树的左右孩子染黑
            BinNodePosi(T) b = FromParentTo(*p, this->_root) = this->rotateAt(t);  //旋转
            if (HasLChild(*b)) {
                b->lc->color = RB_BLACK;
                updateHeight(b->lc); //左子
            }
            
            if (HasRChild(*b)) {
                b->rc->color = RB_BLACK;
                updateHeight(b->rc);  //右子
            }
            
            b->color = oldColor;
            updateHeight(b);  //新子树根节点继承原根节点的颜色
        }else{
            //黑s无红孩子
            s->color = RB_RED;
            s->height--;      //s转红
            
            if (IsRed(p)) { //BB-2R
                p -> color = RB_BLACK; //p转黑，但黑高度不变
            }else{  //BB-2B
                p->height--;   //p保持黑，但黑高度下降
                solveDoubleBlack(p);  //递归上溯
            }
        }
        
    }else{
        //兄弟s为红:BB-3
        s->color = RB_BLACK;
        p->color = RB_RED;  //s转黑，p转红
        
        BinNodePosi(T) t = IsLChild(*s) ? s->lc : s->rc; //取t与其父s同侧
        this->_hot = p;
        FromParentTo(*p, this->_root) = this->rotateAt(t); //对t及其父亲、祖父做平衡调整
        solveDoubleBlack(r); //继续修正r处双黑---此时的p已转红，古后续只能是BB-1或BB-2R
        
    }
}


#endif /* RedBlack_hpp */
