//
//  Bitmap.h
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef Bitmap_h
#define Bitmap_h

#include <iostream>
#include "Vector.hpp"

class Bitmap{
private:
    //比特图所存放的空间M[],容量为N*sizeof(char)*8比特
    char *M;
    int N;
protected:
    void init(int n){
        M = new char[N = (n+7)/8];
        memset(M, 0, N);
    }
    
public:
    Bitmap(int n = 8) {
        init(n);
    }
    
    Bitmap(char *file,int n = 8){
        init(n);
        FILE *fp = fopen(file, "r");
        fread(M, sizeof(char), N, fp);
        fclose(fp);
    }
    
    ~Bitmap(){
        delete [] M;
        M = NULL;
    }
    
    
    //通过k>>3 即可确定对应的比特位所属字节的秩
    //通过逻辑位运算k&0x07 即可确定该比特位在此字节中的位置
    //通过移位操作0x80 >> (k & 0x07)即可得到该比特位在此字节中对应的数值掩码(mask)
    void set(int k){
        expand(k);
        M[k >> 3] |= (0x80 >> ( k & 0x07));
    }
    
    void clear(int k){
        expand(k);
        M[k >> 3] &= ~(0x80 >> ( k & 0x07));
    }
    
    bool test(int k){
        expand(k);
        return M[k >> 3] & (0x80 >> ( k & 0x07));
    }
    
    //将位图整体导出至指定的文件，以便对此后的新位图批量初始化
    void dump(char *file){
        FILE *fp = fopen(file, "w");
        fwrite(M, sizeof(char), N, fp);
        fclose(fp);
    }
    
    //将前n位转换为字符串
    char* bits2string(int n){
        expand(n - 1); //此时可能被访问的最高位为bitmap[n-1]
        char *s = new char[n + 1];
        s[n] = '\0';   //字符串所占空间，由上层调用者负责释放
        for (int i = 0; i < n; i++) {
            s[i] = test(i) ? '1' : '0';
        }
        return s; //返回字符串位置
    }
    
    
    //若被访问的Bitmap[k]已出界，则需扩容
    void expand(int k){
        if (k < 8 * N) {  //仍在界内，无需扩容
            return;
        }
        
        int oldN = N;
        char *oldM = M;
        init(2 * k);       //与向量类似，加倍策略
        memcpy(M, oldM, oldN);
        //        memcpy_s(M,N,oldM,oldN);
        delete [] oldM;   //原数据转移至新空间
    }
};



//以空间作为补偿，节省初始化时间
class Bitmap2{
private:
    Rank *F;
    Rank N;  //规模为N的向量F,记录[k]被标记的次序(即其在栈T[]中的秩)
    Rank *T;
    Rank top; //容量为N的栈T，记录被标记各位秩的栈，以及栈顶指针
protected:
    inline bool valid(Rank r) {
        return (0 <= r) && (r < top);
    }
    
    inline bool erased(Rank k) { //判断[k]是否曾被标记过，且后来又被清除
        return valid(F[k]) && !(T[F[k]] + 1 + k); //这里约定: T[]
    }
    
public:
    Bitmap2(Rank n = 8){
        N = n;
        F = new Rank[N];
        T = new Rank[N];
        top = 0;
    }
    
    ~Bitmap2(){
        delete [] F;
        delete [] T;
    }
    
    inline void set(Rank k) {  //插入
        if (test(k)) {   //忽略已带标记的位
            return;
        }
        
        if (!erased(k)) {
            F[k] = top++;   //若是初次标记，则创建新校验环
        }
        
        T[F[k]] = k;   //若是曾经标记后被清除，则回复原校验环
    }
    
    inline bool test(Rank k) {
        return valid(F[k]) && (k == T[ F[k] ]);
    }
    
    inline void clear(Rank k){   //删除
        if (test(k)) {
            T[F[k]] = -1-k;   //忽略不带标记的位
        }
    }
};

#endif /* Bitmap_h */
