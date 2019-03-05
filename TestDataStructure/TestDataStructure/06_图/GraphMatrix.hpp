//
//  GraphMatrix.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef GraphMatrix_hpp
#define GraphMatrix_hpp

#include "Graph.hpp"
#include "Vector.hpp"

//邻接矩阵实现图

//顶点对象(为简化起见，并未严格封装)
template <typename Tv> struct Vertex {
    Tv data;
    int inDegree, outDegree;
    VStatus status;    //数据，出入度，状态
    
    int dTime, fTime; //时间标签
    
    int parent;
    int priority; //在遍历树中的父节点、优先级数
    
    Vertex(Tv const& d = (Tv) 0) : data(d),inDegree(0),outDegree(0),status(UNDISCOVERED),dTime(-1),fTime(-1),parent(-1),priority(INT_MAX){}  //暂不考虑权重溢出
};

//边对象(为简化起见，并未严格封装)
template <typename Te> struct Edge{
    Te data;
    int weight;
    EType type;
    Edge(Te const& d,int w):data(d),weight(w),type(UNDETERMINED) {}
};


template <typename Tv,typename Te>
class GraphMatrix : public Graph<Tv, Te> {
private:
    Vector<Vertex<Tv>> V; //顶点集(向量)
    Vector<Vector<Edge<Te>* >> E; //边集(邻接矩阵)
    
public:
    
    GraphMatrix() {
        this->n = this -> e = 0;
    }
    
    ~GraphMatrix() {
        //所有动态创建的边记录，逐条清除
        for (int j = 0; j < this->n; j++) {
            for (int k = 0; k < this->n; k++) {
                delete E[j][k];
            }
        }
    }
    
#pragma mark - 顶点的基本操作:查询第i个顶点(0 <= i < n)
    //数据
    virtual Tv& vertex(int i) override {
        return V[i].data;
    }
    
    //入度
    virtual int inDegree(int i) override {
        return V[i].inDegree;
    }
    
    //出度
    virtual int outDegree(int i) override {
        return V[i].outDegree;
    }
    
    //首个邻接顶点
    virtual int firstNbr(int i) override {
        return nextNbr(i, this->n);
    }
    
    //相对于顶点j的下一邻接顶点(改用邻接表可提高效率)
    virtual int nextNbr(int i,int j) override {
        //逆向线性探索
        while ( (-1 < j) && (!exists(i, --j)) );
        return j;
    }
    
    //状态
    virtual VStatus& status(int i) override {
        return V[i].status;
    }
    
    //时间标签dTime
    virtual int& dTime(int i) override {
        return V[i].dTime;
    }
    
    //时间标签fTime
    virtual int& fTime(int i) override {
        return V[i].fTime;
    }
    
    //在遍历树中的父亲
    virtual int& parent(int i) override {
        return V[i].parent;
    }
    
    //在遍历树中的优先级数
    virtual int& priority(int i) override {
        return V[i].priority;
    }
    
#pragma mark - 顶点的动态操作
    //插入顶点，返回编号
    virtual int insert(Tv const& vertex) override {
        for (int j = 0; j < this->n; j++) {
            //各顶点预留一条潜在的关联边
            E[j].insert(NULL);
        }
        
        this->n++;
        
        //创建新顶点对应的边向量
        E.insert(Vector<Edge<Te> *>(this->n, this->n, (Edge<Te> *)NULL ) );
        //顶点向量增加一个顶点
        return V.insert(Vertex<Tv>(vertex) );
    }
    
    //删除第i个顶点及其关联边(0 <= i < n)
    virtual Tv remove(int i) override {
        for (int j = 0; j < this -> n; j++) {
            //所有出边，逐条删除
            if (exists(i, j)) {
                delete E[i][j];
                V[j].inDegree--;
            }
        }
        
        //删除第i行
        E.remove(i);
        this->n--;
        
        //删除顶点i
        Tv vBak = vertex(i);
        V.remove(i);
        
        //所有入边，逐条删除
        for (int j = 0; j < this->n; j++) {
            if (Edge<Te> *e = E[j].remove(i)) {
                delete e;
                V[j].outDegree--;
            }
        }
        
        return vBak; //返回被删除顶点的信息
    }
    
    
#pragma mark - 边的确认操作
    //边(i, j)是否存在
    virtual bool exists(int i,int j) override {
        return ( 0 <= i) && (i < this->n) && ( 0 <= j) && (j < this->n) && E[i][j] != NULL;
    }
    
#pragma mark - 边的基本操作:查询顶点i与j之间的联边(0 <= i, j< n且exists(i ,j))
    //边(i,j)的类型
    virtual EType& type(int i,int j) override {
        return E[i][j] -> type;
    }
    
    //边(i, j)的数据
    virtual Te& edge(int i,int j) override {
        return E[i][j] -> data;
    }
    
    //边(i,j)的权重
    virtual int& weight(int i,int j) override {
        return E[i][j] -> weight;
    }
    
#pragma mark - 边的动态操作
    
    //插入权重为w的边e = (i,j)
    virtual void insert(Te const& edge,int w, int i, int j) override {
        if ( exists(i, j) ) {
            return;
        }
        
        E[i][j] = new Edge<Te>(edge, w);
        this->e++;
        
        V[i].outDegree++;
        V[j].inDegree++;
    }
    
    //删除顶点i和j之间的联边(exists(i,j))
    virtual Te remove(int i,int j) override {
        //备份后删除边记录
        Te eBak = edge(i, j);
        delete E[i][j];
        E[i][j] = NULL;
        
        //更新边计数与关联顶点的度数
        this -> e--;
        V[i].outDegree--;
        V[j].inDegree--;
        
        return eBak;
        
    }
    
};


#endif /* GraphMatrix_hpp */
