//
//  Dictionary.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef Dictionary_hpp
#define Dictionary_hpp

#define N_CHAR (0x80 - 0x20)  //只考虑可打印字符

//词典Dictionary模版类
template <typename K,typename V> struct Dictionary {
public:
    virtual int size() const = 0;  //当前词条总数
    virtual bool put(K,V) = 0;  //插入词条(禁止雷同词条时可能失败)
    virtual V* get(K k) = 0;  //读取词条
    virtual bool remove(K k) = 0; //删除词条
};


#endif /* Dictionary_hpp */
