//
//  main.cpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#include <iostream>
using namespace std;

//01_绪论
#include "BubbleSort.h"
#include "Countones.h"
#include "Pow.h"
#include "Sum.h"
#include "Fabnacci.hpp"
#include "Reverse.h"
#include "Max.h"
#include "Hanoi.h"
#include "Gcd.h"
#include "Hailstone.h"

//02_向量
#include "Eratosthenes.h"


//03_列表
#include "List.hpp"

//04_栈和队列


//05_二叉树
#include "BinTree.hpp"
#include "PFCForest.hpp"
#include "Huffman.h"


//06_图
#include "Graph.hpp"
#include "GraphMatrix.hpp"
#include "GraphList.hpp"

int main(int argc, const char * argv[]) {
    
 
#pragma mark - 01绪论
    //1.计算机与算法
    //1)通过直线上给定的点A，作该直线的垂线
    //2)三等分给定的线段
    
    //3)冒泡排序(bubblesort1A)
    
    
    //2.算法
    //是指基于特定的计算模型，旨在解决某一信息处理问题而设计的一个指令序列
    
    //2.1 算法的要素
    /*
     1)输入与输出
     2)基本操作、确定性与可行性
     所谓确定性与可行性,是指算法应可描述为由若干语义明确的基本操作组成的指令序列，且每一基本操作在对应的计算模型中均可兑现
     3)有穷性与正确性
     证明算法的有穷性与正确性的一个重要技巧，就是从适当的角度审视整个计算过程，并指出其所具有的某种不变性与单调性。其中的单调性通常是指，问题的有效规模会随着算法的推进不断递减。不变性则不仅应在算法的初始状态下自然满足，而且应与最终的正确性相呼应————当问题的有效规模缩减到0时，不变性应随即等价于正确性。
     4)退化性与鲁棒性
     能处理各种极端的实例
     5)重用性
     */
    
    //2.2 算法的效率
    /*
     1)可计算性
     2)难解性
     3)计算效率
     4)数据结构
     */
    
    
    //3.复杂度
    //3.1 时间复杂度
    //在规模为n的所有输入中选择执行时间最长者作为T(n),并以T(n)度量该算法的时间复杂度
    
    //3.1.1  渐进复杂度
    /*
     在评价算法运行效率时，我们往往可以忽略其处理小规模问题时的能力差异，转而关注其在处理更大规模问题时的表现。这种着眼长远、更为注重时间复杂度的总体变化趋势和增长速度的策略与方法，即所谓的渐进分析。
     1) 大σ记号 (最坏情况复杂度)
     若存在正的常数c和函数f(n),使得对任何n >> 2都有   T(n) <= c*f(n)
     则可认为在n足够大之后，f(n)给出了T(n)增长速度的一个渐进上界。此时，记之为:
     T(n) = σ(f(n))
     大σ记号有一下性质
     a)对于任一常数c>0,有σ(f(n)) = σ(c(f(n)))
     b)对于任意常数a>b>0,有σ(n^a+n^b) = σ(n^a)
     
     2) 大Ω记号 (最好情况复杂度)
     如果存在正的常数c和函数g(n),使得对于任何n >> 2都有   T(n) >= c*g(n)
     就可以认为，在n足够大之后，g(n)给出了T(n)的一个渐进下界。此时我们记之为
     T(n) = Ω(g(n))
     
     3) 大Θ记号
     如果存在正的常数c1 < c2和函数h(n),使得对于任何n >> 2都有 c1*h(n) <= T(n) <= c2*h(n)
     就可以认为在n足够大之后，h(n)给出了T(n)的一个确界，此时我们记之为
     T(n) = Θ(h(n))
     */
    
    //3.2 空间复杂度
    
    
    
    //4.复杂度分析
    //1)常数σ(1)
    //2)对数σ(logn)  ---对数时间复杂度
    //3)线性σ(n)   ---线性时间复杂度
    //4)多项式σ(polynomial(n)) ---多项式时间复杂度
    //5)指数σ(2^n)   ---指数时间复杂度
    
#pragma mark - 02_向量
    
    //1、向量vector
    //向量就是线性数组的一种抽象与泛化，它也是有具有线性次序的一组元素构成的集合V={V0,V1,...,Vn-1},其中的元素分别由秩相互区分
    
    
    //无序向量
    //向量元素可通过相互比对判等---比如，元素类型T或为基本类型，或已重载操作符"=="或"!="。
    //这类仅支持比对，但未必支持比较的向量，称作无序向量
    
    
    //有序向量
    //若向量S[0,n)中的所有元素不仅按线性次序存放，而且其数值大小也按此次序单调分布，则称作有序向量
    
    //一般地，任一问题在最坏情况下的最低计算成本，即为该问题的复杂度下界
    
    
    //快速计算不大于1000000的所有素数
    char cs[] = "/testaaa";
    char *s = cs;
    Eratosthenes(100, s);
    
    
#pragma mark - 03_列表
    //插入排序
    /*
     始终将整个序列视作并切分为两部分:有序的前缀，无序的后缀；通过迭代，反复地将后缀的首元素转移至前缀中
     
     在任何时刻，相对于当前节点e = s[r],前缀s[0,r)总是业已有序
     */
    
    
    //选择排序
    /*
     该算法也将序列划分为无序前缀和有序后缀两部分；此外，还要求前缀不大于后缀。如此，每次只需从前缀中选出最大者，并作为最小元素转移到后缀中，即可使有序部分的范围不断扩张
     
     在任何时刻，后缀S(r,n)已经有序，且不小于前缀S[0,r]
     */
    
    
#pragma mark - 04_栈和队列
    
    
#pragma mark - 05_二叉树
    /*
     PFCForest *forest = initForest();
     PFCTree *tree = generateTree(forest);
     delete forest;
     
     PFCTable *table = generateTable(tree); //将PFC编码树转换成编码表
     
     for (int i = 1; i < argc; i++) {
     Bitmap codeString;
     int n = encode(table, codeString, argv[i]);
     decode(tree, codeString, n);
     }
     
     delete table;
     delete tree;
     */
    
    /*
    int *freq = statistics(argv[1]);
    
    HuffForest *forest = initForest(freq);
    delete freq;
    
    HuffTree *tree = generateTree(forest);
    delete forest;
    
    HuffTable *table = generateTable(tree); //将PFC编码树转换成编码表
    
    for (int i = 2; i < argc; i++) {
        Bitmap *codeString = new Bitmap;
        int n = encode(table, codeString, argv[i]);
        decode(tree, codeString, n);
        delete codeString;
    }
    
    delete table;
    delete tree;
    */
    
#pragma mark - 06_图
    /*
     所谓的图，可定义为G = (V, E)。其中，集合V中的元素称作定点(vertex),集合E中的元素分别对应V中的某一对顶点(u,v),表示他们之间存在某种关系，故亦称作边(edge)。
     
     若边(u,v)所对应顶点u和v的次序无所谓，则称作无向边(undirected edge)。反之若u和v不对等，则称(u,v)为有向边(directed edge)
     
     有向边(u,v)从u指向v，其中u称作该边的起点或尾顶点，而v称作该边的终点或头顶点。
     
     若E中各边均无方向，则G称作无向图(undirected graph)。反之，若E中含有有向边，则G称作有向图(directed graph)。若E同时包含无向边和有向边，则G称作混合图(mixed graph)。
     
     对于有向边e = (u,v),e称作u的出边(outgoing edge),v的入边(incoming edge).
     v的出边总数称作其出度(out-degree);入边总数称作其入度(in-degree)
     
     联接于同一顶点之间的边，称作自环(self-loop).
     不含任何自环的图称作简单图(simple graph)。
     
     所谓路径或通路(path),就是由m+1个顶点与m条边交替而成的一个序列
     p = {v0,e1,v1,e2,v2,...,em,vm}
     
     沿途顶点的通路，称作简单通路。(simple path)
     
     特别的，对于长度m>=1的通路，若起止顶点相同(即v0=vm),则称作环路。
     不含任何环路的有向图，称作有向无环图(directed acyclic graph, DAG)
     若沿途除v0=vm外所有顶点互异，则称作简单环路(simple cycle)。
     特别的，经过图中各边一次且恰好一次的环路，称作欧拉环路(Eulerian tour)。
     经过图中各顶点一次且恰好一次的环路，称作哈密尔顿环路(Hamiltonian tour)。
     
     
     此处的"相容"，准确的含义是：每一个顶点都不会通过边，指向其在此序列中的前驱顶点，这样的一个线性序列，称作原有向图的一个拓扑排序(topological sorting)
     不含环路的有向图---有向无环图---一定存在拓扑排序
     
     */
    
    return 0;
}
