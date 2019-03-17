/*
* Compiler Construction Project
* Group Number 40
* AVIRAL AGRAWAL 2016A7PS0077P
* JITVAN SHUKLA  2016A7PS0083P
* SHUBHAM SHARMA 2016A7PS0115P
* AKSHIT AGARWAL  2016A7PS0122P

*/
#ifndef __LEXER_H__
#define __LEXER_H__
#include "lexer.h"
#endif
#ifndef __LEXER_DEF_H__
#define __LEXER_DEF_H__
#include "lexerDef.h"
#endif

#define STACK_CAPACITY 20

typedef struct stackNode stackNode;
struct stackNode{
    char* keyword;
};

typedef struct stackParse stackParse;
struct stackParse{
    stackNode* node;
    int top;
    int capacity;
};

stackParse* createStack();
int isFull(stackParse* stk);
int isEmpty(stackParse* stk);
void growStack(stackParse* stk);
void push(stackParse* stk, char* keyword);
char* pop(stackParse* stk);
char* peek(stackParse* stk);
void deleteStack(stackParse* stk);
