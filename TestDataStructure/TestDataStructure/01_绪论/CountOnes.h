//
//  CountOnes.h
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef CountOnes_h
#define CountOnes_h


//对数σ(logn)
//对于任意非负整数，统计其二进制展开中 位数1的  总数
int countones(unsigned int n){
    int ones = 0;
    while (0 < n) {
        ones += (1 & n);  //检查最低位，若为1则计数
        n >>= 1;
    }
    return ones;
}

//改进countOnes算法
int countones1(unsigned int n){
    int ones = 0;
    while (0 < n) {
        ones++;
        n &= n - 1;   //清除当前最靠右的1
    }
    return ones;
}//σ(countones(n)),线性正比于数位1的实际数目


#define POW(c)     (1 << (c))  //2^c
#define MASK(c)    (  ((unsigned long)-1)/(POW(POW(c)) + 1)  )
//MASK(0) = 5555 5555(H) = 0101 0101 0101 0101 0101 0101 0101 0101
//MASK(1) = 3333 3333(H) = 0011 0011 0011 0011 0011 0011 0011 0011
//MASK(2) = 0f0f 0f0f(H) = 0000 1111 0000 1111 0000 1111 0000 1111
//MASK(3) = 00ff 00ff(H) = 0000 0000 1111 1111 0000 0000 1111 1111
//MASK(4) = 0000 ffff(H) = 0000 0000 0000 0000 1111 1111 1111 1111


//输入:n的二进制展开位中，以2^c位为单位分组，各组数值已经分别等于原先这2^c位中1的数目
#define ROUND(n,c)    (  (  (n) & MASK(c) ) + ( (n)>>POW(c) & MASK(c) )  ) //运算优先级:先右移，再位与
//过程:以2^c位为单位分组，相邻的组两两捉对累加，累加值用原2^(c+1)位就地记录
//输出:n的二进制展开位中，以2^(c+1)位为单位分组，各组数值已经分别等于原先这2^(c+1)位中1的数目



int countones2(unsigned int n){ //统计整数n的二进制展开位中数位1的总数
    n = ROUND(n, 0); //以02位单位分组，各组内前01位与后01位累加，得到原先这02位中1的数目
    n = ROUND(n, 1); //以04位单位分组，各组内前02位与后02位累加，得到原先这04位中1的数目
    n = ROUND(n, 2); //以08位单位分组，各组内前04位与后04位累加，得到原先这08位中1的数目
    n = ROUND(n, 3); //以16位单位分组，各组内前08位与后08位累加，得到原先这16位中1的数目
    n = ROUND(n, 4); //以32位单位分组，各组内前16位与后16位累加，得到原先这32位中1的数目
    return n;
}
//32位字长时，σ(log_2(32)) = σ(5) = σ(1)
//σ(logW), w = σ(logn)为整数的位宽


#endif /* CountOnes_h */
