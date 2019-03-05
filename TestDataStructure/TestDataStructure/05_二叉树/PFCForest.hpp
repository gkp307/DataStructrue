//
//  PFCForest.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef PFCForest_hpp
#define PFCForest_hpp

//Huffman编码

#include "BinTree.hpp"
#include "Vector.hpp"
#include "Bitmap.h"
#include "Skiplist.hpp"

typedef BinTree<char> PFCTree;  //PFC树

typedef Vector<PFCTree*> PFCForest; //PFC森林

typedef Skiplist<char, char*> PFCTable;  //PFC编码表 (key=字符,value=编码串)


//初始化PFC森林
PFCForest* initForest() {
    PFCForest *forest = new PFCForest;
    for (int i = 0; i < N_CHAR; i++) {
        forest -> insert(i, new PFCTree());
        (*forest)[i] -> insertAsRoot(0x20 + i);
    }
    return forest;
}

//构造PFC编码树
PFCTree *generateTree(PFCForest *forest){
    srand((unsigned int) time(NULL));
    
    while (1 < forest -> size()) {
        PFCTree *s = new PFCTree;
        s -> insertAsRoot('^');
        Rank r1 = rand() % forest -> size();
        s -> attachAsLC(s -> root(), (*forest)[r1]);
        forest -> remove(r1);
        Rank r2 = rand() % forest -> size();
        s -> attachAsRC(s -> root(), (*forest)[r2]);
        forest -> remove(r2);
        forest -> insert(forest->size(), s);
    }
    
    return (*forest)[0];
}

//生成PFC编码表
void generateCT(Bitmap* code,int length,PFCTable *table,BinNodePosi(char) v){
    if (IsLeaf(*v)) {
        table -> put(v->data, code->bits2string(length));
        return;
    }
    
    if (HasLChild(*v)) {
        code -> clear(length);
        generateCT(code, length+1, table, v -> lc);
    }
    
    if (HasRChild(*v)) {
        code -> set(length);
        generateCT(code, length+1, table, v -> rc);
    }
}

PFCTable* generateTable(PFCTree* tree) {
    PFCTable *table = new PFCTable;
    Bitmap *code = new Bitmap;
    generateCT(code, 0, table, tree->root());
    //    release(code);
    delete code;
    
    return table;
}


//编码
int encode(PFCTable *table,Bitmap &codeString,const char *s){
    int n = 0;
    for (size_t m = strlen(s),i = 0; i < m; i++) {
        char **pCharCode = table->get(s[i]);
        if (!pCharCode) {
            pCharCode = table -> get(s[i] + 'A' - 'a');
        }
        if (!pCharCode) {
            pCharCode = table -> get(' ');
        }
        printf("%s", *pCharCode);
        
        for (size_t m = strlen(*pCharCode),j = 0; j < m; j++) {
            '1' == *(*pCharCode + j) ? codeString.set(n++) : codeString.clear(n++);
        }
    }
    
    return n;
}

//解码
void decode(PFCTree *tree,Bitmap& code,int n) {
    BinNodePosi(char) x = tree -> root();
    for (int i = 0; i < n; i++) {
        x = code.test(i) ? x->rc : x ->lc;
        if (IsLeaf(*x)) {
            printf("%c", x->data);
            x = tree -> root();
        }
    }
}

#endif /* PFCForest_hpp */
