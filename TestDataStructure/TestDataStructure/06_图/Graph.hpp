//
//  Graph.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef Graph_hpp
#define Graph_hpp

#include "Stack.hpp"
#include "Queue.hpp"

typedef enum {
    UNDISCOVERED,
    DISCOVERED,
    VISITED
}VStatus;   //顶点状态

typedef enum {
    UNDETERMINED,
    TREE,
    CROSS,
    FORWARD,
    BACKWARD
}EType;  //边在遍历树中所属的类型

template <typename Tv,typename Te>  //顶点类型，边类型
class Graph{
private:
    //所有顶点、边的辅助信息复位
    void reset(){
        for (int i = 0; i < n; i++) { //所有顶点的
            status(i) = UNDISCOVERED;
            dTime(i) = fTime(i) = -1;  //状态，时间标签
            parent(i) = -1;
            priority(i) = INT_MAX; //(在遍历树中的)父节点，优先级数
            
            for (int j = 0; j < n; j++) {
                if (exists(i, j)) {
                    type(i, j) = UNDETERMINED; //所有边的类型
                }
            }
        }
    }
    
    void BFS(int, int&);  //(连通域)广度优先搜索算法
    void DFS(int, int&);  //(连通域)深度优先搜索算法
    void BCC(int, int&, Stack<int>&); //(连通域)基于DFS的双连通分量分解算法
    bool TSort(int, int&,Stack<Tv>*); //(连通域)基于DFS的拓扑排序算法
    template <typename PU> void PFS(int, PU); //(连通域)优先级搜索框架
    
public:
    //顶点的操作
    int n;  //顶点总数
    virtual int insert(Tv const&) = 0;  //插入顶点，返回编号
    virtual Tv remove(int) = 0;  //删除顶点及其关联边，返回该顶点信息
    virtual Tv& vertex(int) = 0; //顶点v的数据(该顶点的确存在)
    virtual int inDegree(int) = 0; //顶点v的入度
    virtual int outDegree(int) = 0; //顶点v的出度
    virtual int firstNbr(int) = 0; //顶点v的首个邻接顶点
    virtual int nextNbr(int, int) = 0;//顶点v的(相对于顶点j的)下一邻接顶点
    virtual VStatus& status(int) = 0; //顶点v的状态
    virtual int& dTime(int) = 0; //顶点v的时间标签dTime
    virtual int& fTime(int) = 0; //顶点v的时间标签fTime
    virtual int& parent(int) = 0; //顶点v在遍历树中的父亲
    virtual int& priority(int) = 0; //顶点v在遍历树中的优先级数
    
    //边的操作
    //这里约定，无向边均统一转化成方向互逆的一对有向边，从而将无向边视作有向边的特例
    int e;  //边总数
    virtual bool exists(int,int) = 0; //边(v, u)是否存在
    virtual void insert(Te const&,int,int,int) = 0;//在顶点v和u之间插入权重为w的边e
    virtual Te remove(int, int) = 0;  //删除顶点v和u之间的边e，返回该边信息
    virtual EType& type(int,int) = 0; //边(v,u)的类型
    virtual Te& edge(int,int) = 0; //边(v,u)的数据(该边的确存在)
    virtual int& weight(int, int) = 0; //边(v, u)的权重
    
    //算法
    void bfs(int);  //广度优先搜索算法
    void dfs(int); //深度优先搜索算法
    void bcc(int);  //基于DFS的双连通分量分解算法
    
    Stack<Tv> *tSort(int); //基于DFS的拓扑排序算法
    
    void prim(int); //最小支撑树prim算法
    void dijkstra(int); //最短路径dijkstra算法
    template <typename PU> void pfs(int, PU);  //优先级搜索框架
    
};

//广度优先搜索算法(breadth-first-serach, BFS)
//越早被访问到的顶点，其邻居越优先被选用
template <typename Tv,typename Te>
void Graph<Tv,Te>::bfs(int s) {
    //assert: 0 <= s < n
    reset();
    int clock = 0;
    int v = s;
    
    do {
        //逐一检查所有顶点,一旦遇到尚未发现的顶点，即从该顶点出发启动一次BFS
        if (UNDISCOVERED == status(v)) {
            BFS(v, clock);
        }
        
    } while ( s != (v = (++v % n)));  //按序号检查，故不漏不重
}

//单个连通域
template <typename Tv,typename Te>
void Graph<Tv, Te>::BFS(int v, int &clock) {
    //assert: 0 <= v < n
    Queue<int> Q;
    status(v) = DISCOVERED;
    Q.enqueue(v);
    
    while (!Q.empty()) {
        int v = Q.dequeue();
        dTime(v) = ++clock;
        
        for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) {
            //枚举v的所有邻居u
            if (UNDISCOVERED == status(u)) {
                //若u尚未被发现，则发现该顶点，引入树边拓展支撑树
                status(u) = DISCOVERED;
                Q.enqueue(u);
                type(v, u) = TREE;
                parent(u) = v;
            }else{
                //若u已被发现，或者甚至已访问完毕，则将(v,u)归类于跨边
                type(v, u) = CROSS;
            }
        }
        
        status(v) = VISITED;
    }
    
}

//深度优先搜索(depth-first-search, DFS)
//优先选取最后一个被访问到的顶点的邻居
template <typename Tv, typename Te>
void Graph<Tv, Te>::dfs(int s) {
    reset();
    int clock = 0;
    int v = s;
    
    do {
        //逐一检查所有顶点,一旦遇到尚未发现的顶点，即从该顶点出发启动一次DFS
        if (UNDISCOVERED == status(v)) {
            DFS(v, clock);
        }
        
    } while ( s != (v = (++v % n)));  //按序号检查，故不漏不重
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::DFS(int v,int& clock) {
    //assert: 0 <= v < n
    dTime(v) = ++clock;
    status(v) = DISCOVERED;
    
    for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) {
        //枚举v的所有邻居
        switch (status(u)) {
            case UNDISCOVERED:
                //u尚未发现，意味着支撑树可在此拓展
                type(v, u) = TREE;
                parent(u) = v;
                DFS(u, clock);
                break;
            case DISCOVERED:
                //u已被发现但尚未访问完毕，应属被后代指向的祖先
                type(v, u) = BACKWARD;
                break;
                
            default:
                //u已访问完毕(VISITED,有向图),则视承袭关系分为前向边或跨边
                type(v, u) = ( dTime(v) < dTime(u)) ? FORWARD : CROSS;
                break;
        }
    }
    
    status(v) = VISITED;
    fTime(v) = ++clock; //至此，当前节点v方告访问完毕
    
}

//基于DFS的拓扑排序算法
template <typename Tv,typename Te>
Stack<Tv>* Graph<Tv, Te>::tSort(int s) {
    reset();
    int clock = 0;
    int v = s;
    
    Stack<Tv> *S = new Stack<Tv>;  //用栈记录排序顶点
    
    do{
        if (UNDISCOVERED == status(v)) {
            //clock并非必须
            if (!TSort(v, clock, S)) {
                
                //任一连通域(亦即整图)非DAG，则不必继续计算，故直接返回
                while (!S->empty()) {
                    S -> pop();
                    break;
                }
            }
        }
        
    }while( s != (v = (++v % n)));
    //若输入为DAG(有向无环图),则S内各顶点自顶向底排序；否则（不存在拓扑排序），S空
    return S;
}

//基于DFS的拓扑排序算法(单趟)
template <typename Tv,typename Te>
bool Graph<Tv, Te>::TSort(int v, int &clock, Stack<Tv> *S) {
    //assert: 0 <= v < n
    dTime(v) = ++clock;
    status(v) = DISCOVERED;
    
    for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) {
        //枚举v的所有邻居
        switch (status(u)) {
            case UNDISCOVERED:
                parent(u) = v;
                type(v, u) = TREE;
                
                //从顶点u出出发深入搜索，若u及其后代不能拓扑排序(则全图亦必如此)，返回并报告
                if (! TSort(u, clock, S)) {
                    return false;
                }
                break;
                
            case DISCOVERED:
                //一旦发现后向边，则不必深入，故返回并报告
                type(v, u) = BACKWARD;
                return false;
                break;
                
            default:
                type(v, u) = ( dTime(v) < dTime(u) ? FORWARD : CROSS);
                break;
        }
    }
    
    status(v) = VISITED;
    S -> push(vertex(v)); //顶点被标记为VISITED时，随即入栈
    return true;  //v及其后代可以拓扑排序
}

//基于DFS的BCC分解算法
template <typename Tv,typename Te>
void Graph<Tv, Te>::bcc(int s) {
    reset();
    int clock = 0;
    int v = s;
    
    Stack<int> S;
    
    do{
        if (UNDISCOVERED == status(v)) {
            BCC(v, clock, S);
            S.pop();
        }
        
    }while( s != (v = (++v % n)) );
    
}

#define hca(x) (fTime(x))  //利用此处闲置的fTime[]充当hca[]
template <typename Tv,typename Te>
void Graph<Tv, Te>::BCC(int v, int &clock, Stack<int> &S) {
    
    hca(v) = dTime(v) = ++clock;
    status(v) = DISCOVERED;
    S.push(v);
    
    for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) {
        
        switch (status(u)) {
            case UNDISCOVERED:
                parent(u) = v;
                type(v, u) = TREE;
                BCC(u, clock, S); //从顶点u出深入
                
                //遍历返回后，若发现u(通过后向边)可指向v的真祖先,则v亦必如此
                if (hca(u) < dTime(v)) {
                    hca(v) = std::min(hca(v), hca(u));
                }else{
                    //否则，以v为关节点(u以下即是一个BCC，且其中顶点此时正集中于栈S的顶部)
                    while ( v != S.pop()) {
                        //依次弹出当前BCC中的节点，亦可根据实际需求转存至其他结构
                        S.push(v); //最后一个顶点(关节点)重新入栈----分摊不足一次
                    }
                }
                
                break;
                
            case DISCOVERED:
                type(v, u) = BACKWARD;
                if (u != parent(v)) {
                    hca(v) = std::min(hca(v), hca(u));
                }
                break;
                
            default:
                type(v, u) = (dTime(v) < dTime(u) ? FORWARD : CROSS);
                break;
        }
    }
    
    status(v) = VISITED;
}

#undef hca


//优先级搜索
template <typename Tv,typename Te> template <typename PU>
void Graph<Tv, Te>::pfs(int s, PU prioUpdater) {
    reset();
    int v = s;
    
    do{
        if (UNDISCOVERED == status(v)) {
            PFS(v, prioUpdater);
        }
    }while( s != (v = (++v % n)));
}

template <typename Tv,typename Te> template <typename PU>
void Graph<Tv, Te>::PFS(int s, PU prioUpdater){
    
    priority(s) = 0;
    status(s) = VISITED;
    parent(s) = -1;
    
    while (1) {  //将下一顶点和边加至PFS(priority-first-search)树中
        for (int w = firstNbr(s); -1 < w; w = nextNbr(s, w)) {
            prioUpdater(this, s, w);
        }
        
        for (int shortest = INT_MAX, w = 0; w < n; w++) {
            //从尚未加入遍历树的顶点中选出下一个
            if (UNDISCOVERED == status(w)) {
                if (shortest > priority(w)) {
                    shortest = priority(w);
                    s = w;
                }
            }
        }
        
        //直至所有顶点均已加入
        if (VISITED == status(s)) {
            break;
        }
        
        status(s) = VISITED;
        type(parent(s), s) = TREE; //将s及与其父的联边加入遍历树
    }
    
}



//针对Prim算法的顶点优先级更新器
template <typename Tv, typename Te> struct PrimPU {
    virtual void operator()(Graph<Tv, Te>* g,int uk,int v){
        if (UNDISCOVERED == g -> status(v)) {
            if (g->priority(v) > g -> weight(uk, v)) {
                //按Prim策略做松弛
                g -> priority(v) = g -> weight(uk, v);
                g -> parent(v) = uk;
            }
        }
    }
};


//迪杰斯特拉算法
//针对Dijkstra算法的顶点优先级更新器
template <typename Tv, typename Te> struct DijkstraPU {
    virtual void operator()(Graph<Tv, Te>* g,int uk,int v){
        if (UNDISCOVERED == g -> status(v)) {
            if (g->priority(v) > g->priority(uk) + g -> weight(uk, v)) {
                //做松弛
                g -> priority(v) = g->priority(uk) + g -> weight(uk, v);
                g -> parent(v) = uk;
            }
        }
    }
};

//针对BFS算法的顶点优先级更新器
template <typename Tv, typename Te> struct BfsPU {
    virtual void operator()(Graph<Tv, Te>* g,int uk,int v){
        if (UNDISCOVERED == g -> status(v)) {  //对于uk每一尚未被发现的邻接顶点v
            if (g->priority(v) > g->priority(uk) + 1) { //将其到起点的距离作为优先级数
                g -> priority(v) = g->priority(uk) + 1; //更新优先级(数)
                g -> parent(v) = uk;//更新父节点
            }//如此效果等同于，先被发现者优先
        }
    }
    
};

//针对DFS算法的顶点优先级更新器
template <typename Tv, typename Te> struct DfsPU {
    virtual void operator()(Graph<Tv, Te>* g,int uk,int v){
        if (UNDISCOVERED == g -> status(v)) {
            if (g->priority(v) > g->priority(uk) - 1) {
                g -> priority(v) = g->priority(uk) - 1;
                g -> parent(v) = uk;
                return;//注意:与BfsPU不同，这里只要有一个邻接顶点可更新，即可立即返回
            }//如此效果等同于，后被发现者优先
        }
    }
    
};

#endif /* Graph_hpp */
