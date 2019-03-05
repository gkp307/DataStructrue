//
//  Entry.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef Entry_hpp
#define Entry_hpp

#include <iostream>

//词条模版类
template <typename K,typename V> struct Entry {
    K key;
    V value;
    
    Entry(K k = K(), V v = V()) : key(k), value(v) {}
    
    Entry(Entry<K,V> const& e) : key(e.key), value(e.value) {}
    
    bool operator<(Entry<K,V> const& e) {
        return key < e.key;
    }
    
    bool operator>(Entry<K,V> const& e) {
        return key > e.key;
    }
    
    bool operator==(Entry<K,V> const& e) {
        return key == e.key;
    }
    
    bool operator!=(Entry<K,V> const& e) {
        return key != e.key;
    }
    
};

#endif /* Entry_hpp */
