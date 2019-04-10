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

#ifndef __AST_H__
#define __AST_H__
#include "ast.h"
#endif

#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__
#include "symbolTable.h"
#endif


typedef struct whileVariables whileVariables;
struct whileVariables{
  char* keyword;
  whileVariables* next;
};

void addVariable(whileVariables* wv, char* keyword);
void fillWhileVariables(whileVariables* wv ,astNode* root,funTable* ft);
int findFunNameOcc(funTable* ft, char* funName);
funTable* getFunctionEntryInTable(funTable* fHead, char* lexeme);
idfTable* findIdEntry(idfTable* localVarTable, char* lexeme);//returns an entry of the idfTable;
void semanticAnalysis(astNode* root, symbolTable* st, char* functionName);
