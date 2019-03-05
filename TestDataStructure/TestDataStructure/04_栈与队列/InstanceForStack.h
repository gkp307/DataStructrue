//
//  InstanceForStack.hpp
//  TestCPPDataStructure
//
//  Created by gkp on 2018/7/27.
//  Copyright © 2018年 51iob. All rights reserved.
//

#ifndef InstanceForStack_hpp
#define InstanceForStack_hpp

#include "Stack.hpp"

//1.栈
/*
 栈中可操作的一端更多的称作栈顶，而另一无法直接操作的盲端则更多的称作栈底
 */

//1.1 栈的典型应用
//1.1.1 逆序输出
//十进制正整数n到base进制的转换(递归版)
void convert(Stack<char> &S,int n,int base) {
    static char digit[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    if (0 < n) {
        S.push(digit[n%base]);
        convert(S, n/base, base);
    }
}//新进制下由高到低的各数位，自顶向下保存与栈中

//十进制正整数n到base进制的转换(迭代版)
void convert1(Stack<char> &S,int n,int base){
    static char digit[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    while (n > 0) {
        int remainder = (int)(n%base);
        S.push(digit[remainder]);
        n /= base;
    }
}//新进制下由高到低的各数位，自顶向下保存与栈中


//1.1.2 递归嵌套

//栈混洗

//1.1.2.1 括号匹配

//1)递归实现
//删除exp[lo,hi]不含括号的最长前缀、后缀
void trim(const char exp[],int& lo,int& hi) {
    while ( (lo <= hi) && (exp[lo] != '(') && exp[lo] != ')' ){
        lo++;
    }
    while ( (lo <= hi) && (exp[hi] != '(') && exp[hi] != ')' ) {
        hi--;
    }
}

//切分exp[lo,hi],使exp匹配仅当子表达式匹配
int divide(const char exp[],int lo,int hi){
    int mi = lo;
    int crc = 1; //crc为[lo,mi]范围内左、右括号数目之差
    while ( (0 < crc) && (++mi < hi)) {  //逐个检查各字符，知道左右括号数目相等，或者越界
        if (exp[mi] == ')') {
            crc--;
        }
        if (exp[mi] == '(') {
            crc++;
        }
    }
    
    return mi;//若mi <= hi,则为合法切分点；否则，意味着局部不可能匹配
}


//parentheses
bool paren(const char exp[], int lo, int hi) { //检查表达式exp[lo,hi]是否括号匹配(递归版)
    trim(exp, lo, hi);
    if (lo > hi) {
        return true;  //清除不含括号的前缀、后缀
    }
    
    if (exp[lo] != '(') {
        return false;   //首字符非左括号，则必不匹配
    }
    
    if (exp[hi] != ')') {
        return false;   //末字符非右括号，则必不匹配
    }
    
    int mi = divide(exp, lo, hi); //确定适当的切分点
    if (mi > hi) {
        return false;   //切分点不合法，意味着局部以致整体不匹配
    }
    
    return paren(exp, lo+1, mi-1) && paren(exp, mi+1, hi);
    
}

//2)迭代实现
//表达式括号匹配，可兼顾三种括号
bool paren2(const char exp[],int lo,int hi) {
    Stack<char> S;
    
    for (int i = lo; i <= hi; i++) {
        switch (exp[i]) { //左括号直接进栈;右括号若与栈顶失配，则表达式必不匹配
            case '(':
            case '[':
            case '{':
                S.push(exp[i]);
                break;
                
            case ')':
                if ( (S.empty()) || ('(' != S.pop()) ) {
                    return false;
                }
                break;
                
            case ']':
                if ( (S.empty()) || ('[' != S.pop())) {
                    return false;
                }
                break;
                
            case '}':
                if ( (S.empty()) || ('{' != S.pop())) {
                    return false;
                }
                break;
                
            default:
                break;  //非括号字符一律忽略
        }
    }
    
    return S.empty();//整个表达式扫描过后，栈中若仍残留(左)括号，则不匹配；否则(栈空)匹配
}

//1.1.3 延迟缓冲

//表达式求值
#define N_OPTR 9  //运算符总数
typedef enum {
    ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE
    //加、减、乘、除、乘方、阶乘、左括号、右括号、起始符与终止符
}Operator;

const char pri[N_OPTR][N_OPTR] = {
    /*                      |----------当前运算符-----------|       */
    /*               +     -    *    /    ^    !    (    )   \0   */
    /*--*//* + */    '>', '>', '<', '<', '<', '<', '<', '>', '>',
    /*|*//*  - */    '>', '>', '<', '<', '<', '<', '<', '>', '>',
    /*栈*//*  * */   '>', '>', '>', '>', '<', '<', '<', '>', '>',
    /*顶*//*  / */   '>', '>', '>', '>', '<', '<', '<', '>', '>',
    /*运*//*  ^ */   '>', '>', '>', '>', '>', '<', '<', '>', '>',
    /*算*//*  ! */   '>', '>', '>', '>', '>', '>', ' ', '>', '>',
    /*符*//*  ( */   '<', '<', '<', '<', '<', '<', '<', '=', ' ',
    /*| *//*  ) */   ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    /*--*//*  \0 */  '<', '<', '<', '<', '<', '<', '<', ' ', '=',
    
};

//求值算法
void readNumber(char*& p,Stack<float>& stk){
    stk.push((float)(*p - '0'));  //当前数位对应的数值进栈
    
    while (isdigit( *(++p) )) {
        stk.push(stk.pop() * 10 + (*p - '0') );
    }
    
    if ('.' != *p) {   //此后非小数点，则意味着当前操作数解析完成
        return;
    }
    
    float fraction = 1; //否则，意味着还有小数部分
    while (isdigit( *(++p) )) {
        stk.push( stk.pop() + (*p - '0') * (fraction /= 10) );  //小数部分
    }
}

//将操作数接至RPN末尾
void append(char *& rpn,float opnd){
    int n = strlen(rpn); //RPN当前长度(以'\0'结尾，长度n+1)
    char buf[64];
    
    if (opnd != (float)(int)opnd) {
        sprintf(buf, "%.2f \\0" ,opnd);   //浮点格式
    }else{
        sprintf(buf, "%d \\0", (int)opnd);  //整数格式
    }
    
    rpn = (char*) realloc(rpn, sizeof(char) * (n + strlen(buf) + 1));
    strcat(rpn, buf);  //RPN加长
}

//将运算符接至RPN末尾
void append(char*& rpn,char optr) {
    int n = strlen(rpn);
    rpn = (char *)realloc(rpn, sizeof(char) * (n + 3));
    sprintf(rpn + n, "%c ", optr);
    rpn[n+2] = '\0';
}


Operator optr2rank(char op) {
    switch (op) {
        case '+': return ADD;
        case '-': return SUB;
        case '*': return MUL;
        case '/': return DIV;
        case '^': return POW;
        case '!': return FAC;
        case '(': return L_P;
        case ')': return R_P;
        case '\0': return EOE;  //起始符和终止符
            
        default: exit(-1);
    }
}

char orderBetween(char op1, char op2){
    return pri[optr2rank(op1)][optr2rank(op2)];
}

float calcu(char op,float f){
    return 0;
}

float calcu(float f1,char c,float f2){
    return 0;
}

//对（已删除白空格的）表达式S求值，并转换为逆波兰式RPN
float evaluate(char *S,char *& RPN) {
    //运算数栈、运算符栈
    Stack<float> opnd;
    Stack<char> optr;
    
    optr.push('\0');  //尾哨兵'\0'也作为头哨兵首先入栈
    while (!optr.empty()) {   //在运算符栈非空之前，逐个处理表达式中各字符
        if (isdigit(*S)) {
            //读入操作数，并将其接至RPN末尾
            readNumber(S, opnd);
            append(RPN, opnd.pop());
        }else{
            //若当前字符为运算符
            switch (orderBetween(optr.top(), *S)) {
                case '<':  //栈顶运算符优先级更低时
                    optr.push(*S);
                    S++; //计算推迟，当前运算符进栈
                    break;
                    
                case '=': //优先级相等(当前运算符为右括号或者尾部哨兵'\0')时
                    optr.pop();
                    S++;  //脱括号并接受下一个字符
                    break;
                    
                case '>': {//栈顶运算符优先级更高时，可实施相应的计算，并将结果重新入栈
                    char op = optr.pop();
                    append(RPN, op); //栈顶运算符出栈并续接至RPN末尾
                    
                    if ('!' == op) {
                        //如属于一元运算符，只需要取出一个操作数并实施医院计算，结果入栈
                        float pOpnd = opnd.pop();
                        opnd.push(calcu(op, pOpnd));
                    }else{
                        //对于其他(二元)运算符
                        float pOpnd2 = opnd.pop(), pOpnd1 = opnd.pop();
                        opnd.push(calcu(pOpnd1, op, pOpnd2));
                    }
                    break;
                }
                    
                default:
                    exit(-1);  //逢语法错误，不做处理直接退出
                    break;
            }
        }
    }
    
    return opnd.pop(); //弹出并返回最后的计算结果
}




//1.1.4 逆波兰表达式

#endif /* InstanceForStack_hpp */
