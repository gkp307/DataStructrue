//
//  Maze.h
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef Maze_h
#define Maze_h

#include "Stack.hpp"

//迷宫寻径
typedef enum {
    AVAILABLE,      //原始可用的
    ROUTE,          //在当前路径上的
    BACKTRACKED,    //所有方向均尝试失败后回溯过的
    WALL            //不可使用的(墙)
}Status;


typedef enum {
    UNKNOWN, EAST,SOUTH,WEST,NORTH,NO_WAY
}ESWN;
//未定、东、南、西、北、无路可通

//依次转至下一邻接方向
inline ESWN nextESWN(ESWN eswn){
    return ESWN(eswn + 1);
}

struct Cell {
    int x,y;
    Status status;
    ESWN incoming,outgoing;  //进入、走出方向
};

#define LABY_MAX  24   //最大迷宫尺寸
Cell laby[LABY_MAX][LABY_MAX]; //迷宫


//查询当前位置的相邻格点
inline Cell* neighbor(Cell *cell) {
    switch (cell -> outgoing) {
        case EAST:
            return cell + LABY_MAX;
            break;
            
        case SOUTH:
            return cell + 1;
            break;
            
        case WEST:
            return cell - LABY_MAX;
            break;
            
        case NORTH:
            return cell - 1;
            break;
            
        default:
            exit(-1);
            break;
    }
}

//从当前位置转入相邻格点
inline Cell* advance(Cell *cell) {
    Cell *next;
    
    switch (cell -> outgoing) {
        case EAST:
            next = cell + LABY_MAX;
            next -> incoming = WEST;
            break;
            
        case SOUTH:
            next = cell + 1;
            next -> incoming = NORTH;
            break;
            
        case WEST:
            next = cell - LABY_MAX;
            next -> incoming = EAST;
            break;
            
        case NORTH:
            next = cell - 1;
            next -> incoming = SOUTH;
            break;
            
        default:
            exit(-1);
            break;
    }
    
    return next;
}


//迷宫寻径算法,在格单元s至t之间规划一条通路(如果的确存在)
bool labyrinth(Cell Laby[LABY_MAX][LABY_MAX],Cell *s,Cell *t){
    if ( (AVAILABLE != s->status) || (AVAILABLE != t -> status)) {
        return false;
    }
    
    Stack<Cell*> path; //用栈记录通路
    s -> incoming = UNKNOWN;
    s -> status = ROUTE;
    path.push(s);  //起点
    
    do{  //从起点出发不断试探、回溯，直到抵达终点，或者穷尽所有可能
        Cell *c = path.top();
        if (c == t) {
            //若已抵达终点，则找到了一条通路;否则，沿尚未试探的方向继续试探
            return true;
        }
        
        while (NO_WAY > ( c -> outgoing = nextESWN(c -> outgoing) )) { //逐一检查所有方向
            if (AVAILABLE == neighbor(c) -> status) {
                break;  //试图找到尚未试探的方向
            }
        }
        
        if (NO_WAY <= c -> outgoing) {  //若所有方向都已尝试过
            c -> status = BACKTRACKED;
            c = path.pop();    //则向后回溯一步
        }else{ //否则，向前试探一步
            path.push(c = advance(c));
            c -> outgoing = UNKNOWN;
            c -> status = ROUTE;
        }
        
    }while(!path.empty());
    
    return false;
}


#endif /* Maze_h */
