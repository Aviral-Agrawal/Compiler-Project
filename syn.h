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

#define MAX_FOLLOW_SIZE 200
#define MAX_FIRST_SIZE 200
#define PARSE_TABLE_SIZE 200

#define NO_OF_NONTERMINALS 100
#define NO_OF_TERMINALS 100
// defining parse table

//Global var for parse tables
// int tCount,ntCount;

typedef struct ft ft;
struct ft{
    char terminal[TERMINAL_LEN];
    ft* next;
};

typedef struct firstSetArray firstSetArray;
struct firstSetArray{
    ft* firstSet;// Array containg the name of non-terminals
    int size;//Init -> 200
    int count;//Current No. of differnet terminals
};

typedef struct fl fl;
struct fl{
    char terminal[TERMINAL_LEN];
    fl* next;
};

typedef struct followSetArray followSetArray;
struct followSetArray{
    fl* followSet;// Array containg the name of non-terminals
    int size;//Init -> 200
    int count;//Current No. of differnet terminals
};

//terminal or non terminal or operator
typedef struct ruleHead ruleHead;
struct ruleHead
{
char nonTermName[MAX_NON_TERMINAL_LEN];
ruleHead* next;
};

typedef struct{
    ruleHead* nonTerm;// Array
    int prodCount;
    firstSetArray *ftsa;
    followSetArray *flsa;
    int* tokenCount;
}grammar;
//grammar rules
//Interface for parser
int isEpsilon(char* keyword);
int isTerminal(char* keyword);

// Returns the first set of the non terminal
ft* getFirst(grammar *gmr, char* keyword);
void insertFirst(grammar *gmr, char *keyword);
void insertFirstHelper(grammar *gmr, ruleHead *rh, char *keyword, int flag);

fl* getFollow(grammar *gmr, char* keyword);
void insertFollow(grammar *gmr, char *keyword);
//This is a recusive function
void insertFollowHelper(grammar *gmr, ruleHead *rh, char* keyword, char* ntHead);



//Fomation of the outstanding follow LIST Structure
typedef struct list list;
struct list{
    char first[MAX_NON_TERMINAL_LEN];
    char second[MAX_NON_TERMINAL_LEN];
    list* next;
};

//List functions
void formNewList();
int strInList(char* curr, char* toBeIncluded);
void insertInList(char* curr, char* toBeIncluded);
void deleteFromList(char* curr, char* toBeIncluded);
void mergeFollow(grammar* gmr, char* curr, char* toBeIncluded);
void printfollow(grammar* gmr, char* keyword);
void printFirst(grammar *gmr, char* keyword);
void insert1(ruleHead *rh, char *keyword);
void calculateFirstSet(grammar* gmr);
void calculateFollowSet(grammar* gmr);

//Parse Table structs
typedef struct parNode parNode;
struct parNode{
    char* keyword;
    int hid;//HASH ID (Init to -1)
    parNode* next;//For Chaining
};

typedef struct parseHash parseHash;
struct parseHash{
    parNode* parData;
    int count;
    int size;
    int tCount;
    int ntCount;
};
int parseHasher(char* keyword,int mod);

int belongsInFollow(grammar *gmr, char *terminal, char *non_terminal);

//Update
int getSetParseIndex(parseHash* ph, char* keyword);
// // int parseTable[NO_OF_NONTERMINALS][NO_OF_TERMINALS];
// int** parseTable;
void getToken(tokenInfo *tk);
void parseHashPrint(parseHash* ph);

// temporary struct and function for parse table index

typedef struct term term;
struct term{
  char keyword[TERMINAL_LEN];
  int index;
  term *next;
};
typedef struct nonTerm nonTerm;
struct nonTerm{
  char keyword[MAX_NON_TERMINAL_LEN];
  int index;
  nonTerm *next;
};

typedef struct dict dict;
struct dict{
  term *tm;
  nonTerm *nm;
  int tCount;
  int ntCount;
};

typedef struct treeNode treeNode;
struct treeNode{
    char* keyword;
    tokenInfo tk;//Update
    treeNode* parent;
    treeNode* firstChild;
    treeNode* nextSibling;
    int leaf;//1 -> Leaf
};

void printParseTable(dict *dt);
int getSetIndex(dict *dt, char *keyword);
// int parseTable[NO_OF_NONTERMINALS][NO_OF_TERMINALS];
int** parseTable;
parseHash *ph;
treeNode* parse(FILE *fp, grammar *gmr, FILE *fp2);
