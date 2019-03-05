//
//  Stack.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef Stack_hpp
#define Stack_hpp

#include "Vector.hpp"

template <typename T>
class Stack: public Vector<T> {  //将向量的首/末端作为栈底/顶
public:
    void push(T const& e) {
        //模版类子类使用模版类父类成员需要使用 域作用符
        this->insert(this->size(), e);
    }
    
    T pop() {
        return this->remove(this->size() - 1);
    }
    
    T& top() {
        return (*this)[this->size() - 1];
    }
};


#endif /* Stack_hpp */
