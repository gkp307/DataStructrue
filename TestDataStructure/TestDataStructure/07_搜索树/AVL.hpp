//
//  AVL.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef AVL_hpp
#define AVL_hpp

#include "BST.hpp"

/*
 任一节点v的平衡因子(balance factor)定义为"其左右子树的高度差",即
 balFac(v) = height(lc(v)) - height(rc(c))
 本书中空树高度取-1，单节点子树(叶节点)高度取0
 
 所谓AVL树，即平衡因子受限的二叉搜索树----其中各节点平衡因子的绝对值均不超过1
 
 */


template <typename T> class AVL : public BST<T> {
public:
    BinNodePosi(T) insert(const T& e);
    bool remove(const T& e);
    
    //BST::serach()等其余接口可直接沿用
};

#define Balanced(x) (stature( (x).lc) == stature( (x).rc ) )  //理想平衡条件
#define BalFac(x) (stature( (x).lc) - stature( (x).rc ) )  //平衡因子
#define AvlBalanced(x) ( (-2 < BalFac(x) ) && ( BalFac(x) < 2 )  )  //AVL平衡条件

//在左右孩子中取更高者
//在AVL平衡调整前。借此确定重构方案
#define tallerChild(x) (\
stature( (x)->lc ) > stature( (x)->rc ) ? (x)->lc : ( /*左高*/ \
stature( (x)->lc ) < stature( (x)->rc ) ? (x)->rc : ( /*右高*/ \
IsLChild( *(x) ) ? (x)->lc : (x)->rc \
) \
) \
)

//将关键码插入AVL树中
template <typename T> BinNodePosi(T) AVL<T>::insert(const T &e) {
    BinNodePosi(T) &x = this -> search(e);
    if (x) {
        return x;  //确认目标节点不存在
    }
    
    BinNodePosi(T) xx = x = new BinNode<T>(e, this->_hot);
    this->_size++;
    
    //此时，x的父亲_hot若增高，则其祖父有可能失衡
    for (BinNodePosi(T) g = this->_hot; g; g = g -> parent) {
        //从x之父出发向上，逐层检查各代祖先g
        
        if ( ! AvlBalanced(*g)) {
            //一旦发现g失衡,则采用"3+4"算法使之复衡，并将子树重新接入原树
            FromParentTo(*g, this->_root) = this -> rotateAt( tallerChild(g));
            break; //g复衡后，局部子树高度必然复原；其祖先亦必如此，故调整随机结束
        }else{
            //否则(g依然平衡),只需简单的更新其高度(注意:即使g为失衡，高度亦可能增加d)
            this -> updateHeight(g);
        }
    }//至多只需一次调整，若果真做过调整，则全树高度必然复原
    
    return xx; //返回新节点位置
}//无论e是否存在于原树中，总有AVL::insert(e)->data == e


//从AVL树中删除关键码e
template <typename T> bool AVL<T>::remove(const T &e) {
    BinNodePosi(T) & x = this->search(e);
    if (!x) {
        return false; //确认目标存在(留意_hot的设置)
    }
    
    removeAt(x, this->_hot);
    this->_size--;//先按BST规则删除之(此后，原节点之父_hot及其祖先均可能失衡)
    
    for (BinNodePosi(T) g = this->_hot; g; g = g-> parent) {
        //从_hot出发向上，逐层检查各代祖先g,一旦发现g失衡，则采用"3+4"算法使之复衡，并将子树连至原父亲
        if (! AvlBalanced(*g)) {
            g = FromParentTo(*g, this->_root) = this->rotateAt(tallerChild(tallerChild(g)));
        }
        
        this->updateHeight(g); //并更新其高度(注意:即使g未失衡，高度亦可能降低)
    }//可能需做Omega(logn)次调整--无论是否做过调整，全树高度均可能降低
    return true;  //删除成功
}//若目标节点存在且被删除，返回true；否则返回false

#endif /* AVL_hpp */
