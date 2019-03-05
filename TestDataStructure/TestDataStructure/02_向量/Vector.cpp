//
//  Vector.cpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#include "Vector.hpp"

template <typename T>
void Vector<T>::copyFrom(T const* A,Rank lo,Rank hi) {
    _elem = new T[_capacity = 2 * (hi-lo)];
    _size = 0;
    while (lo < hi) {
        _elem[_size++] = A[lo++]; //复制至_elem[0,hi-lo)
    }
}

//向量空间不足时扩容
template <typename T> void Vector<T>::expand() {
    if (_size < _capacity) {
        return;
    }
    
    if (_capacity < DEFAULT_CAPACITY) {
        _capacity = DEFAULT_CAPACITY;  //不低于最低容量
    }
    
    T *oldElem = _elem;
    _elem = new T[_capacity <<= 1];  //容量加倍
    for (int i = 0; i < _size; i++) {
        _elem[i] = oldElem[i];
    }
    
    delete [] oldElem;  //释放原空间
}

//装填因子过小时压缩向量所占空间
template <typename T> void Vector<T>::shrink() {
    if (_capacity < DEFAULT_CAPACITY << 1) {
        return;      //不致收缩到DEFAULT_CAPACITY以下
    }
    
    if (_size << 2 > _capacity) {
        return;      //以25%为界
    }
    
    T *oldElem = _elem;
    _elem = new T[_capacity >>= 1];  //容量减半
    for (int i = 0; i < _size; i++) {
        _elem[i] = oldElem[i];
    }
    delete [] oldElem; //释放原空间
}


template <typename T> Vector<T>& Vector<T>::operator=(const Vector<T> &V){
    if (_elem) {
        delete [] _elem;  //释放原有内容
    }
    copyFrom(V._elem, 0, V.size());  //整体复制
    return *this;
}

template <typename T> T& Vector<T>::operator[](Rank r) const {
    return _elem[r]; //assert: 0 <= r < _size
}

//等概率随机置乱区间[lo,hi)
template <typename T> void Vector<T>::unsort(Rank lo, Rank hi) {
    T* V = _elem+lo;
    for (Rank i = hi - lo; i > 0; i--) {
        std::swap(V[i-1], V[rand() % i]);
    }
}

template <typename T> Rank Vector<T>::find(T const&e, Rank lo, Rank hi) const {
    while ( (lo < hi--) && (e != _elem[hi]) );
    
    return hi;  //若hi < lo,则意味着失败；否则hi即命中元素的秩
}

template <typename T> Rank Vector<T>::insert(Rank r, const T &e) {
    expand();  //若有必要，扩容
    for (int i = _size; i > r; i--) {
        _elem[i] = _elem[i-1];
    }
    _elem[r] = e;
    _size++;
    return r;
}

template <typename T> int Vector<T>::remove(Rank lo, Rank hi) {
    if (lo == hi) {
        return 0;   //出于效率考虑，单独处理退化情况，比如remove(0,0)
    }
    
    while ( hi < _size) {
        _elem[lo++] = _elem[hi++];
    }
    
    _size = lo;
    shrink();
    return hi - lo;
}

//单元素删除
template <typename T> T Vector<T>::remove(Rank r) { //0 <= r < _size
    T e = _elem[r];
    remove(r, r+1);
    return e;
}

//删除无序向量中的重复元素(高效版)
template <typename T> int Vector<T>::deduplicate() {
    int oldSize = _size;
    Rank i = 1;
    while ( i < _size)
        ( find(_elem[i], 0, i) < 0) ? i++ : remove(i);
    
    return oldSize - _size;
}

//借助函数指针机制
template <typename T> void Vector<T>::traverse(void (*visit)(T&)) {
    for (int i = 0; i < _size; i++) {
        visit(_elem[i]);   //遍历向量
    }
}

template <typename T> template <typename VST>
void Vector<T>::traverse(VST &visit){
    for (int i = 0; i < _size; i++) {
        visit(_elem[i]);
    }
}


//返回向量中逆序相邻元素对的总数
template <typename T> int Vector<T>::disordered() const {
    int n = 0;
    for (int i = 0; i < _size; i++) {
        if (_elem[i-1] > _elem[i]) {
            n++;
        }
    }
    
    return n;
}


/*
//有序向量重复元素剔除算法(低效版)
template <typename T> int Vector<T>::uniquify() {
     int oldSize = _size;
     int i = 1;
     while (i < _size) {
         _elem[i-1] == _elem[i] ? remove(i) : i++;
     }
     return oldSize - _size;
 }*/

//有序向量重复元素剔除算法(高效版)
template <typename T> int Vector<T>::uniquify() {
    Rank i = 0;
    Rank j = 0;
    while (++j < _size) {
        if (_elem[i] != _elem[j]) {
            _elem[++i] = _elem[j];
        }
    }
    
    _size = ++i;
    shrink();
    return j - i;
}

//二分查找算法(版本A)
template <typename T> static Rank binSearch(T *A,T const& e,Rank lo,Rank hi){
    while (lo < hi) {
        Rank mi = (lo + hi) >> 1;
        if (e < A[mi]) {
            hi = mi;
        }else if (A[mi] < e) {
            lo = mi + 1;
        }else{
            return mi;
        }
    }//成功查找可以提前终止
    
    return -1;  //查找失败
}

//二分查找算法(版本B)
template <typename T> static Rank binSearchB(T *A,T const& e,Rank lo,Rank hi){
    while (1 < hi- lo) {
        Rank mi = (lo + hi) >> 1;
        (e < A[mi]) ? hi = mi : lo = mi;
    }
    return ( e == A[lo]) ? lo : -1; //查找成功时返回对应的秩，否则统一返回-1
}

//二分查找算法(版本C)
template <typename T> static Rank binSearchC(T *A,T const& e,Rank lo,Rank hi){
    while (lo < hi) {
        Rank mi = (lo + hi) >> 1;
        (e < A[mi]) ? hi = mi : lo = mi+1;
    }
    return --lo;
}//有多个命中元素时，总能保证返回秩最大者；查找失败时，能够返回失败的位置

//fabnacci查找算法
template <typename T> static Rank fibSearch(T *A,T const& e,Rank lo,Rank hi){
    Fib fib(hi-lo);
    
    while (lo < hi) {
        while ( hi - lo > fib.get()) {
            fib.prev();
        }
        
        Rank mi = lo + fib.get() - 1;
        if (e < A[mi]) {
            hi = mi;
        }else if(A[mi] < e) {
            lo = mi + 1;
        }else{
            return mi;
        }
    }
    
    return -1;
}


//在有序向量的区间[lo,hi)内，确定不大于e的最后一个节点的秩
template <typename T> Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const {
    return (rand() % 2) ? binSearch(_elem, e, lo, hi) : fibSearch(_elem, e, lo, hi);
}


//冒泡排序
//一趟扫描交换
template <typename T> bool Vector<T>::bubble(Rank lo, Rank hi) {
    bool sorted = true;
    while (++lo < hi) {
        if (_elem[lo-1] > _elem[lo]) {
            sorted = false;
            std::swap(_elem[lo-1], _elem[lo]);
        }
    }
    
    return sorted;
}

template <typename T> void Vector<T>::bubbleSort(Rank lo, Rank hi){
    while (!bubble(lo, hi--)); //逐趟做扫描交换，直至全序
}

/*
//改进的冒泡排序
template <typename T> void Vector<T>::bubbleSort(Rank lo, Rank hi) {
    //assert: 0 <= lo <hi <= size
    while (lo < (hi = bubble(lo, hi)));   //逐趟做扫描交换，直至全序
}

template <typename T> Rank Vector<T>::bubble(Rank lo, Rank hi) {
    Rank last = lo;   //最右侧的逆序对初始化为[lo-1,lo]
    
    while (++lo < hi) {  //自左向右，逐一检查各对相邻元素
        if (_elem[lo-1] > _elem[lo]) {  //若逆序，则
            last = lo;    //更新最右侧你许对位置记录，并
            std::swap(_elem[lo-1], _elem[lo]); //通过交换使局部有序
        }
        
    }
    return last;
}
*/

//选择排序
template <typename T>
void Vector<T>::selectionSort(Rank lo,Rank hi) {
    while (lo < --hi) {
        std::swap(_elem[max(lo, hi)], _elem[hi]);
    }
}

template <typename T>
Rank Vector<T>::max(Rank lo, Rank hi) { //在[lo,hi]中找出最大者
    Rank mx = hi;
    while (lo < hi--) {
        if (_elem[hi] > _elem[mx]) {
            mx = hi;
        }
    }
    return mx;
}

//归并排序
template <typename T> void Vector<T>::mergeSort(Rank lo, Rank hi) {
    if (hi - lo < 2) {//单元素区间自然有序，否则...
        return;
    }
    
    int mi = ( lo + hi) / 2;   //以中点为界
    mergeSort(lo, mi);
    mergeSort(mi, hi);   //分别排序
    if (_elem[mi-1] > _elem[mi]) {
        merge(lo, mi, hi);  //归并
    }
    
}

//有序向量的归并
//各自有序的向量[lo,mi)和[mi,hi)
template <typename T> void Vector<T>::merge(Rank lo, Rank mi, Rank hi) {
    T *A = _elem + lo; //合并后的向量A[0,hi-lo) = _elem[lo,hi)
    int lb = mi - lo;
    T *B = new T[lb];   //前子向量B[0,lb) = _elem[lo,mi)
    
    for (Rank i = 0; i < lb; B[i] = A[i++]);  //复制前子向量
    
    int lc = hi - mi;
    T *C = _elem + mi;
    
    for (Rank i = 0,j = 0,k = 0; (j < lb) || (k < lc) ; ) { //B[j]和C[k]中的小者续至A末尾
        // !(k < lc)表示C数组合并完了，B数组还没合并完
        if ( (j < lb) && ( !(k < lc) || (B[j] <= C[k] ) )) {
            A[i++] = B[j++];
        }
        if ( (k < lc) && ( !(j < lb) || (C[k] < B[j]))) {
            A[i++] = C[k++];
        }
    }
    
    delete [] B; //释放临时空间
}

//堆排序

//快速排序
template <typename T> void Vector<T>::quickSort(Rank lo, Rank hi) {
    if (hi - lo < 2) {
        return;
    }
    
    Rank mi = partition(lo, hi-1); //在[lo,hi-1]内构造轴点
    quickSort(lo, mi);
    quickSort(mi+1, hi);
}

/*
 //轴点构造算法
 template <typename T>
 Rank Vector<T>::partition(Rank lo, Rank hi) { //版本A:基本形式
     std::swap(_elem[lo], _elem[lo+rand() % (hi-lo+1)]); //任选一个元素与首元素交换
     T pivot = _elem[lo]; //以首元素为候选轴点-----一经以上交换，等效于随机选取
 
     while (lo < hi) {
         while ( (lo < hi) && (pivot <= _elem[hi]) ) {
             hi--;
         }
 
         _elem[lo] = _elem[hi];
 
         while ( (lo < hi) && (this->_elem[lo] <= pivot) ) {
             lo++;
         }
 
         _elem[hi] = _elem[lo];
     }
 
     _elem[lo] = pivot;
     return lo;
 }
 */


//轴点构造算法:通过调整元素位置构造区间[lo,hi]的轴点，并返回其秩
template <typename T>
Rank Vector<T>::partition(Rank lo, Rank hi) { //版本B：可优化处理多个关键码雷同的退化情况
    std::swap(_elem[lo], _elem[lo+rand()%(hi-lo+1)]);
    T pivot = _elem[lo];
    
    while (lo < hi) {
        while (lo < hi) {
            if (pivot < _elem[hi]) {
                hi--;
            }else{
                _elem[lo++] = _elem[hi];
                break;
            }
        }
        
        while (lo < hi) {
            if (_elem[lo] < pivot) {
                lo++;
            }else{
                _elem[hi--] = _elem[lo];
                break;
            }
        }
    }
    
    _elem[lo] = pivot;
    return lo;
}


/*
//通过调整元素位置构造区间[lo,hi]的轴点，并返回其秩
//版本C(不稳定)
template <typename T>
Rank Vector<T>::partition(Rank lo, Rank hi) {
    
    std::swap(_elem[lo], _elem[lo+ rand() % (hi-lo+1)]);
    T pivot = _elem[lo];
    
    int mi = lo;
    
    for (int k = lo+1; k <= hi; k++) {
        if (_elem[k] < pivot) {
            std::swap(_elem[++mi], _elem[k]);
        }
    }
    
    std::swap(_elem[lo], _elem[mi]);
    return mi;
    
}
*/

template <typename T> void Vector<T>::sort(Rank lo, Rank hi) {
    switch (rand() % 5) {
        case 1: bubbleSort(lo, hi); break;     //冒泡排序
        case 2: selectionSort(lo, hi); break;  //选择排序
        case 3: mergeSort(lo, hi); break;      //归并排序
        case 4: heapSort(lo, hi); break;       //堆排序
        default: quickSort(lo, hi); break;     //快速排序
    }
}
