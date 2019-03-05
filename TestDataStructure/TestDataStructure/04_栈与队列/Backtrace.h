//
//  Backtrace.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef Backtrace_hpp
#define Backtrace_hpp


#include "Stack.hpp"

//试探回溯法

//1.八皇后
struct Queen {
    int x,y;
    
    Queen(int xx = 0,int yy = 0) :x(xx), y(yy){}
    
    bool operator==(Queen const& q) const {
        return (x == q.x) || (y == q.y) || (x + y == q.x + q.y) || (x - y == q.x - q.y);
    }
    
    bool operator!=(Queen const& q) const {
        return ! (*this == q);
    }
    
};

//N皇后算法，迭代版
void placeQueen(int N) {
    Stack<Queen> solu;
    Queen q(0,0); //从原点位置出发
    
    int nCheck = 0;
    int nSolu = 0;
    
    do {  //反复试探，回溯
        
        if (N <= solu.size() || N <= q.y) {  //若已出界，则回溯一行，并继续试探下一列
            q = solu.pop();
            q.y++;
        }else{
            //否则试探下一行
            while ( (q.y < N) && (0 <= solu.find(q)) ) {
                //通过与已有皇后的比对，尝试找到可摆放下一皇后的列
                q.y++;
                nCheck++;
            }
            
            if (N > q.y) {
                //若存在可摆放的列，则摆上当前皇后
                solu.push(q);
                if (N <= solu.size() ) {
                    //若部分解已成为全部解，则通过全局变量nSolu计数
                    nSolu++;
                }
                
                //转入下一行，从第0列开始，试探下一皇后
                q.x++;
                q.y = 0;
            }
        }
        
        
    }while( (0 < q.x) || (q.y < N)); //所有分支均已或穷举或剪枝之后，算法结束
}


#endif /* Backtrace_hpp */
