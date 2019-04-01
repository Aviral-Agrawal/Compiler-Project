#ifndef __SYN_H__
#define __SYN_H__
#include "syn.h"
#endif

#ifndef __PARSE_STACK_H__
#define __PARSE_STACK_H__
#include "parseStack.h"
#endif

#ifndef __N_ARY_H__
#define __N_ARY_H__
#include "nAry.h"
#endif

typedef struct astNode astNode;
struct astNode{
  char *keyword;
  tokenInfo tk;
  astNode* parent;
  astNode* firstChild;
  astNode* nextSibling;
  int leaf;//1 -> Leaf
};
void createAbstractSyntaxTree(astNode * ast_root, treeNode *p_root);
int isUseless(treeNode *node);
void printASTinFile(astNode* root, FILE* fp);
