/*
* Compiler Construction Project
* "lexer.h"
* Group Number 40
* AVIRAL AGRAWAL 2016A7PS0077P
* JITVAN SHUKLA  2016A7PS0083P
* SHUBHAM SHARMA 2016A7PS0115P
* AKSHIT AGARWAL  2016A7PS0122P
*
* Date Created : 24_02_2019
* Last Modified : 24_02_2019 18:15:00 PM
* User : Akshit Agarwal
*/
#ifndef __LEXER_DEF_H__
#define __LEXER_DEF_H__
#include "lexerDef.h"
#endif

keyTable* newKeywordTable();
void insertKeyword(keyTable *kt, char* keyword, token tk);
int symHasher(char* keyword,int mod);
void hashPrinter(char* keyword,int mod);
token lookKeyword(keyTable* kt, char* keyword);
//Returns TK_ERROR is keyword not found
void populateKeyword(keyTable* kt);

//Hash table functions
int hash(char* lexeme, hashTable* ht);
hashTable* newHashTable(int size);//Initial size of the table to be 50
hashTable* insertInTable(char* lexeme, token tk, hashTable* ht);
hashTable* reSize(hashTable* ht);//doubles the existing size
token lookInHashTable(char* lexeme, hashTable* ht);
void printHashTable(hashTable* ht);


//Lexer functions
char* getStream(FILE* fp);
void getNextToken(tokenInfo *t, keyTable* kt, hashTable* ht, FILE* fp, FILE* fp2);
token lookup(char* search, int length, keyTable* kt);// Returns the token
// RETURNS TK_ERROR if Value not Found
hashTable* insert(char* lexeme, token tk, hashTable* ht);
char* enumToString(token);

int stringToInt(tokenInfo *tk);
float stringToReal(tokenInfo *tk);
int stringToInt(tokenInfo *tk);
void printNum(tokenInfo* tk);
void printTokenInfile(FILE* fp, keyTable* kt, hashTable* ht);
