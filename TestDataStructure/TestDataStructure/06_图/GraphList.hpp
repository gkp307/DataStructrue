//
//  GraphList.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef GraphList_hpp
#define GraphList_hpp

//邻接表实现图
#include "Vector.hpp"
#include "List.hpp"
#include "Graph.hpp"
#include "GraphMatrix.hpp"

template <typename Tv,typename Te>
class GraphList : public Graph<Tv, Te> {
private:
    Vector<Vertex<Tv>> V;
    Vector<List<Edge<Te> * >> E;
};

#endif /* GraphList_hpp */
