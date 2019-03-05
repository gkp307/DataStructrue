//
//  Eratosthenes.h
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef Eratosthenes_h
#define Eratosthenes_h


#include "Bitmap.h"
#include <iostream>

using namespace std;


//计算不大于10^8的所有素数
void Eratosthenes(int n,char *file){
    Bitmap B(n);
    B.set(0);
    B.set(1);  //0和1都不是素数
    
    for (int i = 2; i < n; i++) {  //反复地，从下一可认定的素数i起
        if (!B.test(i)) {
            for (int j = min(i, 46340) * min(i, 46340); j < n; j += i) {
                B.set(j);  //以i为间隔，将下一个数标记为合数
            }
        }
    }
    
    B.dump(file); //将所有整数的筛选标记统一存入指定文件，以便日后直接导入
}


#endif /* Eratosthenes_h */
