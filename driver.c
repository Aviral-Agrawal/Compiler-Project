/*
* Compiler Construction Project
* Driver Functions
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

#ifndef __LEXER_H__
#define __LEXER_H__
#include "lexer.h"
#endif

#include <unistd.h>
#include <limits.h>

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


#ifndef __GMR_CALL_H__
#define __GMR_CALL_H__
#include "gmrCall.h"
#endif

#ifndef __AST_H__
#define __AST_H__
#include "ast.h"
#endif

#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__
#include "symbolTable.h"
#endif

#ifndef __SEMANTIC_H__
#define __SEMANTIC_H__
#include "semantic.h"
#endif

#include <time.h>

treeNode* printSynToken(char* ch, char* ch1);
void removeComments(char *testcaseFile)
{
	FILE* fp = fopen(testcaseFile, "r");
	if(fp==NULL)
	{
		printf("testcaseFile entered not open.\n");
		return;
	}
    FILE* fp2 = fopen("cleanCode.txt", "w");
	if(fp2==NULL)
	{
		printf("cleanFile could not be opened.\n");
		return;
	}

	BUFF = getStream(fp);
	while(BUFF[0] != '\0')
	{
		int bufpos = 0;
		int bufLen = strlen(BUFF);
		//Iterate over the currently filled buffer
		while(bufpos < bufLen)
		{
			//if(comment found) then, skip the remaining portion of the line
			if(BUFF[bufpos] == '%')
			{
				//if(comment is on a new line), then,
				//simply omit it including it's '\n'
				//bufpos == 0 means that the first line of the code is a comment
				//(BUFF[bufpos-1] == '\n') checks if the comment is only thing on this line
				if(bufpos == 0 || BUFF[bufpos-1] == '\n')
				{
					while(BUFF[bufpos] != '\n')
						bufpos++;
					while(BUFF[bufpos] == '\n' || BUFF[bufpos] == '\r')
						bufpos++;
				}
				//if(comment has some code before it on the same line), then,
				//we want to print the '\n' at the end
				else
				{
					while(BUFF[bufpos] != '\n')
						bufpos++;
					while(BUFF[bufpos] == '\n' || BUFF[bufpos] == '\r')
						bufpos++;
					bufpos--;//to be able to print the '\n' of this line
				}
			}
			//if(valid code found) then, print it
			else
				fprintf(fp2,"%c", BUFF[bufpos++]);
		}//End of the while loop
		free(BUFF);
		BUFF = getStream(fp);
	}//End of the while loop
	fclose(fp);
	fclose(fp2);
	char cwd[400];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
	    printf("\nFile Written in : %s/cleanCode.txt\n\n", cwd);
	} else {
     perror("getcwd() error");
     return;
 	}
}//End of the function

void printTime(char* ch, char* ch1)
{
    clock_t    start_time, end_time;
    double total_CPU_time, total_CPU_time_in_seconds;
    start_time = clock();
    // invoke your lexer and parser here
//======================
	printSynToken(ch,ch1);
//======================
    end_time = clock();
    total_CPU_time  =  (double) (end_time - start_time);
    total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
    // Print both total_CPU_time and total_CPU_time_in_seconds
    printf("%lf : TOTAL CPU TIME\n%lf : TOTAL CPU TIME IN SECONDS\n",total_CPU_time*1000,total_CPU_time_in_seconds*1000);
}

void optionPrinter(int* t)
{
	printf("Enter 0 : For Exit\n");
	printf("Enter 1 : For Removing Comments\n");
	printf("Enter 2 : For Invoking Lexer Only\n");
	printf("Enter 3 : For Invoking Lexer and Parser Both\n");
	printf("Enter 4 : See the Execution Time\n");
	printf("Enter 5 : print the AST Tree\n");
	printf("Enter 6 : Call SemanticAnalyzer and Type Checker\n");
	scanf("%d",t);
	// fflush(stdin);
	// fflush(stdout);
}

void printLexToken(char* testcaseFile)
{
	keyTable* kt = newKeywordTable();
    hashTable* ht = newHashTable(INIT_SYM_TAB_LEN);
    populateKeyword(kt);
    FILE* fp = fopen(testcaseFile, "r");
    if(fp == NULL)
    {
        printf("Input file not found for Case 2!!\n");
        return ;
    }
    // int inputSize = strlen(BUFF);
    printTokenInfile(fp, kt, ht);

	fclose(fp);

	//code for printing the errors on the console
	FILE *fp2 = fopen("lexicalError.txt","r");
	if(fp2 == NULL)
		printf("Error in opening file, please check it from lexicalError.txt.\n");
	else
	{
		char deli;
		char* production = (char*) malloc(sizeof(char)*512);
		printf("\nLexical Errors ===>\n");
		// fscanf(fp2,"%[^\n]",production);
		// printf("Testing errors: %s\n", production);
		// fscanf(fp2,"%[^\n]",production);
		// printf("Testing errors: %s\n", production);
		while(fscanf(fp2,"%[^\n]",production)!=EOF)
		{
			printf("%s\n", production);
			fscanf(fp,"%c",&deli);
		}
		free(production);
		fclose(fp2);
	}

	char cwd[400];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
	    printf("\nLexical Error written in : %s/lexicalError.txt\n\n", cwd);
	} else {
     perror("getcwd() error");
 	}

	if (getcwd(cwd, sizeof(cwd)) != NULL) {
	    printf("\nTokens also Written in : %s/outputLex.txt\n\n", cwd);
	} else {
     perror("getcwd() error");
     return;
 	}

}


treeNode* printSynToken(char* ch,char* ch1)
{
	removeBar("old_grammar.txt");
	grammar *r=file_open_one_prod("new_grammar.txt");
	calculateFirstSet(r);
	calculateFollowSet(r);

	   FILE *fp1=fopen(ch,"r");
	   FILE *fp2=fopen(ch1,"w");
	   FILE *fp3=fopen(ch1,"w");
	   if(!fp1 || !fp2 || !fp3)
	   	{
			printf("Error writing files in Option 3\n");
		}

	   treeNode* tree = parse(fp1,r,fp2);
	   printTree(tree,fp3);
	   printf("\n");
	   fclose(fp1);
	   fclose(fp2);
	   fclose(fp3);

	return tree;
}

astNode *printAST(char* outfile, treeNode* p_root)
{
	astNode* ast_root = (astNode*) malloc(sizeof(astNode));
	ast_root->keyword = p_root->keyword;
	ast_root->tk = p_root->tk;
	ast_root->parent = NULL;
	ast_root->nextSibling = NULL;
	ast_root->firstChild = NULL;
	createAbstractSyntaxTree(ast_root, p_root);
	FILE* fpout = fopen(outfile, "w");
	printASTinFile(ast_root, fpout);
	fclose(fpout);
	return ast_root;
}


int main(int argc, char *argv[]) {
	if(argc!=4)
		{
			printf("Error: Too few arguments while executing!!\n");
			return -1;
		}
	printf("\n%20s*************** CURRENT STATUS OF PROJECT ***************\n","");


	astNode *ast_root;
    int t;
	optionPrinter(&t);
    while(t!=0)
    {
		// if(t!=0 || t!=1 || t!=2 || t!= 3 || t!= 4)
		// 	optionPrinter(&t);
        if(t==1)
			removeComments(argv[1]);
        else if(t==2)
            printLexToken(argv[1]);
        else if(t==3)
            printSynToken(argv[1],argv[2]);
        else if(t==4)
            printTime(argv[1],argv[2]);
		else if(t == 5)
		{
			treeNode* p_root = printSynToken(argv[1],argv[2]);
			ast_root = printAST(argv[3], p_root);
		}
		else if(t == 6)
		{
			treeNode* p_root = printSynToken(argv[1],argv[2]);
			astNode* ast_root = (astNode*) malloc(sizeof(astNode));
			ast_root->keyword = p_root->keyword;
			ast_root->tk = p_root->tk;
			ast_root->parent = NULL;
			ast_root->nextSibling = NULL;
			ast_root->firstChild = NULL;
			createAbstractSyntaxTree(ast_root, p_root);
			symbolTable* st = initSymbolTable();
			populateSymbolTable(ast_root,st);
			// printGlobalTable(st);
			// printf("\n");
			// printRecordTable(st);
			// printf("\n");
			// printFunctionTable(st);
			// typeChecker(ast_root,st);
			semanticAnalysis(ast_root,st,"");
		}
		printf("\nEntered option %d done!!\n",t);
		optionPrinter(&t);
		free(BUFF);
    }
	if(t==0)
		printf("Entered 0 : Exiting Successfully!!\n");
    return 0;
}
