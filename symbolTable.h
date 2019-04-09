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

#define NUM_WIDTH 4
#define RNUM_WIDTH 8

typedef struct symbolTable symbolTable;
typedef struct funTable funTable;
typedef struct recTable recTable;
typedef struct globTable globTable;
typedef struct idfTable idfTable;

struct symbolTable
{
  funTable *fHead;
  recTable *rHead;
  globTable *gHead;//Global Table
};

struct funTable{
  char keyword[32];
  idfTable *inputParams;
  idfTable *outputParams;
  idfTable *localVariable;
  funTable *next;
};

struct recTable{
  char keyword[32];
  idfTable *fields;
  recTable *next;
};

struct globTable{
  char keyword[32];
  int width;
  int offset;
  int nType;// 0 -NUM 1-RNUM 2-RECORD
  idfTable *extraData;
  globTable *next;//Points to next entry in the global Table
};

struct idfTable{
  char keyword[32];
  int width;
  int offset;
  int nType;// 0 -NUM 1-RNUM 2-RECORD
  idfTable *extraData;//Data for While and If Blocks Variables
  idfTable *next;//Points to next entry in the function Table
};

symbolTable* initSymbolTable();
funTable* initFunTable();
recTable* initRecTable( );
idfTable* initIdfTable();
globTable* initGlobalTable();
void populateSymbolTable(astNode *root,symbolTable* st);
void printGlobalTable(symbolTable* st);
void printRecordTable(symbolTable* st);
void printFunctionTable(symbolTable* st);
void populateRecordTable(astNode *root,symbolTable* st);
void populateGlobalTable(astNode *root,symbolTable* st);
void populateFunctionTable(astNode *root,symbolTable* st);
funTable *findFunction(symbolTable *st, char *func_name);
int findIdentifier(idfTable *it, char *id); // returns ntype -1 if not found
int findIdinGlobal(symbolTable *st, char *id); // returns ntype -1 if not found
idfTable *findRecordFields(symbolTable *st, char *rec_name);
int typeChecker(astNode *root, symbolTable *st);
