/*
* Compiler Construction Project
* Group Number 40
* AVIRAL AGRAWAL 2016A7PS0077P
* JITVAN SHUKLA  2016A7PS0083P
* SHUBHAM SHARMA 2016A7PS0115P
* AKSHIT AGARWAL  2016A7PS0122P

*/
#ifndef __SYN_H__
#define __SYN_H__
#include "syn.h"
#endif

treeNode* createTreeNode(char* keyword, treeNode* parent);
treeNode* locateNextNonTerminal(treeNode* curNode);
//Updated
void printTree(treeNode* root, FILE* fp);
void printParseTree(treeNode* root,char* outfile);
