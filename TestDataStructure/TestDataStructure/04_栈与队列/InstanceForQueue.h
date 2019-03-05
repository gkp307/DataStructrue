//
//  InstanceForQueue.h
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef InstanceForQueue_h
#define InstanceForQueue_h


#include "Queue.hpp"

//循环分配器


//银行服务模拟
struct Customer {
    int window;           //所属窗口
    unsigned int time;    //服务时长
};

int bestWindow(Queue<Customer> windows[], int nWin){
    
    int minSize = windows[0].size(),optiwin = 0;
    
    for (int i = 1; i < nWin; i++) {
        if (minSize > windows[i].size()) {
            minSize = windows[i].size();
            optiwin = i;
        }
    }
    
    return optiwin;
}

void simulate(int nWin,int serveTime) { //按指定窗口数，服务总时间模拟银行业务
    Queue<Customer>* windows = new Queue<Customer>[nWin]; //为每一窗口创建一个队列
    
    for (int now = 0; now < serveTime; now++) { //在下班之前，每隔一个单位时间
        if (rand() % (1 + nWin)) { //新顾客以nWin/(nWin+1)的概率到达
            Customer c;
            c.time = 1 + rand() % 98;  //新顾客到达，服务时常随机确定
            c.window = bestWindow(windows, nWin); //找出最佳的服务窗口
            windows[c.window].enqueue(c); //新顾客加入对应的队列
        }
        
        for (int i = 0; i < nWin; i++) {  //分别检查
            if (!windows[i].empty()) {  //各非空队列
                if (--windows[i].front().time <= 0) { //队首顾客的服务时长减少一个单位
                    windows[i].dequeue();  //服务完毕的顾客出列，由后继顾客接替
                }
            }
        }
    }
    
    delete []windows;  //释放所有队列(此前，~List()会自动清空队列 )
}



#endif /* InstanceForQueue_h */
