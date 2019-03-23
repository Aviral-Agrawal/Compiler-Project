
/*
* Compiler Construction Project
* "lexer.c"
* Group Number 40
* AVIRAL AGRAWAL 2016A7PS0077P
* JITVAN SHUKLA  2016A7PS0083P
* SHUBHAM SHARMA 2016A7PS0115P
* AKSHIT AGARWAL  2016A7PS0122P
*
* Date Created : 24_02_2019
* Last Modified : 25_02_2019 18:15:00 PM
* User : Akshit Agarwal
*/

//Keyword TABLE IMPLEMENTATION
// Keyword Table to populate the keyword table

// [][][][][][][][] Linked List Structure
//     []      []
//     []

#ifndef __LEXER_DEF_H__
#define __LEXER_DEF_H__
#include "lexerDef.h"
#endif

#ifndef __LEXER_H__
#define __LEXER_H__
#include "lexer.h"
#endif
keyTable* newKeywordTable()
{
    keyTable* kt = (keyTable*) malloc(sizeof(keyTable));
    kt->size = 0;
    kt->keywordCount = 0;
    kt->value = NULL;
    return kt;
}

void hashPrinter(char* keyword,int mod)
{
    char *p = keyword;
    unsigned h = 0x811c9dc5;
    int i;
    int len = strlen(keyword);
    for ( i = 0; i < len; i++ )
      h = ( (h%mod) ^ p[i] )%mod * 0x01000193;
    printf("%s %d\n",keyword,h%mod);
}

int symHasher(char* keyword,int mod)
{// Function for hashing table keywords

    char *p = keyword;
    unsigned h = 0x811c9dc5;
    int i;
    int len = strlen(keyword);
    //Standard Hash Function
    for ( i = 0; i < len; i++ )
      h = ( (h%mod) ^ p[i] )%mod * 0x01000193;
    return h%mod;

}

void populateKeyword(keyTable* kt)
{
    insertKeyword(kt,"<---",TK_ASSIGNOP);
    insertKeyword(kt,"%",TK_COMMENT);
    insertKeyword(kt,"with",TK_WITH);
    insertKeyword(kt,"parameters",TK_PARAMETERS);
    insertKeyword(kt,"end",TK_END);
    insertKeyword(kt,"while",TK_WHILE);
    insertKeyword(kt,"type",TK_TYPE);
    insertKeyword(kt,"_main",TK_MAIN);
    insertKeyword(kt,"global",TK_GLOBAL);
    insertKeyword(kt,"parameter",TK_PARAMETER);
    insertKeyword(kt,"list",TK_LIST);
    insertKeyword(kt,"[",TK_SQL);
    insertKeyword(kt,"]",TK_SQR);
    insertKeyword(kt,"input",TK_INPUT);
    insertKeyword(kt,"output",TK_OUTPUT);
    insertKeyword(kt,"int",TK_INT);
    insertKeyword(kt,"real",TK_REAL);
    insertKeyword(kt,",",TK_COMMA);
    insertKeyword(kt,";",TK_SEM);
    insertKeyword(kt,":",TK_COLON);
    insertKeyword(kt,".",TK_DOT);
    insertKeyword(kt,"endwhile",TK_ENDWHILE);
    insertKeyword(kt,"(",TK_OP);
    insertKeyword(kt,")",TK_CL);
    insertKeyword(kt,"if",TK_IF);
    insertKeyword(kt,"then",TK_THEN);
    insertKeyword(kt,"endif",TK_ENDIF);
    insertKeyword(kt,"read",TK_READ);
    insertKeyword(kt,"write",TK_WRITE);
    insertKeyword(kt,"return",TK_RETURN);
    insertKeyword(kt,"+",TK_PLUS);
    insertKeyword(kt,"-",TK_MINUS);
    insertKeyword(kt,"*",TK_MUL);
    insertKeyword(kt,"/",TK_DIV);
    insertKeyword(kt,"call",TK_CALL);
    insertKeyword(kt,"record",TK_RECORD);
    insertKeyword(kt,"endrecord",TK_ENDRECORD);
    insertKeyword(kt,"else",TK_ELSE);
    insertKeyword(kt,"&&&",TK_AND);
    insertKeyword(kt,"@@@",TK_OR);
    insertKeyword(kt,"~",TK_NOT);
    insertKeyword(kt,"<",TK_LT);
    insertKeyword(kt,"<=",TK_LE);
    insertKeyword(kt,"==",TK_EQ);
    insertKeyword(kt,">",TK_GT);
    insertKeyword(kt,">=",TK_GE);
    insertKeyword(kt,"!=",TK_NE);
}

void insertKeyword(keyTable *kt, char* keyword, token tk)
{

    //Memory Allocation
    if(kt->size==0)
    {// This is initialising Sufficient Size
        kt->size=KEYWORD_TABLE_LEN;//Defined in Lex.h
        kt->keywordCount = 0;
        kt->value = (symNode*) malloc(sizeof(symNode)*kt->size);
        int i=0;
        for(i=0;i<kt->size;i++)
        {
            kt->value[i].keyword=NULL;
            kt->value[i].tk=0;
            kt->value[i].next=NULL;
        }
    }

    //Check if already there
    int idx = symHasher(keyword,kt->size);//Gets the hash value of keyword
    // hashPrinter(keyword,kt->size);
    token check = lookKeyword(kt,keyword);
    if(check!=TK_ERROR)
        return;

    // if(kt->size>=idx)
    //     {
    //         kt->value[idx].tk = tk;
    //         kt->value[idx].keyword = keyword;
    //     }
    // else
    // {
    //     //If want to realloc, then loop for inserting everything
    //     // kt->size*=2;
    //     // kt->value = (symNode*) realloc(kt->value,sizeof(symNode)*kt->size);
    //     kt->value[idx].tk = tk;
    //     kt->value[idx].keyword = keyword;
    // }

    //Checking in the linked List
    if(kt->value[idx].tk==TK_ERROR)// if empty pos
        {
            kt->value[idx].tk = tk;
            kt->value[idx].keyword = keyword;
            kt->value[idx].next=NULL;
        }
    else
    {
        symNode* node = (symNode*) malloc(sizeof(symNode));
        node->keyword = keyword;
        node->tk=tk;
        node->next=NULL;

        symNode* temp = kt->value[idx].next,*prev=&kt->value[idx];
        // printf("CHAINNNNNNN\n");
        while(temp!=NULL)
        {
            prev=temp;
            temp = temp->next;
            // printf("CHAINNNNNNN\n");
        }
        prev->next = node;
    }

    kt->keywordCount++;
}

token lookKeyword(keyTable* kt, char* keyword)
{
    int idx = symHasher(keyword,kt->size);
    symNode *temp = kt->value;
    symNode val = temp[idx];
    // First Case Checking
    if(val.tk==TK_ERROR)
        return TK_ERROR;
    if(strlen(val.keyword) > 0 && strcmp(keyword,val.keyword)==0)
        return val.tk;

    temp = val.next;
    while(temp!=NULL)
    {
        if(strcmp(keyword,temp->keyword)==0)
            return temp->tk;
        temp= temp->next;
    }

    //NOT FOUND
    return TK_ERROR;
}

//END of Keyword Table Implementation

// ==========================================================================
// ==========================================================================


//Symbol table Implementation(Hash table)

int hash(char* lexeme, hashTable* ht)
{
    int mod = ht->size;
    unsigned h = 0x811c9dc5;
    int len = strlen(lexeme);
    int i = 0;
    //Standard Hash Function
    for ( i = 0; i < len; i++ )
      h = ( (h%mod) ^ lexeme[i] )%mod * 0x01000193;
    return h%mod;
}

hashTable* newHashTable(int size)//Initial size of the table to be 50
{
    hashTable* ht = (hashTable*) malloc(sizeof(hashTable));
    ht->size = size;
    ht->elementCount = 0;
    ht->table = (tableCell*) malloc(sizeof(tableCell)*size);
    int i=0;
    for(i=0;i<size;i++)
    {
        ht->table[i].next = NULL;
        ht->table[i].lexPtr = NULL;
    }
    return ht;
}//end of the function

hashTable* insertInTable(char* lexeme, token tk, hashTable* ht)
{
    if(ht->elementCount+1 > 0.5 * ht->size)
        ht = reSize(ht);
    int hashIndex = hash(lexeme, ht);
    ht->elementCount++;
    if(ht->table[hashIndex].lexPtr == NULL)//nothing in the bucket
    {
        ht->table[hashIndex].t = tk;
        ht->table[hashIndex].lexPtr = lexeme;
        return ht;
    }
    //Collision detected
    if(ht->table[hashIndex].next == NULL)
    {
        tableCell* temp = (tableCell*) malloc(sizeof(tableCell));
        temp->t = tk;
        temp->lexPtr = lexeme;
        temp->next = NULL;
        ht->table[hashIndex].next = temp;
        return ht;
    }
    //Finding the end of the linked list at this bucket
    tableCell* temp = ht->table[hashIndex].next;
    while(temp->next != NULL)
        temp = temp->next;
    temp->next =  (tableCell*) malloc(sizeof(tableCell));
    temp = temp->next;
    temp->lexPtr = lexeme;
    temp->t = tk;
    temp->next = NULL;
    return ht;
}

hashTable* reSize(hashTable* ht)//Doubles the size of the hash table
{
    hashTable* htNew = newHashTable(ht->size * 2);
    int i=0;
    for(i=0;i<ht->size;i++)
    {
        if(ht->table[i].lexPtr == NULL)
            continue;
        insertInTable(ht->table[i].lexPtr, ht->table[i].t, htNew);
        if(ht->table[i].next == NULL)
            continue;
        tableCell* temp = ht->table[i].next;
        tableCell* prev = temp;
        while(temp != NULL)//Traversing the chained linked list
        {
            prev = temp;
            insertInTable(temp->lexPtr, temp->t, htNew);
            temp = temp->next;
            free(prev);
        }
    }
    free(ht->table);
    free(ht);
    return htNew;
}

token lookInHashTable(char* lexeme, hashTable* ht)
{
    int hashIndex = hash(lexeme, ht);
    if(ht->table[hashIndex].lexPtr == NULL)
        return TK_ERROR;
    if(strcmp(ht->table[hashIndex].lexPtr, lexeme) == 0)
        return ht->table[hashIndex].t;
    if(ht->table[hashIndex].next == NULL)//Lexeme not found in the table
        return TK_ERROR;
    tableCell* temp = ht->table[hashIndex].next;
    while(temp != NULL)
    {
        if(strcmp(temp->lexPtr, lexeme) == 0)
            return temp->t;
        temp = temp->next;
    }
    return TK_ERROR;//Nothing matches in the table
}

void printHashTable(hashTable* ht)
{
    int i=0;
    for(i=0;i<ht->size;i++)
    {
        if(ht->table[i].lexPtr == NULL)
        {
            printf("ht[%d] --> NULL\n", i+1);
            continue;
        }
        if(ht->table[i].lexPtr != NULL)
            printf("ht[%d] --> %d, %s", i+1, ht->table[i].t, ht->table[i].lexPtr);
        if(ht->table[i].next == NULL)
        {
            printf("\n");
            continue;
        }
        tableCell* temp = ht->table[i].next;
        while(temp != NULL)
        {
            printf(" ||  %d, %s", temp->t, temp->lexPtr);
            temp = temp->next;
        }
        printf("\n");
    }
}

//END of Symbol Table Implementation

// ==========================================================================
// ==========================================================================

char* temp = NULL;
char* getStream(FILE* fp)
{
    char* buff = NULL;
    char deli;
    buff = (char*) malloc(sizeof(char)*512);
    memset(buff, '\0', sizeof(char)*512);
    int i;
    char* production = NULL;
    production = (char*) malloc(sizeof(char)*512);
    memset(production, '\0', sizeof(char)*512);

    if(temp!=NULL && strlen(temp)!=0)
    {
        strcpy(buff,temp);
        free(temp);
    }
    while(fscanf(fp,"%[^\n]",production)!=EOF   && (strlen(buff) + strlen(production))<=512)
    {
        if(strlen(production)==0)
            strcat(buff,"\n");
        else if((512-strlen(buff))>=strlen(production))//Buffer has adequate space
        {
            strcat(buff,production);
            strcat(buff,"\n");
        }
        memset(production, '\0', sizeof(char)*512);
        fscanf(fp,"%c",&deli);
    }//end of while loop

    if(temp==NULL)
    {
        temp=(char*)malloc(sizeof(char)*512);
        memset(temp, '\0', sizeof(char)*512);
    }
    if((strlen(buff) + strlen(production))>512)
    {
        strcpy(temp,production);
        free(production);
    }
    return buff;
}


// char* getStream(FILE* fp)
// {
//     if(BUFF == NULL)
//     {
//         BUFF = (char*) malloc(sizeof(char)*1024);
//         memset(BUFF, '\0', sizeof(char)*1024);
//     }
//
// int numOfChars;
//     if ((numOfChars = fscanf(fp,"%[^\n]\n",BUFF)) != EOF) {
//         strcat(BUFF, "\n");
//         return BUFF;
//     } else {
//         //EOF
//         memset(BUFF, '\0', sizeof(char) * 1024);
//     }
// }



char* enumToString(token tk)
{
    switch(tk){
        case TK_ERROR:
            return "TK_ERROR";
        case TK_ASSIGNOP:
            return "TK_ASSIGNOP";
        case TK_COMMENT :
            return "TK_COMMENT";
        case TK_FIELDID :
            return "TK_FIELDID";
        case TK_ID :
            return "TK_ID";
        case TK_NUM :
            return "TK_NUM";
        case TK_RNUM :
            return "TK_RNUM";
        case TK_FUNID :
            return "TK_FUNID";
        case TK_RECORDID :
            return "TK_RECORDID";
        case TK_WITH :
            return "TK_WITH";
        case TK_PARAMETERS :
            return "TK_PARAMETERS";
        case TK_END :
            return "TK_END";
        case TK_WHILE :
            return "TK_WHILE";
        case TK_TYPE :
            return "TK_TYPE";
        case TK_MAIN :
            return "TK_MAIN";
        case TK_GLOBAL :
            return "TK_GLOBAL";
        case TK_PARAMETER :
            return "TK_PARAMETER";
        case TK_LIST :
            return "TK_LIST";
        case TK_SQL :
            return "TK_SQL";
        case TK_SQR :
            return "TK_SQR";
        case TK_INPUT :
            return "TK_INPUT";
        case TK_OUTPUT :
            return "TK_OUTPUT";
        case TK_INT :
            return "TK_INT";
        case TK_REAL :
            return "TK_REAL";
        case TK_COMMA :
            return "TK_COMMA";
        case TK_SEM :
            return "TK_SEM";
        case TK_COLON :
            return "TK_COLON";
        case TK_DOT :
            return "TK_DOT";
        case TK_ENDWHILE :
            return "TK_ENDWHILE";
        case TK_OP :
            return "TK_OP";
        case TK_CL :
            return "TK_CL";
        case TK_IF :
            return "TK_IF";
        case TK_THEN :
            return "TK_THEN";
        case TK_ENDIF :
            return "TK_ENDIF";
        case TK_READ :
            return "TK_READ";
        case TK_WRITE :
            return "TK_WRITE";
        case TK_RETURN :
            return "TK_RETURN";
        case TK_PLUS :
            return "TK_PLUS";
        case TK_MINUS :
            return "TK_MINUS";
        case TK_MUL :
            return "TK_MUL";
        case TK_DIV :
            return "TK_DIV";
        case TK_CALL :
            return "TK_CALL";
        case TK_RECORD :
            return "TK_RECORD";
        case TK_ENDRECORD :
            return "TK_ENDRECORD";
        case TK_ELSE :
            return "TK_ELSE";
        case TK_AND :
            return "TK_AND";
        case TK_OR :
            return "TK_OR";
        case TK_NOT :
            return "TK_NOT";
        case TK_LT :
            return "TK_LT";
        case TK_LE :
            return "TK_LE";
        case TK_EQ :
            return "TK_EQ";
        case TK_GT :
            return "TK_GT";
        case TK_GE :
            return "TK_GE";
        case TK_NE :
            return "TK_NE";
        case TK_DOLLAR:
            return "TK_DOLLAR";
        default:
            return "TK_ERROR";
        }
}

// Returns the token
// RETURNS TK_ERROR if Value not Found
token lookup(char* search, int length, keyTable* kt)
{
    token tk = lookKeyword(kt, search);
    return tk;
}

hashTable* insert(char* lexeme, token tk, hashTable* ht)
{
    token temp = lookInHashTable(lexeme, ht);
    if(temp != TK_ERROR)
        return ht;
    return insertInTable(lexeme, tk, ht);
}

//Handle the length of the Identifiers and fields and functions
void getNextToken(tokenInfo *t, keyTable* kt, hashTable* ht, FILE* fp, FILE* fp2)
{
    static int buffPos=0, curLine=1;
    int i;
    int curLexeme=0;// Need to be used while adding character to the buffers
    int state=0;//initial state
    //DFA begin
    while(1)
      {
        if(BUFF[0] == '\0')
        {
            t->tkType = TK_DOLLAR;
            curLine = 1;
            return;
        }
        char ch = BUFF[buffPos++];
        switch(state)
        {
            case 0:
        //t->lexeme='\0'
        for(i=0;i<MAX_LEX_LEN;i++)
            t->lexeme[i]='\0';
                switch (ch)
                {
                    case '<':
                        state = 1;
                        t->lexeme[curLexeme++] =ch;
                        break;
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        t->lexeme[curLexeme++] =ch;
                        state = 8;
                        break;
                    case '%':
                        //Remove the next three lines
                        t->tkType = TK_COMMENT;
                        t->lexeme[0] = '%';
                        t->lexeme[1] = '\0';
                        t->lineNo = curLine;
                        while(ch!='\n')
                            ch = BUFF[buffPos++];
                        buffPos--;
                        state=0;
                        return;
                    case '=':
                        state = 13;
                        t->lexeme[curLexeme++] =ch;
                        break;
                    case '>':
                        state = 15;
                        t->lexeme[curLexeme++] =ch;
                        break;
                    case '!':
                        state = 18;
                        t->lexeme[curLexeme++] =ch;
                        break;
                    case '#':
                        state = 20;
                        t->lexeme[curLexeme++] =ch;
                        break;
                    case '+':
                        t->tkType=TK_PLUS;
                        t->lineNo=curLine;
                        t->lexeme[curLexeme++] =ch;
                        t->lexeme[curLexeme++] ='\0';
                        return;//state 23 handeled
                    case '-':
                        t->tkType=TK_MINUS;
                        t->lineNo=curLine;
                        t->lexeme[curLexeme++] =ch;
                        t->lexeme[curLexeme++] ='\0';
                        return;//state 24
                    case '*':
                        t->tkType=TK_MUL;
                        t->lineNo=curLine;
                        t->lexeme[curLexeme++] =ch;
                        t->lexeme[curLexeme++] ='\0';
                        return;//state 25
                    case '/':
                        t->tkType=TK_DIV;
                        t->lineNo=curLine;
                        t->lexeme[curLexeme++] =ch;
                        t->lexeme[curLexeme++] ='\0';
                        return;//state 26
                    case '(':
                        t->tkType=TK_OP;
                        t->lineNo=curLine;
                        t->lexeme[curLexeme++] =ch;
                        t->lexeme[curLexeme++] ='\0';
                        return;//state 27
                    case ')':
                        t->tkType=TK_CL;
                        t->lineNo=curLine;
                        t->lexeme[curLexeme++] =ch;
                        t->lexeme[curLexeme++] ='\0';
                        return;//state 28
                    case '[':
                        t->tkType=TK_SQL;
                        t->lineNo=curLine;
                        t->lexeme[curLexeme++] =ch;
                        t->lexeme[curLexeme++] ='\0';
                        return;//state 29
                    case ']':
                        t->tkType=TK_SQR;
                        t->lineNo=curLine;
                        t->lexeme[curLexeme++] =ch;
                        t->lexeme[curLexeme++] ='\0';
                        return;//state 30
                    case ',':
                        t->tkType=TK_COMMA;
                        t->lineNo=curLine;
                        t->lexeme[curLexeme++] =ch;
                        t->lexeme[curLexeme++] ='\0';
                        return;//state 31
                    case ';':
                        t->tkType=TK_SEM;
                        t->lineNo=curLine;
                        t->lexeme[curLexeme++] =ch;
                        t->lexeme[curLexeme++] ='\0';
                        return;//state 32
                    case ':':
                        t->tkType=TK_COLON;
                        t->lineNo=curLine;
                        t->lexeme[curLexeme++] =ch;
                        t->lexeme[curLexeme++] ='\0';
                        return;//state 33
                    case '.':
                        t->tkType=TK_DOT;
                        t->lineNo=curLine;
                        t->lexeme[curLexeme++] =ch;
                        t->lexeme[curLexeme++] ='\0';
                        return;//state 34
                    case '~':
                        t->tkType = TK_NOT;
                        t->lineNo=curLine;
                        t->lexeme[curLexeme++] =ch;
                        t->lexeme[curLexeme++] ='\0';
                        return;//state 35 handeled
                    case '@':
                        state = 36;
                        t->lexeme[curLexeme++] =ch;
                        break;
                    case '&':
                        state = 39;
                        t->lexeme[curLexeme++] =ch;
                        break;
                    case '\n':
                        curLine++;
                        state = 0;
                        break;

                    case 'b':
                        state = 50;
                        t->lexeme[curLexeme++]=ch;
                        break;
                    case 'c':
                        state = 50;
                        t->lexeme[curLexeme++]=ch;
                        break;
                    case 'd':
                        state = 50;
                        t->lexeme[curLexeme++]=ch;
                        break;
                    case '_':
                        state = 42;
                        t->lexeme[curLexeme++] =ch;
                        break;
                    case '\0':
                        buffPos = 0;
                        free(BUFF);
                        BUFF = getStream(fp);
                        state = 0;
                        break;
                    default:
                        if(ch>='a' && ch<='z')
                        {
                            state = 54;
                            t->lexeme[curLexeme++] = ch;
                        }
                        else if(ch==' ' || ch=='\n' || ch=='\t' || ch=='\0')
                            state = 0;
                        else
                        {
                            t->tkType = TK_ERROR;
                            t->lineNo = curLine;
                            fprintf(fp2,"Line %-4d: Unknown Symbol %c\n",t->lineNo,ch);
                                return;
                        }
                        break;
                }
                break;
                //End of Case 0 i.e. The State ZERO
            case 1:
                switch(ch)
                {
                    //state 3 handled
                    case '=':
                        t->tkType=TK_LE;
                        t->lexeme[curLexeme++] = ch;
                        t->lexeme[curLexeme++] = '\0';
                        return;
                    case '-':
                        t->lexeme[curLexeme++] = ch;
                        state=4;
                        break;
                    default://state 2 handeled
                        t->tkType=TK_LT;
                        t->lexeme[curLexeme++] = '\0';
                        buffPos--;
                        return;
                }
                break;

            case 4:
                if(ch=='-')
                {
                    state=5;
                    t->lexeme[curLexeme++] = ch;
                }
                else
                    state = 404;
                break;

            case 5:
                if(ch=='-')
                {
                    t->tkType = TK_ASSIGNOP;
                    t->lexeme[curLexeme++] = ch;
                    t->lexeme[curLexeme++] = '\0';
                    t->lineNo = curLine;
                    return;// Case 6
                }
                else
                    state = 404;
                break;

            case 8:
                switch(ch)
                {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        t->lexeme[curLexeme++] = ch;
                        state = 8;//Self Loop
                        break;
                    case '.':
                        state = 10;
                        t->lexeme[curLexeme++] = ch;
                        break;
                    default:
                        t->lexeme[curLexeme] = '\0';
                        t->tkType = TK_NUM;
                        t->lineNo = curLine;
                        buffPos--;// RETRACTION
                        return;//state 9 handeled
                }
                break;

            case 10:
                switch (ch)
                {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        state = 11;
                        t->lexeme[curLexeme++] = ch;
                        break;
                    default:
                        state = 404;
                        break;
                }
                break;

            case 11:
                switch (ch)
                {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        t->tkType = TK_RNUM;
                        t->lexeme[curLexeme++] = ch;
                        t->lexeme[curLexeme] = '\0';
                        t->lineNo = curLine;
                        return;//case 12 handeled
                    default:
                        state = 404;
                        break;
                }
                break;

            case 13:
                if(ch=='=')
                {
                    //State 14 handeled
                    t->tkType=TK_EQ;
                    t->lexeme[curLexeme++] = ch;
                    t->lexeme[curLexeme++] = '\0';
                    t->lineNo = curLine;
                    return;
                }
                else
                    state = 404;
                break;

            case 15:
                if(ch=='=')
                {
                    //State 17 handeled
                    t->tkType=TK_GE;
                    t->lexeme[curLexeme++] = ch;
                    t->lexeme[curLexeme++] = '\0';
                    t->lineNo = curLine;
                    return;
                }
                else
                {
                    // State 16 handeled
                    t->tkType=TK_GT;
                    t->lineNo = curLine;
                    t->lexeme[curLexeme++] = '\0';
                    buffPos--;// RETRACTION
                    return;
                }
                break;

            case 18:
                if(ch=='=')
                {
                    t->tkType=TK_NE;
                    t->lexeme[curLexeme++] = ch;
                    t->lexeme[curLexeme++] = '\0';
                    t->lineNo = curLine;
                    return;
                }
                else
                    state = 404;
                break;

            case 20:
                if( ch>='a' && ch<='z')
                {
                    t->lexeme[curLexeme++] = ch;
                    state = 21;
                }
                else
                    state = 404;// Error Handling
                break;

            case 21:
                if(ch>='a' && ch<='z' )
                {
                    t->lexeme[curLexeme++] = ch;
                    state = 21;
                }
                else
                {
                    //Case 22 handeled
                    t->tkType = TK_RECORDID;
                    t->lineNo = curLine;
                    t->lexeme[curLexeme] = '\0';
                    buffPos--;
                    return;
                }
                break;

            case 36:
                if(ch == '@')
                {
                    state = 37;
                    t->lexeme[curLexeme++] = ch;
                }
                else
                    state = 404;
                break;

            case 37:
                if(ch == '@')
                {
                    t->tkType = TK_OR;
                    t->lexeme[curLexeme++] = ch;
                    t->lexeme[curLexeme++] = '\0';
                    return;//state 38 handeled
                }
                else
                    state = 404;
                break;

            case 39:
                if(ch == '&')
                {
                    state = 40;
                    t->lexeme[curLexeme++] = ch;
                }
                else
                    state = 404;
                break;

            case 40:
                if(ch=='&')
                {
                    //Case 41 handeled
                    t->tkType=TK_AND;
                    t->lineNo = curLine;
                    t->lexeme[curLexeme++] = ch;
                    t->lexeme[curLexeme++] = '\0';
                    return;
                }
                else
                    state = 404;
                break;

            case 42:
                if(ch=='m')
                {
                    state = 46;
                    t->lexeme[curLexeme++] = ch;
                }
                else if(ch>='a' && ch<='z' || ch>='A' && ch<='Z')
                {
                    t->lexeme[curLexeme++] = ch;
                    state=43;
                }
                else
                      state=404;
                break;

            case 43:
                if(ch>='a' && ch<='z' || ch>='A' && ch<='Z')
                {
                    t->lexeme[curLexeme++] = ch;
                    state=43;
                }
                else if(ch>='0' && ch<='9')
                {
                    t->lexeme[curLexeme++] = ch;
                    state =44;
                }
                else
                {
                    //case 45 handled
                    t->tkType = TK_FUNID;
                    t->lineNo = curLine;
                    if(curLexeme-1 > MAX_FUN_LEN)
                        state = 404;
                    else
                    {
                        t->lexeme[curLexeme++] = '\0';
                        ht = insert(t->lexeme, t->tkType, ht);
                        buffPos--;
                        return;
                    }
                }
                break;

            case 44:
                if(ch>='0' && ch<='9')
                {
                    t->lexeme[curLexeme++] = ch;
                    state =44;
                }
                else
                {
                    //case 45 handled
                    t->tkType = TK_FUNID;
                    t->lineNo = curLine;
                    if(curLexeme-1 > MAX_FUN_LEN)
                        state = 404;
                    else
                    {
                        t->lexeme[curLexeme++] = '\0';
                        ht = insert(t->lexeme, t->tkType, ht);
                        buffPos--;
                        return;
                    }
                }
                break;

            case 46:
                if(ch=='a')
                {
                    state = 47;
                    t->lexeme[curLexeme++] = ch;
                }
                else if(ch>'a' && ch<='z' || ch>='A' && ch<='Z' || ch>='0' && ch<='9')
                {
                    t->lexeme[curLexeme++] = ch;
                    state=43;
                }
                else
                {
                    //case 45 handled
                    t->tkType = TK_FUNID;
                    t->lineNo = curLine;
                    if(curLexeme-1 > MAX_FUN_LEN)
                        state = 404;
                    else
                    {
                        t->lexeme[curLexeme++] = '\0';
                        ht = insert(t->lexeme, t->tkType, ht);
                        buffPos--;
                        return;
                    }
                }
                break;

            case 47:
                if(ch=='i')
                {
                    state = 48;
                    t->lexeme[curLexeme++] = ch;
                }
                else if(ch>='a' && ch<='z' || ch>='A' && ch<='Z' || ch>='0' && ch<='9')
                {
                    t->lexeme[curLexeme++] = ch;
                    state=43;
                }
                else
                {
                    t->tkType = TK_FUNID;
                    t->lineNo = curLine;
                    if(curLexeme-1 > MAX_FUN_LEN)
                        state = 404;
                    else
                    {
                        t->lexeme[curLexeme] = '\0';
                        ht = insert(t->lexeme, t->tkType, ht);
                        buffPos--;
                        return;
                    }
                }
                break;

            case 48:
                if(ch=='n')
                {
                    //DFA modified, state 60 added
                    state = 60;
                    t->lexeme[curLexeme++] = ch;

                }
                else if(ch>='a' && ch<='z' || ch>='A' && ch<='Z' || ch>='0' && ch<='9')
                {
                    t->lexeme[curLexeme++] = ch;
                    state=43;
                }
                else
                {
                    //case 45 handled
                    t->tkType = TK_FUNID;
                    t->lineNo = curLine;
                    if(curLexeme-1 > MAX_FUN_LEN)
                        state = 404;
                    else
                    {
                        t->lexeme[curLexeme] = '\0';
                        ht = insert(t->lexeme, t->tkType, ht);
                        buffPos--;
                        return;
                    }
                }
                break;

            case 60:
                if(ch>='a' && ch<='z' || ch>='A' && ch<='Z' || ch>='0' && ch<='9')
                {
                    t->lexeme[curLexeme++] = ch;
                    state=43;
                }
                else
                {
                    //case 49 handled
                    t->tkType = TK_MAIN;
                    t->lineNo = curLine;
                    t->lexeme[curLexeme] = '\0';
                    buffPos--;
                    return;
                }
                break;

            case 50:
                if(ch>='a' && ch<='z')
                {
                    state = 54;
                    t->lexeme[curLexeme++] = ch;
                }
                else if(ch>='2' && ch<='7')
                {
                    state = 51;
                    t->lexeme[curLexeme++] = ch;
                }
                else
                {
                    //State 55 handled
                    t->lineNo= curLine;
                    t->lexeme[curLexeme++] = '\0';
                    buffPos--;//Retraction
                    //Lookup in the keyword Table
                    token temp = lookup(t->lexeme,curLexeme, kt);
                    if(temp!=TK_ERROR)
                    {
                        //This lexeme is a keyword
                        t->tkType = temp;
                        return;
                    }
                    else
                    {
                        t->tkType = TK_FIELDID;
                        //insert needs to be done here
                        ht = insert(t->lexeme,TK_FIELDID, ht);
                        return;
                    }
                }
                break;

            case 51:
                if(ch>='b' && ch<='d')
                {
                    state=51;
                    t->lexeme[curLexeme++]=ch;
                }
                else if(ch>='2' && ch<='7')
                {
                    state=52;
                    t->lexeme[curLexeme++]=ch;
                }
                else//DFA changed, no need for lookup()
                {
                    if(curLexeme-1 > MAX_IDF_LEN)
                    {
                        state = 404;
                        t->tkType = TK_ID;
                        t->lineNo= curLine;
                    }
                    else
                    {
                        t->tkType = TK_ID;
                        t->lineNo= curLine;
                        t->lexeme[curLexeme++] = '\0';
                        buffPos--;//Retraction
                        //insert needs to be done here
                        // 0 Failure 1 Success
                        ht = insert(t->lexeme,TK_ID, ht);
                        return;
                    }
                }
                break;

            case 52:
                if(ch>='2' && ch<='7')
                {
                    state=52;
                    t->lexeme[curLexeme++]=ch;
                }
                else//DFA changed, lookup() removed
                {
                    //state 53
                    if(curLexeme-1 > MAX_IDF_LEN)
                    {
                        state = 404;
                        t->tkType = TK_ID;
                        t->lineNo= curLine;
                    }
                    else
                    {
                        t->tkType = TK_ID;
                        t->lineNo= curLine;
                        t->lexeme[curLexeme++] = '\0';
                        buffPos--;//Retraction
                        //insert needs to be done here
                        // 0 Failure 1 Success
                        ht = insert(t->lexeme,TK_ID, ht);
                        return;
                    }
                }
                break;

            case 54:
                if(ch>='a' && ch<='z' )
                {
                    state = 54;
                    t->lexeme[curLexeme++] = ch;
                }
                else
                {
                    t->lineNo= curLine;
                    t->lexeme[curLexeme++] = '\0';
                    buffPos--;//Retraction
                    token temp = lookup(t->lexeme,curLexeme, kt);
                    if(temp!=TK_ERROR)
                    {
                        //This lexeme is a keyword
                        t->tkType = temp;
                        return;
                    }
                    else
                    {
                        if(curLexeme-1 > MAX_FIELD_LEN)
                        {
                            state = 404;
                            t->tkType = TK_FIELDID;
                            t->lineNo= curLine;
                        }
                        else
                        {
                            t->tkType = TK_FIELDID;
                            //insert needs to be done here
                            ht = insert(t->lexeme,TK_FIELDID, ht);
                            return;
                        }
                    }
                }
                break;

            //Default Error Mechanism
            case 404:

                if(curLexeme > MAX_IDF_LEN && t->tkType == TK_ID)
                {
                    fprintf(fp2,"Line %-4d: Identifier is longer than the prescribed length of 20 characters\n", t->lineNo);
                    t->tkType = TK_ERROR;
                    t->lineNo = curLine;
                    t->lexeme[curLexeme++] = '\0';
                    buffPos-=2;
                    return;
                }
                else if(curLexeme > MAX_FIELD_LEN && t->tkType == TK_FIELDID)
                {
                    fprintf(fp2,"Line %-4d: Record field identifier is longer than the prescribed length of 20 characters\n", t->lineNo);
                    t->tkType = TK_ERROR;
                    t->lineNo = curLine;
                    t->lexeme[curLexeme++] = '\0';
                    buffPos-=2;
                    return;
                }
                else if(curLexeme > MAX_FUN_LEN && t->tkType == TK_FUNID)
                {
                    fprintf(fp2,"Line %-4d: Function is longer than the prescribed length of 30 characters\n", t->lineNo);
                    t->tkType = TK_ERROR;
                    t->lineNo = curLine;
                    t->lexeme[curLexeme++] = '\0';
                    buffPos-=2;
                    return;
                }
                else
                {
                    t->tkType = TK_ERROR;
                    t->lineNo = curLine;
                    t->lexeme[curLexeme++] = '\0';
                    buffPos-=2;
                    fprintf(fp2,"Line %-4d: Unknown pattern %s\n",t->lineNo,t->lexeme);
                    return;
                }
                break;
        }//End of the switch statement

      }// End of While
}// End of Function


void printTokenInfile(FILE* fp, keyTable* kt, hashTable* ht)
{
    FILE* fpout = fopen("outputLex.txt", "w");
    BUFF = getStream(fp);
    FILE* fp2 = fopen("lexicalError.txt","w");
    while(1)
    {
        tokenInfo *t = (tokenInfo*) malloc(sizeof(tokenInfo));
        t->lexeme[0] = '\0';//initialisation
        getNextToken(t, kt, ht, fp,fp2);
        // fprintf(fp, "<%s>", enumToString(t->tkType));

        if(BUFF[0] == '\0' || t->tkType == TK_DOLLAR)
        {
           // printf("<%s>\n", "TK_DOLLAR");//Integrate this in fprintf
            break;
        }
        printf("<%-20s> Line : %d\n", enumToString(t->tkType), t->lineNo);
        fprintf(fpout,"<%-20s> Line : %d\n", enumToString(t->tkType), t->lineNo);
        free(t);
    }
    fclose(fpout);
    fclose(fp2);
}
int stringToInt(tokenInfo *tk)
{
    int val =0;
    if(tk->tkType==TK_NUM)
    {
        int n = strlen(tk->lexeme);
        int i=0;
        for(i=0;i<n;i++)
        {
            val = val*10 + (((tk->lexeme)[i])-'0');
        }
    }
    return val;
}//End of the function


void printNum(tokenInfo* tk)
{
    if(tk->tkType==TK_RNUM || tk->tkType==TK_RNUM)
    {
        printf("%s",tk->lexeme);
    }
}//end of the function
//
// int main()
// {
//     keyTable* kt = newKeywordTable();
//     hashTable* ht = newHashTable(INIT_SYM_TAB_LEN);
//     populateKeyword(kt);
//     FILE* fp = fopen("./testcases/testcase4.txt", "r");
//     if(fp == NULL)
//     {
//         printf("Input file not found!!\n");
//         return -1;
//     }
//     // int inputSize = strlen(BUFF);
//     printTokenInfile(fp, kt, ht);
//     fclose(fp);
//     return 0;
// }
