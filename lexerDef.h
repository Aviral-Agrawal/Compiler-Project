/*
* Compiler Construction Project
* "lexerDef.h"
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

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>

#define MAX_IDF_LEN 20
// Maximum Identifier length is [2,20]
#define MAX_FUN_LEN 30
// Maximum function length is [2,30]
#define MAX_FIELD_LEN 20
//Assumption :-
//As discussed we have assucmed the maximum length of
//the record field identifier as 20
// Maximum Identifier length is [2,20]
#define MAX_LEX_LEN 512
// Taking into account the undefined length of the function name

//Buffer Allocated for Lexical Analyzer

#define MAX_BUF_LEN 512
//This size is large so as to incorporate
// the undefined size of the function name
char* BUFF;//This is the input buffer

// E-> E+ T
#define MAX_NON_TERMINAL_LEN 32
#define TERMINAL_LEN 50

#define KEYWORD_TABLE_LEN 200
#define INIT_SYM_TAB_LEN 50

//Enum declarartion for the TK_*
typedef enum{//out of these 47 have to be pre-filled in the keyword table
	TK_ERROR,
	TK_ASSIGNOP,
	TK_COMMENT,
	TK_FIELDID,
	TK_ID,
	TK_NUM,
	TK_RNUM,
	TK_FUNID,
	TK_RECORDID,
	TK_WITH,
	TK_PARAMETERS,
	TK_END,
	TK_WHILE,
	TK_TYPE,
	TK_MAIN,
	TK_GLOBAL,
	TK_PARAMETER,
	TK_LIST,
	TK_SQL,
	TK_SQR,
	TK_INPUT,
	TK_OUTPUT,
	TK_INT,
	TK_REAL,
	TK_COMMA,
	TK_SEM,
	TK_COLON,
	TK_DOT,
	TK_ENDWHILE,
	TK_OP,
	TK_CL,
	TK_IF,
	TK_THEN,
	TK_ENDIF,
	TK_READ,
	TK_WRITE,
	TK_RETURN,
	TK_PLUS,
	TK_MINUS,
	TK_MUL,
	TK_DIV,
	TK_CALL,
	TK_RECORD,
	TK_ENDRECORD,
	TK_ELSE,
	TK_AND,
	TK_OR,
	TK_NOT,
	TK_LT,
	TK_LE,
	TK_EQ,
	TK_GT,
	TK_GE,
	TK_NE,
	TK_DOLLAR
}token;


typedef struct {
	char lexeme[MAX_LEX_LEN];
	token tkType;
	unsigned int lineNo;
	//NUMERIC_VALUE was here!!
}tokenInfo;

// KEYWORD TABLE INTERFACE
typedef struct symNode symNode;
struct symNode{
    char* keyword;// To compare the keywords
    token tk;
    symNode *next; // A pointer to below (Linear Chaining Mechanism)
};

typedef struct{
    int keywordCount;
    int size;// Size of the value array
    symNode *value;
}keyTable;

typedef struct tableCell tableCell;
struct tableCell{
	token t;
	char* lexPtr;
	tableCell* next;//To maintain the linked list
};

typedef struct{
	int size;
	int elementCount;
	tableCell* table;//note that the access to the element of the
	//table would be like ht->table[i].next = Pointer to the next
}hashTable;
