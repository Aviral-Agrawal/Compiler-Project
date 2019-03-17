/*
* Compiler Construction Project
* Group Number 40
* AVIRAL AGRAWAL 2016A7PS0077P
* JITVAN SHUKLA  2016A7PS0083P
* SHUBHAM SHARMA 2016A7PS0115P
* AKSHIT AGARWAL  2016A7PS0122P

*/
#ifndef __PARSE_STACK_H__
#define __PARSE_STACK_H__
#include "parseStack.h"
#endif

stackParse* createStack()
{
    stackParse* stk = (stackParse*) malloc(sizeof(stackParse));
    if(!stk)
        return NULL;
    stk->top=-1;
    stk->capacity=STACK_CAPACITY;
    stk->node = (stackNode*) malloc(sizeof(stackNode)*stk->capacity);
    if(!stk->node)
        return NULL;
    int i;
    for(i=0;i<stk->capacity;i++)
        stk->node[i].keyword = '\0';
    return stk;
}

int isFull(stackParse* stk)
{
    return (stk->top==stk->capacity-1);
}

int isEmpty(stackParse* stk)
{
    if(stk->top==-1)
        return 1;
    return 0;
}

void growStack(stackParse* stk)
{
    // printf("Stack is Doubling to :%d\n",stk->capacity*2);
    stk->capacity *=2;
    stk->node = realloc(stk->node,sizeof(stackNode)*stk->capacity);
}

void push(stackParse* stk, char* keyword)
{
    if(isFull(stk))
        growStack(stk);
    stk->node[++(stk->top)].keyword=keyword;
}

char* pop(stackParse* stk)
{
    if(isEmpty(stk))
        return NULL;
    return stk->node[stk->top--].keyword;
}

char* peek(stackParse* stk)
{
    return stk->node[stk->top].keyword;
}

void deleteStack(stackParse* stk)
{
    if(stk)
    {
        if(stk->node)
            free(stk->node);
            // printf("Deleteing StackNodes ... Done!!\n");}
        free(stk);
    // printf("Deleting Stack Struct... Done!!\n");
    }
}


// int main(int argc, char const *argv[]) {
//     stackParse* stk = createStack();
//     printf("%d is empty\n",isEmpty(stk));
//     push(stk,"Akshit");
//     push(stk,"Aviral");
//     push(stk,"Shubham");
//     push(stk,"Jitvan");
//     printf("%d is Full\n",isFull(stk));
//     push(stk,"Compiler");
//     printf("%d is empty\n",isEmpty(stk));
//     printf("%s : PEEK\n",peek(stk));
//     pop(stk);
//     printf("%s : PEEK\n",peek(stk));
//     pop(stk);
//     printf("%s : PEEK\n",peek(stk));
//     pop(stk);
//     printf("%s : PEEK\n",peek(stk));
//     pop(stk);
//     printf("%s : PEEK\n",peek(stk));
//     pop(stk);
//     printf("%d is empty\n",isEmpty(stk));
//     deleteStack(stk);
//     return 0;
// }
