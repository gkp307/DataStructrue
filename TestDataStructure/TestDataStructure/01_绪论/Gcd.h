//
//  Gcd.h
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef Gcd_h
#define Gcd_h


//最大公约数 greatest common divisor
int gcdCN(int a,int b){  //assert: 0 < min(a,b)
    int r = 0;   //a和b的2^r形式的公因子
    while (! (   ( a & 1) || (b & 1)   ) ) {  //若a和b都为偶数
        a >>= 1;
        b >>= 1;
        r++;
    }//以下，a和b至多其一为偶
    
    while (1) {
        while (!(a & 1)) {    //a偶b奇，则剔除a的所有因子2
            a >>= 1;
        }
        while (!(b & 1)) {    //b偶a奇，则剔除b的所有因子2
            b >>= 1;
        }
        
        (a > b) ? a = a-b : b = b-a; //简化为gcd( max(a,b)-min(a,b), min(a,b) )
        
        if (0 == a) {
            return b << r;  //简化至平凡情况:gcd(0,b) = b
        }
        if (0 == b) {
            return a << r;  //简化至平凡情况:gcd(a,0) = a
        }
        
    }
}//σ(log(a+b))

#endif /* Gcd_h */
