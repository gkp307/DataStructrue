//
//  List.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef List_hpp
#define List_hpp

#include "ListNode.hpp"
#include <iostream>

template <typename T> class List{
private:
    int _size;
    ListNodePosi(T) header;
    ListNodePosi(T) trailer;
    
protected:
    void init(); //列表创建时的初始化
    int clear(); //清楚所有节点
    void copyNodes(ListNodePosi(T),int);  //复制列表中自位置p起的n项
    void merge(ListNodePosi(T)&, int, List<T>&, ListNodePosi(T), int); //归并
    void mergeSort(ListNodePosi(T)&,int);
    void selectionSort(ListNodePosi(T),int);
    void insertionSort(ListNodePosi(T),int);

    
public:
    List() {
        init();
    }
    
    List(List<T> const& L);
    List(List<T> const& L,Rank r,int n);
    List(ListNodePosi(T) p,int n);
    
    ~List();
    
    //只读访问接口
    Rank size() const {
        return _size;
    }
    
    bool empty() const {
        return _size <= 0;
    }
    
    T& operator[](Rank r) const;
    ListNodePosi(T) first() const { //首节点位置
        return header -> succ;
    }
    
    ListNodePosi(T) last() const { //末节点位置
        return trailer -> pred;
    }
    
    bool valid(ListNodePosi(T) p) {//判断位置p是否对外合法
        return p && (trailer != p) && (header != p);
    }
    
    int disordered() const;
    
    ListNodePosi(T) find(T const& e)const {
        return find(e, _size, trailer);
    }
    
    ListNodePosi(T) find(T const&e,int n,ListNodePosi(T) p) const;
    
    ListNodePosi(T) search(T const& e) const { //有序列表查找
        return search(e, _size, trailer);
    }
    
    ListNodePosi(T) search(T const& e,int n,ListNodePosi(T) p) const;
    
    ListNodePosi(T) selectMax(ListNodePosi(T) p,int n);
    ListNodePosi(T) selectMax() {
        return selectMax(header->succ, _size);
    }
    
    //可写访问接口
    ListNodePosi(T) insertAsFirst(T const& e);
    ListNodePosi(T) insertAsLast(T const& e);
    ListNodePosi(T) insertA(ListNodePosi(T) p, T const& e); //将e作为p的后继插入
    ListNodePosi(T) insertB(ListNodePosi(T) p,T const& e); //将e作为p的前驱插入
    
    T remove(ListNodePosi(T) p);
    
    void merge(List<T>& L){ //全列表归并
        merge(first(), size(), L, L.first(), L._size);
    }
    
    void sort(ListNodePosi(T) p,int n);  //列表区间排序
    void sort() {  //列表整体排序
        sort(first(), _size);
    }
    
    int deduplicate();         //无序去重
    int uniquify();            //有序去重
    void reverse();            //前后倒置
    
    //遍历
    void traverse(void (*)(T&));
    template <typename VST> void traverse(VST&);
};

#endif /* List_hpp */
