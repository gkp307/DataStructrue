//
//  BinTree.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef BinTree_hpp
#define BinTree_hpp

#include "BinNode.hpp"
#include "Stack.hpp"

template <typename T> class BinTree {
protected:
    int _size;
    BinNodePosi(T) _root;
    virtual int updateHeight(BinNodePosi(T) x);  //更新节点x的高度
    void updateHeightAbove(BinNodePosi(T) x);  //更新节点x及其祖先的高度
public:
    BinTree() : _size(0),_root(NULL){}
    ~BinTree() {
        if (0 < _size) {
            remove(_root);
        }
    }
    
    int size() const {
        return _size;
    }
    
    bool empty() const {
        return !_root;
    }
    
    BinNodePosi(T) root() const {
        return _root;
    }
    
    BinNodePosi(T) insertAsRoot(T const& e);
    BinNodePosi(T) insertAsLC(BinNodePosi(T) x,T const& e); //e作为x的左孩子插入
    BinNodePosi(T) insertAsRC(BinNodePosi(T) x,T const& e); //e作为x的右孩子插入
    BinNodePosi(T) attachAsLC(BinNodePosi(T) x,BinTree<T>* &t); //t作为x的左子树接入
    BinNodePosi(T) attachAsRC(BinNodePosi(T) x,BinTree<T>* &t);//t作为右子树插入
    
    int remove(BinNodePosi(T) x); //删除以位置x除节点为根的子树，返回该子树原先的规模
    BinTree<T>* secede(BinNodePosi(T) x); //将子树x从当前树中摘除，并将其转换为一棵独立子树
    
    template <typename VST> void travLevel(VST& visit) {
        if (_root) {
            _root->travLevel(visit); //层次遍历
        }
    }
    
    template <typename VST> void travPre(VST& visit){
        if (_root) {
            _root->travPre(visit);  //先序遍历
        }
    }
    
    template <typename VST> void travIn(VST& visit){
        if (_root) {
            _root->travIn(visit);  //中序遍历
        }
    }
    
    template <typename VST> void travPost(VST& visit){
        if (_root) {
            _root->travPost(visit);  //后序遍历
        }
    }
    
    bool operator< (BinTree<T> const& t) {
        return _root && t._root && lt(_root, t._root);
    }
    
    bool operator==(BinTree<T> const& t){
        return _root && t._root && (_root == t._root);
    }
    
};


//高度更新
template <typename T> int BinTree<T>::updateHeight(BinNodePosi(T) x){
    return x->height = 1 + std::max( stature(x->lc), stature(x->rc) );
}

template <typename T> void BinTree<T>::updateHeightAbove(BinNodePosi(T) x){
    while (x) {
        updateHeight(x);
        x = x->parent;
    }
}

//节点插入
template <typename T> BinNodePosi(T) BinTree<T>::insertAsRoot(T const&e){
    _size = 1;
    return _root = new BinNode<T>(e);
}

template <typename T> BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const &e){
    _size++;
    x->insertAsLC(e);
    updateHeightAbove(x);
    return x -> lc;
}

template <typename T> BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const &e){
    _size++;
    x->insertAsRC(e);
    updateHeightAbove(x);
    return x -> rc;
}

//子树接入
template <typename T>
BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T> *&S) {
    //二叉树子树接入算法：将S当作节点x的左子树接入，S本身置空
    if ((x -> lc = S -> _root)) {
        x -> lc -> parent = x;
    }
    
    _size += S -> _size;
    updateHeightAbove(x);
    S->_root = NULL;
    S->_size = 0;
    
    //    release(S);
    delete S;
    
    S = NULL;
    return x;
}

template <typename T>
BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T> *&S) {
    if ((x -> rc = S -> _root)) {
        x -> rc -> parent = x;
    }
    
    _size += S -> _size;
    updateHeightAbove(x);
    S -> _root = NULL;
    S -> _size = 0;
    //    release(S);
    delete S;
    
    S = NULL;
    return x;
}


//子树删除
template <typename T>
int BinTree<T>::remove(BinNodePosi(T) x){
    //assert: x为二叉树中的合法位置
    FromParentTo(*x, this->_root) = NULL;  //切断来自父节点的指针
    updateHeightAbove(x->parent);  //更新祖先高度
    int n = removeAt(x);
    _size -= n;
    return n;
}


//删除二叉树中位置x处的节点及其后代，返回被删除节点的数值
template <typename T>
static int removeAt(BinNodePosi(T) x){
    if (!x) {
        return 0;
    }
    
    int n = 1 + removeAt(x -> lc) + removeAt(x -> rc);
    //    release(x -> data);
    
    //    release(x);
    
    x -> data = NULL;
    delete x;
    return n;
}

//子树分离
//将子树x从当前树中摘除，将其封装为一棵独立子树返回
template <typename T>
BinTree<T>* BinTree<T>::secede(BinNodePosi(T) x){
    //assert: x为二叉树中的合法位置
    FromParentTo(*x, this->_root) = NULL;
    updateHeightAbove(x -> parent);
    BinTree<T>* S = new BinTree<T>;
    S -> _root = x;
    x -> parent = NULL;
    S -> _size = x -> size();
    _size -= S -> _size;
    return S;
}


//遍历
//1.递归式遍历
//1.1 先序遍历
template <typename T,typename VST>
void travPre_R(BinNodePosi(T) x, VST& visit) {
    if (!x) {
        return;
    }
    
    visit(x -> data);
    travPre_R(x -> lc, visit);
    travPre_R(x -> rc, visit);
}

//1.2 后序遍历
template <typename T,typename VST>
void travPost_R(BinNodePosi(T) x,VST& visit) {
    if (!x) {
        return;
    }
    travPost_R(x -> lc, visit);
    travPost_R(x -> rc, visit);
    visit(x -> data);
}

//1.3 中序遍历
template <typename T,typename VST>
void travIn_R(BinNodePosi(T) x, VST& visit){
    if (!x) {
        return;
    }
    
    travIn_R(x -> lc, visit);
    visit(x -> data);
    travIn_R(x -> rc, visit);
}

//2.迭代式遍历
//2.1 先序遍历
template <typename T,typename VST>
void travPre_I1(BinNodePosi(T) x,VST& visit){
    Stack<BinNodePosi(T)> S;
    
    if (x) {
        S.push(x);
    }
    
    while (!S.empty()) {
        x = S.pop();
        visit(x -> data);
        if (HasRChild(*x)) {
            S.push(x -> rc);
        }
        
        if (HasLChild(*x)) {
            S.push(x -> lc);
        }
    }
    
}

//从当前节点出发，沿左分支不断深入，直至没有左分支的节点；沿途节点遇到后立即访问
template <typename T,typename VST>
static void visitAlongLeftBranch(BinNodePosi(T) x,VST& visit,Stack<BinNodePosi(T)>& S) {
    while (x) {
        visit(x->data);
        S.push(x -> rc);
        x = x->lc;
    }
}

template <typename T,typename VST>
void trav_Pre_I2(BinNodePosi(T) x,VST& visit) {
    Stack<BinNodePosi(T)> S;  //辅助栈
    
    while (true) {
        visitAlongLeftBranch(x, visit, S);
        if (S.empty()) {
            break;
        }
        x = S.pop();
    }
}

//2.2 中序遍历
template <typename T>
static void goAlongLeftBranch(BinNodePosi(T) x,Stack<BinNodePosi(T)>& S) {
    while (x) {
        S.push(x);
        x = x -> lc;
    }
}

template <typename T,typename VST>
void travIn_I1(BinNodePosi(T) x,VST& visit) {
    Stack<BinNodePosi(T)> S;
    while (true) {
        goAlongLeftBranch(x, S);
        if (S.empty()) {
            break;
        }
        
        x = S.pop();
        visit(x -> data);
        x = x -> rc;   //转向右子树
    }
}

template <typename T,typename VST>
void travIn_I2(BinNodePosi(T) x,VST& visit){
    Stack<BinNodePosi(T)> S;
    while (true) {
        if (x) {
            S.push(x);
            x = x -> lc;
        }else if(!S.empty()) {
            x = S.pop();
            visit(x -> data);
            x = x -> rc;
        }else{
            break;
        }
    }
}

template <typename T,typename VST>
void travIn_I3(BinNodePosi(T) x,VST& visit){
    bool backtrack = false; //前一步是否刚从右子树回溯----省去栈，仅σ(1)辅助空间
    while (true) {
        if (!backtrack && HasLChild(*x)) { //若有左子树且不是刚刚回溯，则深入遍历左子树
            x = x -> lc;
        }else{
            //否则----无左子树或刚刚回溯(相当于无左子树)
            visit(x -> data);
            
            if (HasRChild(*x)) {
                //若其右子树非空，则深入右子树继续遍历,并关闭回溯标志
                x = x -> rc;
                backtrack = false;
            }else{
                //若右子树为空，则回溯
                if (! (x = x -> succ())) {
                    break;
                }
                backtrack = true;
            }
        }
        
    }
}

//继续改进
template <typename T,typename VST>
void travIn_I4(BinNodePosi(T) x,VST& visit) {
    while (true) {
        if (HasLChild(*x)) {   //若有左子树，则深入遍历左子树
            x = x -> lc;
        }else{
            visit(x -> data); //访问当前节点，并
            
            while (!HasRChild(*x)) {
                //不断的在无右分支处，回溯至直接后继(在没有后继的末节点处，直接退出)
                if (! (x = x->succ())) {
                    return;
                }else{
                    visit(x -> data);
                }
            }
            
            //(直至有右分支处)转向非空的右子树
            x = x -> rc;
        }
        
    }
}


//2.3 后序遍历
//在以S栈顶节点为根的子树中，找到最高左侧可见叶节点
template <typename T>
static void gotoHLVFL(Stack<BinNodePosi(T)>& S) { //沿途所遇节点依次入栈
    while (BinNodePosi(T) x = S.top()) {  //自顶向下，反复检查当前节点(即栈顶)
        if (HasLChild(*x)) {  //尽可能向左
            if (HasRChild(*x)) {
                S.push(x -> rc);//若有右孩子，优先入栈
            }
            S.push(x -> lc);//然后才转至左孩子
        }else{  //实不得已
            S.push(x -> rc);  //才向右
        }
    }
    
    S.pop();  //返回之前，弹出栈顶的空节点
}

template <typename T,typename VST>
void travPost_I(BinNodePosi(T) x,VST& visit){ //二叉树的后序遍历(迭代版)
    Stack<BinNodePosi(T)> S; //辅助栈
    if (x) {
        S.push(x);   //根节点入栈
    }
    
    while (!S.empty()) {
        if (S.top() != x -> parent) { //若栈顶非当前节点之父(则必为其右兄)，此时需
            gotoHLVFL(S); //在以其右兄为根之子树中，找到HLVFL
        }
        x = S.pop();
        visit(x -> data);  //弹出栈顶(即前一节点之后继),并访问之
    }
}

#endif /* BinTree_hpp */
