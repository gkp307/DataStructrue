//
//  Queue.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef Queue_hpp
#define Queue_hpp

#include "List.hpp"

template <typename T> class Queue: public List<T> {
public:
    void enqueue(T const& e){
        this->insertAsLast(e);
    }
    
    T dequeue(){
        return this->remove(this->first());
    }
    
    T& front(){
        return this->first()->data;
    }
};


#endif /* Queue_hpp */
