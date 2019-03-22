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

#define TOKEN_ARR_LEN 100
int thingsPushed;
int containsEpsilon(ft *alpha)
{
    while(alpha!=NULL)
    {
        if(isEpsilon(alpha->terminal))
            return 1;
        alpha=alpha->next;
    }
    return 0;
}
void handleLexError(tokenInfo* tk)
{
    printf("Line %d: ",tk->lineNo);
}
void handleSyntaxErrorWithSyn(tokenInfo* tk,stackParse *stk)
{
  if(isTerminal(peek(stk)))
  {
      if(strcmp(peek(stk),"TK_SEM")==0)
      {
          printf("\nLine %d: Semicolon is missing",tk->lineNo);
      }
      else
      {
      printf("\nLine %d: The token %s for lexeme %s does not match with the expected token %s",tk->lineNo,enumToString(tk->tkType),tk->lexeme,peek(stk));
      }
  }
  else
    printf("\nLine %d: Unexpected token %s for lexeme %s",tk->lineNo,enumToString(tk->tkType),tk->lexeme);
}
int belongsInFollow(grammar *gmr, char *terminal, char *non_terminal)
{
  fl *cur=getFollow(gmr,non_terminal);
  while(cur!=NULL)
  {
    if(strcmp(cur->terminal,terminal)==0)
      return 1;
    cur=cur->next;
  }
  return 0;
}
void genParseTable(grammar *gmr,dict *dt)
{
  //initialising all elements to -1 (error)
  int k=0;
  int j=0;
  parseTable = (int**) malloc(sizeof(int*)*NO_OF_NONTERMINALS);
  for(j=0;j<NO_OF_NONTERMINALS;j++)
  {
    parseTable[j] = (int*) malloc(sizeof(int)*NO_OF_TERMINALS);
    for(k=0;k<NO_OF_TERMINALS;k++)
      parseTable[j][k] =-1;
  }

  int prodCount=gmr->prodCount;
  int i=0;
  char *leftside=(char *)malloc(MAX_NON_TERMINAL_LEN);
  char *rightside=(char *)malloc(TERMINAL_LEN);

  for(i=0;i<prodCount;i++) //for every A->alpha
  {
    strcpy(leftside,gmr->nonTerm[i].nonTermName);
    strcpy(rightside,gmr->nonTerm[i].next->nonTermName);
    ft *alpha=getFirst(gmr,rightside);
    int flagEpsilon=0;//Epsilon in the first or not
    int flagDollar=0;//Dollar in the follow or not
    int row=getSetIndex(dt,leftside);
    ruleHead *rh=gmr->nonTerm[i].next->next;


    while(alpha!=NULL)//Iterating in first here
    {
      if(isTerminal(alpha->terminal))
      {//if first is not eps then simply fill in the table
      int col=getSetIndex(dt,alpha->terminal);
      parseTable[row][col]=i;
      }
      else if(isEpsilon(alpha->terminal))
      {//Check for epsilon
        //flagEpsilon=1;

        while( rh!=NULL && containsEpsilon(getFirst(gmr, rh->nonTermName)))
        {
            ft *alpha1=getFirst(gmr, rh->nonTermName);
            while(alpha1!=NULL){
            if(isTerminal(alpha1->terminal))
            {//if first is not eps then simply fill in the table
            int col1=getSetIndex(dt,alpha1->terminal);
            parseTable[row][col1]=i;
            }
            alpha1=alpha1->next;
            }
            rh=rh->next;
        }

        if(rh==NULL){
            flagEpsilon=1;
        }
        else
        {

            ft *alpha2=getFirst(gmr, rh->nonTermName);
            while(alpha2!=NULL){
            if(isTerminal(alpha2->terminal))
            {//if first is not eps then simply fill in the table
            int col2=getSetIndex(dt,alpha2->terminal);
            parseTable[row][col2]=i;
            }
            alpha2=alpha2->next;
            }
            rh=rh->next;

      }
    }
      alpha=alpha->next;
    }

    //handling epsilon case :
    // left side ka follow nikalkar proceed
    if(flagEpsilon)
    {
      fl *beta=getFollow(gmr,leftside);
      while(beta!=NULL)
      {
        if(isTerminal(beta->terminal))
        {
          int col3=getSetIndex(dt,beta->terminal);
          parseTable[row][col3]=i;
        }
        else
        {
          flagDollar=1;
        }
        beta=beta->next;
      }
    }

    if(flagEpsilon && flagDollar)
    {
      int col4=getSetIndex(dt,"TK_DOLLAR");
      parseTable[row][col4]=i;
    }

  }
}

void printParseTable(dict *dt)
{
  int i=0,j=0;
  for(;i<dt->ntCount;i++)
  {
    for(j=0;j<dt->tCount;j++)
    {
      printf("%d ",parseTable[i][j]);
    }
    printf("\n");
  }
}

//Returns the AST
treeNode* parse(FILE *fp, grammar *gmr, FILE *fp2)
{

  int tokenCount=0;
  int pos = 0;//Buffer Position for Tokens
  int arraySize = TOKEN_ARR_LEN;
  //All tokens are stored in this array
  tokenInfo* tkData = (tokenInfo*) malloc(sizeof(tokenInfo)*arraySize);
  stackParse *stk = createStack();
  push(stk,"TK_DOLLAR");
  push(stk,gmr->nonTerm[0].nonTermName);

  //Initialising parse table ---> 0
  // ph=(parseHash *)malloc(sizeof(parseHash));
  // ph->size=0;
  // ph->count=0;
  // ph->tCount=0;
  // ph->ntCount=0;
  dict *dt=(dict *)malloc(sizeof(dict));
  dt->tm=(term *)malloc(sizeof(term));
  dt->tm->index=0;
  dt->nm=(nonTerm *)malloc(sizeof(nonTerm));
  dt->nm->index=0;
  dt->ntCount=0;
  dt->tCount=0;
  genParseTable(gmr,dt);
  //parseHashPrint(ph);
  treeNode* ast = createTreeNode(gmr->nonTerm[0].nonTermName,NULL);//Abstract Syntax Tree
  ast->nextSibling = NULL;
  treeNode* child,sibling,temp1;
  treeNode *parent=ast->parent;
  treeNode* curNode = ast;//Current Pointer for Traversing

  keyTable* kt = newKeywordTable();
  hashTable* ht = newHashTable(INIT_SYM_TAB_LEN);
  populateKeyword(kt);
  int i=0;
  if(BUFF != NULL)
    free(BUFF);
  BUFF = getStream(fp);
  getNextToken(&tkData[0],kt,ht,fp,fp2);
  //Getting all the tokens from the input file at once
  while(tkData[i].tkType!=TK_DOLLAR)
  {
    if(i==arraySize-1)
    {
      arraySize*=2;
      tkData = realloc(tkData,sizeof(tokenInfo)*arraySize);
    }
    getNextToken(&tkData[++i],kt,ht,fp,fp2);
  }
  tokenCount=i;

  while(1)
  {
    //getNextToken(fp,&tkData[tokenCount++]);//VOID return type
    if(tkData[pos].tkType==TK_ERROR)
    {
        while(strcmp(peek(stk),"TK_SEM")!=0)
        {
            pop(stk);
        }

        pop(stk);
        while(tkData[pos].tkType!=TK_SEM)
            pos++;
        pos++;
    }
    // checking the parse table
    if(tkData[pos].tkType==TK_COMMENT){
        pos++;
        continue;
    }

    int row=getSetIndex(dt,peek(stk));
    int col = getSetIndex(dt,enumToString(tkData[pos].tkType));
    int index=parseTable[row][col];
    if(strcmp(peek(stk),enumToString(tkData[pos].tkType))!=0 &&index==-1)
    {
      // checking if it's syn condition
      char *terminal=enumToString(tkData[pos].tkType);
      char *non_terminal=peek(stk);
      fl *cur1=getFollow(gmr,non_terminal);
      int synFlag=0;
      while(cur1!=NULL)
      {
        if(strcmp(cur1->terminal,terminal)==0)
        {
          // syn condition
          synFlag=1;
        }
        cur1=cur1->next;
      }
      if(synFlag==1)
      {

        // read parse table again

        pop(stk);
        continue;

      }
      else
      {
          handleSyntaxErrorWithSyn(&tkData[pos],stk);
          while(!belongsInFollow(gmr,enumToString(tkData[pos].tkType),peek(stk)) && tkData[pos].tkType!=TK_SEM)
          {
              pos++;
          }
          if(tkData[pos].tkType==TK_SEM)
          {
            pos++;
            while(strcmp(peek(stk),"TK_SEM")!=0)
            {
                pop(stk);
            }
            pop(stk);
          }
          //getNextToken(fp,&tkData[tokenCount++]);
      }
        continue;
     }
    // pushing the right side of the rule on the stack in the opposite direction


    //Parsing here

    //Case: I --> End of string $ reached
    if(strcmp(enumToString(tkData[pos].tkType),"TK_DOLLAR")==0 && strcmp(peek(stk),"TK_DOLLAR")==0)
    {
      return ast;//AST node returned
    }
    else if(strcmp(enumToString(tkData[pos].tkType),peek(stk))==0)
    {// terminal- terminal matching here
      curNode->leaf=1;//Terminal Marking
      // strcpy(curNode->tk.lexeme,"123.45");
      strcpy(curNode->tk.lexeme,tkData[pos].lexeme);
      // curNode->tk.tkType = TK_RNUM;
      curNode->tk.tkType = tkData[pos].tkType;
      curNode->tk.lineNo=tkData[pos].lineNo;
      treeNode* aloc = locateNextNonTerminal(curNode);
      if(aloc!=NULL)
      {
          curNode = aloc;
          parent = curNode->parent;
      }
      //curNode points to a leftmost Non-Terminal now
      char* key = pop(stk);
      thingsPushed--;
      pos++;
      //getNextToken(fp,&tkData[tokenCount++]);
      continue;
    }
    else
    {//Rule Pusherd in ast
      //E-> A+T
      ruleHead *cur=gmr->nonTerm[index].next;// then A
      parent = curNode;
      curNode->firstChild = createTreeNode(cur->nonTermName,parent);
      curNode = curNode->firstChild;
      treeNode* temp = curNode;//To advance in siblings

      stackParse* rev = createStack();
      push(rev,cur->nonTermName);
      cur=cur->next;
      //Epsilon should not be there on the reverse or the main stack
      if(strcmp(peek(rev),"eps")==0)
        {
            pop(rev);
            curNode->leaf=1;
            pop(stk);
            treeNode* aloc = locateNextNonTerminal(curNode);
            if(aloc!=NULL)
            {
                curNode = aloc;
                parent = curNode->parent;
            }
            continue;
        }

      //Filling Siblings now
      while(cur!=NULL)
      {
        temp->nextSibling = createTreeNode(cur->nonTermName,parent);
        push(rev,cur->nonTermName);
        temp = temp->nextSibling;
        cur=cur->next;
      }
      //Storing the rules in the reverse manner
      pop(stk);
      thingsPushed=0;
      while(!isEmpty(rev))
      {
          push(stk,pop(rev));
          thingsPushed++;
      }
      deleteStack(rev);
    }

  }//end of while
}

// int main()
// {
//     grammar *gmr=(grammar *)malloc(sizeof(grammar));
//     gmr->ftsa = NULL;
//     gmr->flsa = NULL;
//     gmr->nonTerm=(ruleHead *)malloc(200*sizeof(ruleHead));
//     gmr->prodCount=8;
//     insert1(&gmr->nonTerm[0],"E");
//     insert1(&gmr->nonTerm[0],"T");
//     insert1(&gmr->nonTerm[0],"Eprime");
//
//     insert1(&gmr->nonTerm[1],"Eprime");
//     insert1(&gmr->nonTerm[1],"TK_PLUS");
//     insert1(&gmr->nonTerm[1],"T");
//     insert1(&gmr->nonTerm[1],"Eprime");
//
//     insert1(&gmr->nonTerm[2],"Eprime");
//     insert1(&gmr->nonTerm[2],"eps");
//
//     insert1(&gmr->nonTerm[3],"T");
//     insert1(&gmr->nonTerm[3],"F");
//     insert1(&gmr->nonTerm[3],"Tprime");
//
//     insert1(&gmr->nonTerm[4],"Tprime");
//     insert1(&gmr->nonTerm[4],"TK_MUL");
//     insert1(&gmr->nonTerm[4],"F");
//     insert1(&gmr->nonTerm[4],"Tprime");
//
//     insert1(&gmr->nonTerm[5],"Tprime");
//     insert1(&gmr->nonTerm[5],"eps");
//
//     insert1(&gmr->nonTerm[6],"F");
//     insert1(&gmr->nonTerm[6],"TK_ID");
//
//     insert1(&gmr->nonTerm[7],"F");
//     insert1(&gmr->nonTerm[7],"TK_OP");
//     insert1(&gmr->nonTerm[7],"E");
//     insert1(&gmr->nonTerm[7],"TK_CL");
//     insertFirst(gmr,"F");
//     insertFirst(gmr,"Tprime");
//     insertFirst(gmr,"T");
//     insertFirst(gmr,"Eprime");
//     insertFirst(gmr,"E");
//     // printFirst(gmr,"E");
//     // printFirst(gmr,"Eprime");
//     // printFirst(gmr,"T");
//     // printFirst(gmr,"Tprime");
//     // printFirst(gmr,"F");
//     //printf("First set have been succesfully created!!\n");
//     formNewList();
//     insertFollow(gmr, "E");
//     insertFollow(gmr, "Eprime");
//     insertFollow(gmr, "T");
//     insertFollow(gmr, "Tprime");
//     insertFollow(gmr, "F");
//     // printfollow(gmr, "E");
//     // printfollow(gmr, "Eprime");
//     // printfollow(gmr, "T");
//     // printfollow(gmr, "Tprime");
//     // printfollow(gmr, "F");
//     // parseHash *ph=(parseHash *)malloc(sizeof(parseHash));
//     // ph->size=0;
//     // ph->count=0;
//     // ph->tCount=0;
//     // ph->ntCount=0;
//     // dict *dt=(dict *)malloc(sizeof(dict));
//     // dt->tm=(term *)malloc(sizeof(term));
//     // dt->tm->index=0;
//     // dt->nm=(nonTerm *)malloc(sizeof(nonTerm));
//     // dt->nm->index=0;
//     // dt->tCount=0;
//     // dt->ntCount=0;
//     // genParseTable(gmr,dt);
//     //printParseTable(dt);
//     // int i=0;
//     // int j=0;//
//     // printParseTable(ph);
//     //printf("%d",parseTable[4][4]);
//     FILE *fp;
//     treeNode* tree = parse(fp,gmr);
//     printTree(tree);
//     printf("\n");
// }
// parses 1 token at a time and then adds it to the array

// void getToken(tokenInfo *tk)//Mocks the //getNextToken()
// {
//   static int send = 0;
//   token* data = (token*) malloc(sizeof(token)*5);
//   data[0]=TK_ID;
//   data[1]=TK_PLUS;
//   data[3]=TK_ID;
//   data[2]=TK_MUL;
//   data[4]=TK_DOLLAR;
//
//   if(send<6)
//   tk->tkType = data[send++];
//   else
//   tk->tkType = TK_ERROR;
// }
