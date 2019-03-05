//
//  Huffman.h
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef Huffman_h
#define Huffman_h


#include "BinTree.hpp"
#include "List.hpp"
#include "Bitmap.h"
#include "HashTable.hpp"

//Huffman编码树

struct HuffChar{
    char ch;
    int weight;
    HuffChar(char c = '^',int w = 0) : ch(c), weight(w){
        
    }
    
    bool operator<(HuffChar const& hc){
        return weight > hc.weight;
    }
    
    bool operator==(HuffChar const& hc) {
        return weight == hc.weight;
    }
};

#define HuffTree BinTree<HuffChar>

typedef List<HuffTree*> HuffForest;

typedef Bitmap HuffCode;

typedef HashTable<char, char*> HuffTable;

//字符出现频率的样本统计
int* statistics(const char *sample_text_file) {
    int *freq = new int[N_CHAR];
    memset(freq, 0, sizeof(int) * N_CHAR);
    
    FILE *fp = fopen(sample_text_file, "r");
    
    for (char ch; 0 < fscanf(fp, "%c", &ch); ) {
        if (ch >= 0x20) {
            freq[ch-0x20]++;
        }
    }
    
    fclose(fp);
    return freq;
}


HuffForest *initForest(int *freq) {
    HuffForest *forest = new HuffForest;
    
    for (int i = 0; i < N_CHAR; i++) {
        forest -> insertAsLast(new HuffTree);
        forest -> last() -> data -> insertAsRoot(HuffChar(0x20+i, freq[i]));
    }
    return forest;
}


HuffTree *minHChar(HuffForest *forest){
    ListNodePosi(HuffTree*) p = forest -> first();
    ListNodePosi(HuffTree*) minChar = p; //最小Huffman树所在的节点
    
    int minWeight = p -> data->root()->data.weight; //目前的最小权重
    
    while (forest -> valid(p = p -> succ)) {
        if (minWeight > p ->data -> root() -> data.weight) {
            minWeight = p ->data -> root() -> data.weight;
            minChar = p;
        }
    }
    
    return forest->remove(minChar);
}

HuffTree *generateTree(HuffForest *forest) {
    while (1 < forest->size()) {
        HuffTree *T1 = minHChar(forest);
        HuffTree *T2 = minHChar(forest);
        
        HuffTree *S = new HuffTree();
        S -> insertAsRoot(HuffChar('^', T1->root()->data.weight + T2->root()->data.weight));
        S -> attachAsLC(S -> root(), T1);
        S -> attachAsRC(S -> root(), T2);
        
        forest -> insertAsLast(S);
    }
    
    return forest->first()->data;
}

void generateCT(Bitmap* code,int length,HuffTable *table,BinNodePosi(HuffChar) v){
    if (IsLeaf(*v)) {
        table -> put(v->data.ch, code->bits2string(length));
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

HuffTable *generateTable(HuffTree *tree) {
    HuffTable *table = new HuffTable;
    Bitmap *code = new Bitmap;
    generateCT(code, 0, table, tree->root());
    delete code;
    return table;
}

//编码
int encode(HuffTable *table,Bitmap *codeString,const char *s){
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
        
        for (size_t m = strlen(*pCharCode),j = 0; j < m; i++) {
            '1' == *(*pCharCode + j) ? codeString->set(n++) : codeString->clear(n++);
        }
    }
    
    return n;
}

//解码
void decode(HuffTree *tree,Bitmap* code,int n) {
    BinNodePosi(HuffChar) x = tree -> root();
    for (int i = 0; i < n; i++) {
        x = code -> test(i) ? x->rc : x ->lc;
        if (IsLeaf(*x)) {
            printf("%c", x->data.ch);
            x = tree -> root();
        }
    }
}


#endif /* Huffman_h */
