//
//  Skiplist.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef Skiplist_hpp
#define Skiplist_hpp


#include "Quadlist.hpp"
#include "Dictionary.hpp"
#include "Entry.hpp"
#include "List.hpp"


//跳转表
template <typename K,typename V> class Skiplist : public Dictionary<K, V>, public List<Quadlist<Entry<K,V>> *> {
protected:
    bool skipSearch(ListNode<Quadlist<Entry<K,V>>* > *qlist, QuadlistNode<Entry<K,V>> * &p,K& k);
    
public:

    int size() const {
        return this->empty() ? 0 : this->last()->data->size(); //底层Quadlist的规模
    }
    int level() {
        return List<Quadlist<Entry<K,V>> *>::size();  //层高==#Quadlist,不一定要开放
    }
    bool put(K,V); //插入(注意与map有区别----Skiplist允许词条重复，故必然成功)
    V* get(K k); //读取
    bool remove(K k); //删除
    
    Skiplist() {}
    
    
};


template <typename K,typename V> V* Skiplist<K,V>::get(K k) {
    if (this->empty()) {
        return NULL;
    }
    
    ListNode<Quadlist<Entry<K, V>> *> *qlist = this->first();
    QuadlistNode<Entry<K, V>> *p = qlist->data->first(); //首节点开始
    return skipSearch(qlist, p, k) ? &(p -> entry.value) : NULL;
}

template <typename K,typename V> bool Skiplist<K,V>::skipSearch(ListNode<Quadlist<Entry<K, V> > *> *qlist, QuadlistNode<Entry<K, V> > *&p, K &k) {
    //从指定层qlist的首节点p出发，向右，向下查找目标关键码k
    while (true) { //在每一层
        while (p->succ && (p->entry.key <= k)) { //从前向后查找
            p = p->succ; //知道出现更大的key或溢出至trailer
        }
        
        p = p -> pred; //此时倒回一步，即可判断是否命中，否则转入下一层
        if (p -> pred && (k == p->entry.key)) {
            return true;
        }
        
        qlist = qlist -> succ;
        if (!qlist -> succ) {
            return false;  //若已到穿透底层，则意味着失败
        }
        
        p = (p -> pred) ? p -> below : qlist -> data -> first(); //否则转至当前塔的下一节点
    }
}

template <typename K,typename V> bool Skiplist<K,V>::put(K k, V v) {
    Entry<K, V> e = Entry<K, V>(k, v); //待插入的词条(将被随机的插入多个副本)
    if (this->empty()) {
        this->insertAsFirst(new Quadlist<Entry<K, V>>); //插入首个Entry
    }
    
    ListNode<Quadlist<Entry<K, V>> *> *qlist = this->first(); //从顶层四联表的首节点出发
    QuadlistNode<Entry<K, V>> *p = qlist->data->first();
    
    if (skipSearch(qlist, p, k)) {
        while (p->below) { //查找适当的插入位置(不大于关键码k的最后一个节点p)
            p = p -> below; //若已有雷同词条，则需强制转至塔底
        }
    }
    
    qlist = this->last(); //以下，紧邻与p的右侧，一座新塔将自底而上逐层生长
    
    QuadlistNode<Entry<K, V>> *b = qlist->data->insertAfterAbove(e, p);
    
    while (rand() & 1) {  //经投掷硬币，若确定新塔需要在长高一层，则找出不低于此高度的最近前驱
        while (qlist->data->valid(p) && !p->above) {
            p = p -> pred;
        }
        
        if (!qlist->data->valid(p)) {
            //若该前驱是header
            if (qlist == this->first()) {
                //且当前已是最顶层，则意味着必须首先创建新的一层，然后将p转至上一层Skiplist的header
                this->insertAsFirst(new Quadlist<Entry<K, V>>);
            }
            
            p = qlist->pred->data->first()->pred;
        }else{
            //否则可径自 将p提高至该高度
            p = p -> above;
        }
        
        qlist = qlist -> pred;  //上升一层，并在该层
        b = qlist ->data->insertAfterAbove(e, p, b); //将新节点插入p之后、b之上
    }
    
    return true;
    
}

//跳转表词条删除算法
template <typename K,typename V> bool Skiplist<K,V>::remove(K k) {
    if (this->empty()) {
        return false; //空表情况
    }
    
    ListNode<Quadlist<Entry<K, V>> *> *qlist = this->first();
    QuadlistNode<Entry<K, V>> *p = qlist->data->first();
    
    if (!skipSearch(qlist, p, k)) {
        return false;  //目标词条不存在，直接返回
    }
    
    do{
        //若目标词条不存在，则逐层拆除与之对应的塔
        QuadlistNode<Entry<K, V>> *lower = p->below;
        qlist->data->remove(p);
        p = lower;            //删除当前层节点，再
        qlist = qlist -> succ;  //转入下一层
        
    }while(qlist->succ);
    
    while (!this->empty() && this->first()->data->empty()) {
        //逐一的清除已可能不含词条的顶层Quadlist
        List<Quadlist<Entry<K,V>> *>::remove(this->first());
    }
    
    return true;  //删除操作成功完成
}

#endif /* Skiplist_hpp */
