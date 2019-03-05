//
//  BubbleSort.h
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef BubbleSort_h
#define BubbleSort_h

//冒泡排序
void bubblesort1A(int A[], int n) {
    bool sorted = false;
    while (!sorted) {
        sorted = true;
        
        for (int i = 1; i < n; i++) {
            if (A[i-1] > A[i]) {
                swap(A[i-1], A[i]);
                sorted = false;
            }
        }
        n--;
    }
}

#endif /* BubbleSort_h */
