//
//  Vector.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef Vector_hpp
#define Vector_hpp

#include <iostream>
#include "Fabnacci.hpp"


typedef int Rank;             //秩
#define DEFAULT_CAPACITY (3)  //默认的初始容量


template <typename T> class Vector{
protected:
    //规模，容量，数据区
    Rank _size;
    int _capacity;
    T* _elem;
    
    void copyFrom(T const* A,Rank lo,Rank hi);  //复制数组区间A[lo,hi)
    void expand(); //空间不足时扩容
    void shrink(); //装填因子过小时压缩
    bool bubble(Rank lo,Rank hi); //扫描交换
    void bubbleSort(Rank lo,Rank hi);  //冒泡排序算法
    Rank max(Rank lo,Rank hi); //选取最大元素
    void selectionSort(Rank lo,Rank hi); //选择排序算法
    void merge(Rank lo,Rank mi,Rank hi); //归并算法
    void mergeSort(Rank lo,Rank hi);  //归并排序算法
    Rank partition(Rank lo,Rank hi); //轴点构造算法
    void quickSort(Rank lo,Rank hi); //快速排序算法
    void heapSort(Rank lo,Rank hi); //堆排序
    
public:
    //构造函数
    Vector(int c = DEFAULT_CAPACITY, int s = 0,T v = 0) {
        _elem = new T[_capacity = c];
        for (_size = 0; _size < s; _elem[_size++] = v);
    }
    
    Vector(T const* A,Rank n){  //数组整体复制
        copyFrom(A, 0, n);
    }
    
    Vector(T const* A,Rank lo,Rank hi){  //区间
        copyFrom(A, lo, hi);
    }
    
    Vector(Vector<T> const& v) {
        copyFrom(v._elem, 0, v._size);
    }
    
    Vector(Vector<T> const& v,Rank lo,Rank hi){
        copyFrom(v._elem, lo, hi);
    }
    
    ~Vector(){
        delete [] _elem;  //释放内存空间
    }
    
    //只读访问接口
    //规模
    Rank size() const {
        return _size;
    }
    
    //判空
    bool empty() const {
        return !_size;
    }
    
    int disordered() const;  //判断向量是否已排序
    
    Rank find(T const& e){      //无序向量整体查找
        return find(e, 0, _size);
    }
    
    Rank find(T const& e,Rank lo,Rank hi) const; //无序向量区间查找
    
    Rank search(T const& e) const {
        return (0 >= _size) ? -1 : search(e, 0, _size);
    }
    
    Rank search(T const& e, Rank lo, Rank hi) const;   //有序向量区间查找
    
    //可写访问接口
    T& operator[](Rank r) const;
    Vector<T> & operator=(Vector<T> const&);
    T remove(Rank r);
    int remove(Rank lo,Rank hi);
    Rank insert(Rank r,T const& e);
    Rank insert(T const& e) {
        return insert(_size, e);
    }
    
    void sort(Rank lo,Rank hi);  //对[lo,hi]排序
    void sort() {
        sort(0, _size);
    }
    
    void unsort(Rank lo,Rank hi); //对[lo,hi]置乱
    void unsort() {
        unsort(0, _size);
    }
    
    int deduplicate();  //无序去重
    int uniquify();  //有序去重
    //遍历
    void traverse(void (*)(T&)); //遍历(使用函数指针，只读或局部性修改)
    template <typename VST> void traverse(VST&);  //遍历(使用函数对象，可全局性修改)
    
};


#endif /* Vector_hpp */
