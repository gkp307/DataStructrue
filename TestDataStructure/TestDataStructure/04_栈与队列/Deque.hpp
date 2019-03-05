//
//  Deque.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef Deque_hpp
#define Deque_hpp

//双端队列

template <typename T> class Deque {
public:
    T& front();
    T& rear();
    void insertFront(T const& e);
    void insertRear(T const& e);
    T removeFront();
    T removeRear();
};


#endif /* Deque_hpp */
