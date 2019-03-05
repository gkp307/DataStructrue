//
//  List.cpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#include "List.hpp"

#include <algorithm>

template <typename T> void List<T>::init() {
    header = new ListNode<T>;
    trailer = new ListNode<T>;
    header -> succ = trailer;
    header -> pred = NULL;
    trailer -> pred = header;
    trailer -> succ = NULL;
    _size = 0;
}

//重载下标运算符
template <typename T>
T& List<T>::operator[](Rank r)const { //assert: 0<=r<size
    ListNodePosi(T) p = first();
    while (0 < r--) {
        p = p -> succ;
    }
    return p -> data;
}

//在无序列表内节点p的n个前驱中，找到等于e的最后者
template <typename T>
ListNodePosi(T) List<T>::find(const T &e, int n, ListNodePosi(T) p)const {
    while (0 < n--) {
        if (e == (p = p -> pred) -> data) {
            return p;
        }
    }
    return NULL;
}

template <typename T> ListNodePosi(T) List<T>::insertAsFirst(T const&e) {
    _size++;
    return header -> insertAsSucc(e);
}

template <typename T> ListNodePosi(T) List<T>::insertAsLast(T const&e){
    _size++;
    return trailer -> insertAsPred(e);
}

//After
template <typename T> ListNodePosi(T) List<T>::insertA(ListNode<T> *p, T const&e){
    _size++;
    return p -> insertAsSucc(e);
}

//Before
template <typename T> ListNodePosi(T) List<T>::insertB(ListNode<T> *p, T const&e){
    _size++;
    return p -> insertAsPred(e);
}

//列表内部方法：复制列表中自位置p起的n项
template <typename T>
void List<T>::copyNodes(ListNodePosi(T) p, int n){
    init();
    while (n--) {
        insertAsLast(p->data);
        p = p -> succ;
    }
}

template <typename T>
List<T>::List(ListNodePosi(T) p,int n) {
    copyNodes(p, n);
}

template <typename T>
List<T>::List(List<T> const& L) {
    copyNodes(L.first(), L._size);
}

//复制L中自第r项起的n项 (assert: r+n <= L._size)
template <typename T>
List<T>::List(List<T> const& L,Rank r,int n){
    copyNodes(L[r], n);
}

//在列表中删除指定节点p
template <typename T>
T List<T>::remove(ListNodePosi(T) p){
    T e = p -> data;
    p -> pred -> succ = p -> succ;
    p -> succ -> pred = p -> pred;
    delete p;
    _size--;
    return e;
}

template <typename T> int List<T>::clear() {
    int oldSize = _size;
    while (0 < _size) {
        remove(header -> succ);
    }
    return oldSize;
}

template <typename T> List<T>::~List(){
    clear();
    delete header;
    delete trailer;
}

//剔除无序列表中的重复节点
template <typename T> int List<T>::deduplicate() {
    if (_size < 2) { //平凡列表自然无重复
        return 0;
    }
    
    int oldSize = _size;
    ListNodePosi(T) p = header;
    Rank r = 0;
    
    while (trailer != (p = p ->succ)) {
        ListNodePosi(T) q = find(p -> data, r, p);
        q ? remove(q) : r++;
    }//assert:循环过程中的任意时刻，p的所有前驱互不相同
    return oldSize - _size;
}

template <typename T>
void List<T>::traverse(void (*visit)(T &)) {
    for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ) {
        visit(p -> data);
    }
}

template <typename T> template <typename VST>
void List<T>::traverse(VST &visit) {
    for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ) {
        visit(p -> data);
    }
}

#pragma mark - 有序列表
//成批删除重复元素，效率更高
template <typename T> int List<T>::uniquify() {
    if (_size < 2) {
        return 0;
    }
    
    int oldSize = _size;
    ListNodePosi(T) p = first();
    ListNodePosi(T) q;
    
    while (trailer != (q = p -> succ)) {
        if (p -> data != q -> data) {
            p = q;
        }else{
            remove(q);
        }
    }
    
    return oldSize - _size;
}

//在有序列表内节点p的n个前驱中，找到不大于e的最后者
template <typename T>
ListNodePosi(T) List<T>::search(T const&e, int n, ListNodePosi(T) p) const{
    //assert: 0 <= n <= rank(p) < _size
    while (0 <= n--) {
        if ((  (p = p -> pred)->data  ) <= e) {
            break;
        }
    }
    
    return p;
}

#pragma mark - 排序
template <typename T>
void List<T>::sort(ListNodePosi(T) p, int n){
    switch (rand() % 3) {
        case 1: insertionSort(p, n);break;
        case 2: selectionSort(p, n);break;
        default: mergeSort(p, n);break;
    }
}


//插入排序
//对起始于位置p的n个元素排序
template <typename T>
void List<T>::insertionSort(ListNodePosi(T) p, int n){
    for (int r = 0; r < n; r++) {
        //search是在前驱中查找
        insertA(search(p->data, r, p), p->data); //查找适当的位置并插入
        p = p -> succ;
        remove(p->pred); //转向下一节点
    }
}

//从起始于位置p的n个元素中选出最大者
template <typename T>
ListNodePosi(T) List<T>::selectMax(ListNodePosi(T) p, int n){
    ListNodePosi(T) max = p;
    for (ListNodePosi(T) cur = p; 1 < n; n--) {
        
        if (! lt( (cur=cur->succ)->data, max->data )) {
            max = cur;
        }
    }
    return max;
}

//选择排序,对起始于位置p的n个元素排序
template <typename T> void List<T>::selectionSort(ListNodePosi(T) p, int n){
    //valid(p) && rank(p)+n<=size
    
    ListNodePosi(T) head = p -> pred;
    ListNodePosi(T) tail = p;
    for (int i = 0; i < n; i++) { //待排序区间为(head, tail)
        tail = tail -> succ;
    }
    
    while (1 < n) {
        //找出最大者(歧义时后者优先)
        ListNodePosi(T) max = selectMax(head -> succ, n);
        //将其移至无序区间末尾(作为有序区间新的首元素)
        insertB(tail, remove(max));
        
        tail = tail -> pred;
        n--;
    }
}

/*
 //选择排序(更高效,将insertB修改为swap)
 template <typename T> void List<T>::selectionSort(ListNodePosi(T) p, int n){
    //valid(p) && rank(p)+n<=size
 
    ListNodePosi(T) head = p -> pred;
    ListNodePosi(T) tail = p;
    for (int i = 0; i < n; i++) { //待排序区间为(head, tail)
        tail = tail -> succ;
    }
 
    while (1 < n) {
        //找出最大者(歧义时后者优先)
        ListNodePosi(T) max = selectMax(head -> succ, n);
        //将其移至无序区间末尾(作为有序区间新的首元素)
        //        insertB(tail, remove(max));
        std::swap(tail->pred->data, max->data);
 
        tail = tail -> pred;
        n--;
    }
 }
 */


//有序列表的归并：当前列表中自p起的n个元素，与列表L中自q起的m个元素归并
template <typename T>
void List<T>::merge(ListNodePosi(T) &p,int n,List<T> &L,ListNodePosi(T) q,int m){
    //注意：在归并排序之类的场合，有可能this==L && rank(p)+n = rank(q)
    
    //借助前驱（可能是header），以便返回前...
    ListNodePosi(T) pp = p -> pred;
    while (0 < m) {  //在q尚未移出区间之前
        if ( (0 < n) && (p -> data <= q -> data)) { //若p仍在区间内且v(p) <= v(q)
            //p归入合并的列表，并替换为其直接后继
            if (q == (p = p -> succ)) {
                break;
            }
            n--;
        }else{
            //若p已超出右界或v(q) < v(p)
            insertB(p, L.remove( (q = q -> succ) -> pred)); //将q移至p之前
            m--;
        }
    }
    
    p = pp -> succ;//确定归并后区间的(新)起点
}

//列表的归并排序算法：对起始于位置p的n个元素排序
template <typename T>
void List<T>::mergeSort(ListNodePosi(T) &p, int n){
    if (n < 2) {
        return;
    }
    
    int m = n >> 1;  //以中点为界,均分列表
    ListNodePosi(T) q = p;
    for (int i=0; i < m; i++) {
        q = q -> succ;
    }
    
    //对前后子列表分别排序
    mergeSort(p, m);
    mergeSort(q, n-m);
    merge(p, m, *this, q, n-m); //归并
}//注意:排序后，p依然指向归并后区间的(新)起点


//倒置
/*
 template <typename T> void List<T>::reverse() {
    if (_size < 2) {
        return;
    }
 
    ListNodePosi(T) p;
    ListNodePosi(T) q;
 
    for (p = header,q = p -> succ; p != trailer; p = q,q = p -> succ) {
        p -> pred = q;
    }
 
    trailer -> pred = NULL;
 
    for (p = header,q = p -> pred; p != trailer; p = q,q = p -> pred) {
        q -> succ = p;
    }
 
    header -> succ = NULL;
    std::swap(header, trailer);  //头、尾节点互换
 
 }
 
 
 template <typename T> void List<T>::reverse() {
    if (_size < 2) {
        return;
    }
 
    for (ListNodePosi(T) p = header; p; p = p -> pred) {
        std::swap(p -> pred, p -> succ);
    }
 
    std::swap(header, trailer);
 }
 
 */

template <typename T> void List<T>::reverse() {
    ListNodePosi(T) p = header;
    ListNodePosi(T) q = trailer;
    
    for (int i = 1; i < _size; i += 2) {
        std::swap( (p = p -> succ) -> data, (q = q -> pred) -> data); //交换对称节点的数据项
    }
    
}
