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

#ifndef __N_ARY_H__
#define __N_ARY_H__
#include "nAry.h"
#endif

#include <unistd.h>
#include <limits.h>

#define INIT_GMR_LEN 200
//Global declarations


// void removeComments(char *testcaseFile);
void pop_grammar(grammar* r);
grammar* file_open_one_prod(char file_name[32]);
ruleHead* make_livl(char* token,ruleHead* head,int c);
void extrac(char* rule,grammar* r,int cur_rule);
void removeBar(char* file_name);
