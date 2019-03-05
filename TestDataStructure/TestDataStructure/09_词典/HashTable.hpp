//
//  HashTable.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/8/21.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef HashTable_hpp
#define HashTable_hpp


#include <iostream>

static size_t hashCode(char c) {
    return (size_t)c;
}

static size_t hashCode(int k) {
    return (size_t)k;
}

static size_t hashCode(long long i) {
    return (size_t)( (i >> 32) + (int)i);
}

static size_t hashCode(char s[]) {
    int h = 0;
    for (size_t n = strlen(s),i = 0; i < n; i++) {
        h = (h<<5) | (h>>27);
        h += (int)s[i];
    }
    return (size_t)h;
}


#include "Dictionary.hpp"
#include "Bitmap.h"
#include "Entry.hpp"

template <typename K,typename V>
class HashTable : public Dictionary<K, V> {
private:
    Entry<K, V> **ht;//桶数组，存放词条指针
    int M; //桶数组容量
    int N; //词条数量
    Bitmap *lazyRemoval; //懒惰删除记忆
    
#define lazilyRemoved(x) (lazyRemoval->test(x))
#define markAsRemoved(x) (lazyRemoval->set(x))
    
protected:
    int probe4Hit(const K& k); //沿关键码k对应的查找链，找到词条匹配的桶
    int probe4Free(const K& k);//沿关键码k对应的查找链，找到首个可用桶
    void rehash(); //重散列算法:扩充桶数组，保证装填因子在警戒线以下
    
public:
    HashTable(int c = 5); //创建一个容量不小于c的散列表
    ~HashTable(); //释放桶数组及其中各(非空)元素所指向的词条
    int size() const {
        return N;
    }//当前的词条数目
    
    bool put(K,V); //插入(禁止雷同词条，故可能失败)
    V* get(K k);//读取
    bool remove(K k); //删除
    
};

int primeNLT(int c,int n,char *file) { //根据file文件中的记录，在[c,n)内取最小的素数
    Bitmap B(file, n); //file已经按位图格式，记录了n以内的所有素数，因此只要
    while (c < n) {     //从c开始，逐位的
        if (B.test(c)) { //测试，即可
            c++;
        }else{
            return c;  //返回首个发现的素数
        }
    }
    return c; //若没有这样的素数，返回n(实用中不能如此简化处理)
}

template <typename K,typename V>
HashTable<K, V>::HashTable(int c) {
    M = primeNLT(c, 1048576, "prime-bitmap.text"); //不小于c的素数M
    N = 0;
    ht = new Entry<K, V> *[M];//开辟桶数量(还需核对申请成功)，初始装填因子为N/M=0%
    memset(ht, 0, sizeof(Entry<K, V>*) * M); //初始化各桶
    lazyRemoval = new Bitmap(M);//懒惰删除标记比特图
    
}

template <typename K,typename V> HashTable<K, V>::~HashTable() { //析构前释放桶数组以及非空词条
    for (int i = 0; i< M; i++) { //逐一检查各桶
        if (ht[i]) {
            delete ht[i]; //释放非空的桶
        }
    }
    
    delete ht; //释放桶数组
    delete lazyRemoval; //释放懒惰删除标记
}

template <typename K,typename V> V* HashTable<K,V>::get(K k) { //散列表词条查找算法
    int r = probe4Hit(k);
    return ht[r] ? &(ht[r]->value) : NULL; //禁止词条的k值雷同
}


template <typename K,typename V>int HashTable<K,V>::probe4Hit(const K &k) {
    int r = hashCode(k) % M; //从起始桶(按除余法确定)出发
    while ( (ht[r] && (k != ht[r]->key)) || (!ht[r] && lazilyRemoved(r))) {
        r = (r+1)%M; //沿查找链线性试探:跳过所有冲突的桶，以及带懒惰删除标记的桶
    }
    return r; //调用者根据ht[r]是否为空，即可判断查找是否成功
}


template <typename K,typename V>bool HashTable<K,V>::remove(K k) { //散列表词条删除算法
    int r = probe4Hit(k);
    if (!ht[r]) {
        return false; //对应词条不存在时，无法删除
    }
    
    delete ht[r];
    ht[r] = NULL;
    markAsRemoved(r);
    N--;
    return true; //否则释放桶中词条，设置懒惰删除标记，并更新词条总数
}

template <typename K,typename V>bool HashTable<K,V>::put(K k, V v) {//散列表词条插入
    if (ht[probe4Hit(k)]) {
        return false; //雷同元素不必重复插入
    }
    
    int r = probe4Free(k); //为新词条找个空桶(只要装填因子控制得当，必然成功)
    ht[r] = new Entry<K, V>(k,v);
    ++N; //插入(注意:懒惰删除标记无需复位)
    if (N * 2 > M) {
        rehash(); //装填因子高于50%后重散列
    }
    return true;
}

template <typename K,typename V> int HashTable<K,V>::probe4Free(const K &k) {
    int r = hashCode(k) % M;//从起始桶(按除余法确定)出发
    while (ht[r]) {
        r = (r+1) % M; //沿查找链逐桶试探，直到首个空桶(无论是否带有懒惰删除标记)
    }
    return r; //为保证空桶总能找到，装填因子及散列表长需要合理设置
}

template <typename K,typename V> void HashTable<K,V>::rehash() {
    int old_capacity = M;
    Entry<K, V> **old_ht = ht;
    
    M = primeNLT(2*M, 1048576, "prime-1048576-bitmap.text");//容量至少加倍
    N = 0;
    ht = new Entry<K, V>*[M];
    memset(ht, 0, sizeof(Entry<K, V> *) * M); //新桶数组
    
    delete lazyRemoval;
    lazyRemoval = new Bitmap(M); //新开懒惰删除标记比特图
    
    for (int i = 0; i < old_capacity; i++) { //扫描原桶数组
        if (old_ht[i]) { //将非空桶中的词条逐一
            put(old_ht[i]->key, old_ht[i]->value); //插入至新的桶数组
        }
    }
    
    delete old_ht; //释放原桶数组---由于其中原先存放的词条均已转移，故只需释放桶数组本身
    
    
}




#endif /* HashTable_hpp */
