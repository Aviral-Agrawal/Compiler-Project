#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__
#include "symbolTable.h"
#endif
// /*
// #include <stdio.h>
//
// #define ANSI_COLOR_RED     "\x1b[31m"
// #define ANSI_COLOR_GREEN   "\x1b[32m"
// #define ANSI_COLOR_YELLOW  "\x1b[33m"
// #define ANSI_COLOR_BLUE    "\x1b[34m"
// #define ANSI_COLOR_MAGENTA "\x1b[35m"
// #define ANSI_COLOR_CYAN    "\x1b[36m"
// #define ANSI_COLOR_RESET   "\x1b[0m"
//
// int main () {
//
//   printf(ANSI_COLOR_RED     "This text is RED!"     ANSI_COLOR_RESET "\n");
//   printf(ANSI_COLOR_GREEN   "This text is GREEN!"   ANSI_COLOR_RESET "\n");
//   printf(ANSI_COLOR_YELLOW  "This text is YELLOW!"  ANSI_COLOR_RESET "\n");
//   printf(ANSI_COLOR_BLUE    "This text is BLUE!"    ANSI_COLOR_RESET "\n");
//   printf(ANSI_COLOR_MAGENTA "This text is MAGENTA!" ANSI_COLOR_RESET "\n");
//   printf(ANSI_COLOR_CYAN    "This text is CYAN!"    ANSI_COLOR_RESET "\n");
//
//   return 0;
// }
// */
symbolTable* initSymbolTable()
{
  symbolTable *st=malloc(sizeof(symbolTable));
  st->fHead=NULL;
  st->rHead=NULL;
  st->gHead=NULL;
  return st;
}

funTable* initFunTable()
{
  funTable* ft = (funTable*)malloc(sizeof(funTable));
  memset(ft->keyword,'\0',sizeof(ft->keyword));
  ft->inputParams= NULL;
  ft->outputParams= NULL;
  ft->localVariable= NULL;
  ft->next= NULL;
  return ft;
}

recTable* initRecTable( )
{
  recTable* rt = (recTable*) malloc(sizeof(recTable));
  memset(rt->keyword,'\0',sizeof(rt->keyword));
  rt->fields=NULL;
  rt->next=NULL;
  return rt;
}

idfTable* initIdfTable()
{
  idfTable* idf = (idfTable*) malloc(sizeof(idfTable));
  memset(idf->keyword,'\0',sizeof(idf->keyword));
  idf->width=0;
  idf->offset=0;
  idf->nType=-1;
  idf->extraData=NULL;
  idf->next=NULL;
  return idf;
}

globTable* initGlobalTable()
{
  globTable* glb = (globTable*) malloc(sizeof(globTable));
  memset(glb->keyword,'\0',sizeof(glb->keyword));
  glb->width=0;
  glb->offset=0;
  glb->nType=-1;
  glb->next=NULL;
  return glb;
}

void printGlobalTable(symbolTable* st)
{

  if(!st || !st->gHead)
  return;
  // printf("%c[4mHello world\n%c[0m",27,27);
  globTable* glb = st->gHead;
  printf("WELCOME TO GLOBAL TABLE PRINTER!!\n");
  printf("%c[35m%-10s%c[0m %c[35m%-10s%c[0m %c[35m%-10s%c[0m %c[35m%-10s%c[0m\n",27,"KEYWORD",27,27,"WIDTH",27,27,"OFFSET",27,27,"NUM_TYPE",27);
  while(glb!=NULL)
  {
    printf("%-10s %-10d %-10d ",glb->keyword,glb->width,glb->offset);
    if(glb->nType==0)
    printf("%-10s\n","INT");
    else if(glb->nType==1)
    printf("%-10s\n","REAL");
    else if(glb->nType==2)
    printf("%-10s\n","RECORD");
    glb=glb->next;
  }

}

void printRecordTable(symbolTable* st)
{
  if(!st || !st->rHead)
  return;
  recTable* rec = st->rHead;
  printf("WELCOME TO RECORD TABLE PRINTER!!\n");
  printf("%c[35m%-10s%c[0m %c[35m%-10s%c[0m %c[35m%-10s%c[0m %c[35m%-10s%c[0m\n",27,"KEYWORD",27,27,"WIDTH",27,27,"OFFSET",27,27,"NUM_TYPE",27);
  while(rec!=NULL)
  {
    printf("%-10s\n",rec->keyword);
    idfTable *temp=rec->fields;
    while(temp!=NULL)
    {
      printf("%-10s  %-10d  %-10d %-10d\n" , temp->keyword,temp->width,temp->offset,temp->nType);
      temp=temp->next;
    }
    printf("\n");
    rec=rec->next;
  }

}

void printFunctionTable(symbolTable* st)
{
  if(!st || !st->fHead)
  return;
  // printf("%c[4mHello world\n%c[0m",27,27);
  funTable* ft = st->fHead;
  printf("WELCOME TO FUNCTION TABLE PRINTER!!\n");
  // printf("%c[35m%-10s%c[0m %c[35m%-10s%c[0m %c[35m%-10s%c[0m %c[35m%-10s%c[0m\n",27,"KEYWORD",27,27,"WIDTH",27,27,"OFFSET",27,27,"NUM_TYPE",27);
  printf("Function Name : ");
  while(ft!=NULL)
  {
    printf("Function Name : %10s\n",ft->keyword);
    //Input Parameters
    printf("INPUT PARAMETERS:\n");
    idfTable* idfTemp = ft->inputParams;
    while(idfTemp!=NULL)
    {
      printf("%-10s %-10d %-10d ",idfTemp->keyword,idfTemp->width,idfTemp->offset);
      if(idfTemp->nType==0)
      printf("%-10s\n","INT");
      else if(idfTemp->nType==1)
      printf("%-10s\n","REAL");
      else if(idfTemp->nType==2)
      printf("%-10s\n","RECORD");
      idfTemp=idfTemp->next;
    }

    // //Output Parameters
    // printf("OUTPUT PARAMETERS:\n");
    // idfTemp = ft->outputParams;
    // while(idfTemp!=NULL)
    // {
    //   printf("%-10s %-10d %-10d ",idfTemp->keyword,idfTemp->width,idfTemp->offset);
    //   if(idfTemp->nType==0)
    //   printf("%-10s\n","INT");
    //   else if(idfTemp->nType==1)
    //   printf("%-10s\n","REAL");
    //   else if(idfTemp->nType==2)
    //   printf("%-10s\n","RECORD");
    //   idfTemp=idfTemp->next;
    // }
    //
    // //Local Variables
    // printf("LOCAL VARIABLES:\n");
    // idfTemp = ft->localVariable;
    // while(idfTemp!=NULL)
    // {
    //   printf("%-10s %-10d %-10d ",idfTemp->keyword,idfTemp->width,idfTemp->offset);
    //   if(idfTemp->nType==0)
    //   printf("%-10s\n","INT");
    //   else if(idfTemp->nType==1)
    //   printf("%-10s\n","REAL");
    //   else if(idfTemp->nType==2)
    //   printf("%-10s\n","RECORD");
    //   idfTemp=idfTemp->next;
    // }

    // Forwarding to next Function Call
    ft=ft->next;
  }

}

void populateSymbolTable(astNode *root,symbolTable* st)
{
  populateRecordTable(root,st);
  populateGlobalTable(root,st);
  populateFunctionTable(root,st);
}

void populateFunctionTable(astNode *root,symbolTable* st)
{
  if(root==NULL)
  return;
  astNode *temp=root;
  if(strcmp(temp->keyword,"<function>")==0 )
  {

    funTable* iter = st->fHead;
    //recTable* iter = st->rHead;
    // int tempWidth = 0;
    int firstTime=0;
    if(iter==NULL)
    {
      st->fHead=initFunTable();
      iter=st->fHead;
      firstTime=1;
    }
    else
    {
      while(iter->next!=NULL)
      {
        // tempWidth +=iter->inputParams->width;
        iter=iter->next;
      }
      iter->next=initFunTable();//Giving a new Identifier Table for the information
      // tempWidth+=iter->inputParams->width;
      iter=iter->next;
      firstTime=0;
    }

//==============================INPUT PARAMETERS===================================================

    strcpy(iter->keyword,temp->firstChild->tk.lexeme);
    astNode *ipar=temp->firstChild->nextSibling;
    //Logic for going inside now
    astNode *paraList= ipar->firstChild;
    while(strcmp(paraList->keyword,"<parameter_list>")!=0)
    paraList=paraList->nextSibling;

    // Summation for inputParameters width to get offset

    int tempWidthInput = 0;
    iter->inputParams=initIdfTable();
    if(strcmp(paraList->firstChild->firstChild->keyword,"<primitiveDatatype>")==0)
    {
      astNode* temp1 = paraList->firstChild->firstChild->firstChild;

      if(temp1->tk.tkType==TK_INT)
      iter->inputParams->nType=0;
      else if(temp1->tk.tkType==TK_REAL)
      iter->inputParams->nType=1;

      strcpy(iter->inputParams->keyword,paraList->firstChild->nextSibling->tk.lexeme);

      if(iter->inputParams->nType==0)
      {
        iter->inputParams->width = NUM_WIDTH;
        // if(firstTime==1)//Logic just to make the first entry zero
          iter->inputParams->offset=0;
        // else
          // iter->inputParams->offset=tempWidthInput;
      }
      else if(iter->inputParams->nType==1)
      {
        iter->inputParams->width = RNUM_WIDTH;
        // if(firstTime==1)//Logic just to make the first entry zero
          iter->inputParams->offset=0;
        // else
          // iter->inputParams->offset=tempWidthInput;
      }
      tempWidthInput+=iter->inputParams->width ;
    }

    else   if(strcmp(paraList->firstChild->firstChild->keyword,"<constructedDatatype>")==0)
    {
      astNode* temp1 = paraList->firstChild->firstChild->firstChild->nextSibling;
      iter->inputParams->nType=2;
      strcpy(iter->inputParams->keyword,temp1->keyword);
      iter->inputParams->extraData=initIdfTable();
      //traversing the record table to find the size of RECORD
      recTable *tmp=st->rHead;
      int tempWidthInputRecord = 0;
      while(tmp!=NULL)
      {
        if(strcmp(tmp->keyword,temp1->keyword)==0)//matched the record in input list with that in record list in symbol table
        {
          idfTable *temporary;
          temporary=tmp->fields;
          while(temporary->next!=NULL)
          {
            tempWidthInputRecord+=temporary->width;
            temporary=temporary->next;
          }
          tempWidthInputRecord+=temporary->width;
          iter->inputParams->width =tempWidthInputRecord;

        }
        tmp=tmp->next;
      }

      iter->inputParams->offset=0;
      // update this
      tempWidthInput+=iter->inputParams->width;

    }

    //Here "tempWidthInputRecord" is the current summation of the offset

    //Now Recusrsing in the <remaining_list>
    astNode* remListCheck = paraList->firstChild->nextSibling->nextSibling;
    if(strcmp(remListCheck->firstChild->tk.lexeme,"eps")!=0)
    {
      iter->inputParams->next = initIdfTable();
    }
    idfTable* curPosInParamTable = iter->inputParams->next;

    while(strcmp(remListCheck->firstChild->tk.lexeme,"eps")!=0)
    {
      paraList=remListCheck->firstChild;
      if(strcmp(paraList->firstChild->firstChild->keyword,"<primitiveDatatype>")==0)
      {
        astNode* temp2 = paraList->firstChild->firstChild->firstChild;
        astNode* temp1 = paraList->firstChild->nextSibling;

        if(temp2->tk.tkType==TK_INT)
          curPosInParamTable->nType=0;
        else if(temp2->tk.tkType==TK_REAL)
          curPosInParamTable->nType=1;

        strcpy(curPosInParamTable->keyword,temp1->tk.lexeme);

        if(curPosInParamTable->nType==0)
        {
          curPosInParamTable->width = NUM_WIDTH;
          curPosInParamTable->offset=tempWidthInput ;
        }
        else if(curPosInParamTable->nType==1)
        {
          curPosInParamTable->width = RNUM_WIDTH;
          curPosInParamTable->offset= tempWidthInput ;
        }
        //Offset should be always updated to tell the current position
        tempWidthInput+=  curPosInParamTable->width;
      }
      else   if(strcmp(paraList->firstChild->firstChild->keyword,"<constructedDatatype>")==0)
      {
        astNode* temp1 = paraList->firstChild->firstChild->firstChild->nextSibling;
        curPosInParamTable->nType=2;
        strcpy(curPosInParamTable->keyword,temp1->keyword);
        curPosInParamTable->extraData=initIdfTable();
        //traversing the record table to find the size of RECORD
        recTable *tmp=st->rHead;
        int tempWidthInputRecord = 0;
        while(tmp!=NULL)
        {
          if(strcmp(tmp->keyword,temp1->keyword)==0)//matched the record in input list with that in record list in symbol table
          {
            idfTable *temporary;
            temporary=tmp->fields;
            while(temporary->next!=NULL)
            {
              tempWidthInputRecord+=temporary->width;
              temporary=temporary->next;
            }
            tempWidthInputRecord+=temporary->width;
            curPosInParamTable->width =temporary->offset + temporary->width ;

          }
          tmp=tmp->next;
        }

        curPosInParamTable->offset=tempWidthInput;
        tempWidthInput=tempWidthInputRecord;
      }

      //Forwarding Logic
      remListCheck=remListCheck->firstChild->firstChild->nextSibling->nextSibling;
      if(strcmp(remListCheck->firstChild->tk.lexeme,"eps")!=0)
      {
        curPosInParamTable->next = initIdfTable();
        curPosInParamTable= curPosInParamTable->next;
      }
      // if(remListCheck)
    }// end of iterative while



//==============================OUTPUT PARAMETERS===================================================
  //
  //   astNode *opar=temp->firstChild->nextSibling->nextSibling;
  //
  //   if(strcmp(opar->firstChild->tk.lexeme,"eps")!=0)
  //   {
  //     //printf("%s",opar->firstChild->tk.lexeme);
  //     //extract parameter list for output parameters
  //     astNode *paraList= opar->firstChild;
  //     while(strcmp(paraList->keyword,"<parameter_list>")!=0)
  //       paraList=paraList->nextSibling;
  //
  //     iter->outputParams=initIdfTable();
  //     int tempOffset = 0;
  //     if(strcmp(paraList->firstChild->firstChild->keyword,"<primitiveDatatype>")==0)
  //     {
  //       astNode* temp1 = paraList->firstChild->firstChild->firstChild;
  //
  //       if(temp1->tk.tkType==TK_INT)
  //       {
  //         iter->outputParams->nType=0;
  //         iter->outputParams->width = NUM_WIDTH;
  //         iter->outputParams->offset=/*Prev_offset(0 in init) + */0;
  //       }
  //       else if(temp1->tk.tkType==TK_REAL)
  //       {
  //         iter->outputParams->nType=1;
  //         iter->outputParams->width = RNUM_WIDTH;
  //         iter->outputParams->offset=/*Prev_offset(0 in init) + */0;
  //       }
  //
  //       strcpy(iter->outputParams->keyword,paraList->firstChild->nextSibling->tk.lexeme);
  //
  //
  //       //Offset should be always updated to tell the current position
  //       tempOffset=tempOffset + iter->outputParams->offset;
  //     }
  //     else   if(strcmp(paraList->firstChild->firstChild->keyword,"<constructedDatatype>")==0)
  //     {
  //       astNode* temp1 = paraList->firstChild->firstChild->firstChild->nextSibling;
  //       iter->outputParams->nType=2;
  //       strcpy(iter->outputParams->keyword,paraList->firstChild->nextSibling->tk.lexeme);
  //       iter->outputParams->extraData=initIdfTable();
  //       //traversing the record table to find the size of RECORD
  //       recTable *tmp=st->rHead;
  //       while(tmp!=NULL)
  //       {
  //         if(strcmp(tmp->keyword,temp1->tk.lexeme)==0)//matched the record in input list with that in record list in symbol table
  //         {
  //           idfTable *temporary;
  //           temporary=tmp->fields;
  //           while(temporary->next!=NULL)
  //           temporary=temporary->next;
  //           iter->outputParams->width =temporary->offset + temporary->width ;
  //
  //         }
  //         tmp=tmp->next;
  //       }
  //       iter->outputParams->offset=iter->outputParams->width ;
  //       // update this
  //       tempOffset=iter->outputParams->offset;
  //
  //     }
  //
  //     //Now Recusrsing in the <remaining_list>
  //     astNode* remListCheck = paraList->firstChild->nextSibling->nextSibling;
  //     idfTable* curPosInParamTable=iter->outputParams;
  //
  //
  //     while(strcmp(remListCheck->firstChild->tk.lexeme,"eps")!=0)
  //     {
  //       curPosInParamTable->next = initIdfTable();
  //       curPosInParamTable=curPosInParamTable->next;
  //       paraList=remListCheck->firstChild;
  //       if(strcmp(paraList->firstChild->firstChild->keyword,"<primitiveDatatype>")==0)
  //       {
  //         astNode* temp1 = paraList->firstChild->firstChild->firstChild;
  //
  //         if(temp1->tk.tkType==TK_INT)
  //         {
  //           iter->outputParams->nType=0;
  //           iter->outputParams->width = NUM_WIDTH;
  //           iter->outputParams->offset=/*Prev_offset(0 in init) + */0;
  //         }
  //         else if(temp1->tk.tkType==TK_REAL)
  //         {
  //           iter->outputParams->nType=1;
  //           iter->outputParams->width = RNUM_WIDTH;
  //           iter->outputParams->offset=/*Prev_offset(0 in init) + */0;
  //         }
  //
  //         strcpy(iter->outputParams->keyword,paraList->firstChild->nextSibling->tk.lexeme);
  //
  //
  //         //Offset should be always updated to tell the current position
  //         tempOffset=tempOffset + iter->outputParams->offset;
  //       }
  //       else   if(strcmp(paraList->firstChild->firstChild->keyword,"<constructedDatatype>")==0)
  //       {
  //         astNode* temp1 = paraList->firstChild->firstChild->firstChild->nextSibling;
  //         curPosInParamTable->nType=2;
  //         strcpy(curPosInParamTable->keyword,temp1->tk.lexeme);
  //         //curPosInParamTable->extraData=initIdfTable();
  //         //traversing the record table to find the size of RECORD
  //         recTable *tmp=st->rHead;
  //         int recWidth=0;
  //         while(tmp!=NULL)
  //         {
  //           if(strcmp(tmp->keyword,temp1->tk.lexeme)==0)//matched the record in input list with that in record list in symbol table
  //           {
  //             idfTable *temporary;
  //             temporary=tmp->fields;
  //             while(temporary->next!=NULL)
  //             {
  //               recWidth+=temporary->width;
  //               temporary=temporary->next;
  //             }
  //             recWidth += temporary->width;
  //             curPosInParamTable->width =recWidth ;
  //           }
  //           tmp= tmp->next;
  //         }
  //         curPosInParamTable->offset=curPosInParamTable->width ;
  //         tempOffset=curPosInParamTable->offset;
  //       }
  //
  //       //Forwarding Logic
  //
  //       remListCheck=remListCheck->firstChild->firstChild->nextSibling->nextSibling;
  //     }// end of iterative while
  //
  //
  //   }
  //
  //
  //   //i want local variables  node
  //   astNode *stmts=opar->nextSibling;
  //   astNode *declarations=stmts->firstChild->nextSibling;
  //
  //   while((declarations->firstChild!=NULL) && declarations->firstChild->firstChild->nextSibling->nextSibling->nextSibling->firstChild==NULL  )//second condotion to check not global var
  //   {
  //         astNode *declaration=declarations->firstChild;
  //
  //      int tempOffset=0;
  //      if(strcmp(declaration->firstChild->nextSibling->firstChild->keyword,"<primitiveDatatype>")==0)
  //          {
  //
  //              astNode *pdt=declaration->firstChild->nextSibling->firstChild;
  //              astNode* temp1 = pdt->firstChild;
  //              if(iter->localVariable==NULL)
  //                iter->localVariable=initIdfTable();
  //            else
  //            {
  //                iter->localVariable->next=initIdfTable();
  //                iter->localVariable=iter->localVariable->next;
  //            }
  //             idfTable *curPosInParamTable=iter->localVariable;
  //            idfTable *tempo=iter->localVariable;
  //
  //             if(temp1->tk.tkType==TK_INT)
  //               {
  //                      curPosInParamTable->nType=0;
  //                      curPosInParamTable->width = NUM_WIDTH;
  //                        curPosInParamTable->offset=tempOffset;
  //
  //                  }
  //             else if(temp1->tk.tkType==TK_REAL)
  //                  {
  //                      curPosInParamTable->nType=1;
  //                      curPosInParamTable->width = RNUM_WIDTH;
  //                                    curPosInParamTable->offset= tempOffset;
  //                  }
  //
  //
  //             strcpy(curPosInParamTable->keyword,declaration->firstChild->nextSibling->nextSibling->keyword);
  //
  //
  //             //Offset should be always updated to tell the current position
  //             tempOffset= tempOffset + curPosInParamTable->width;
  //         }
  //        declarations=declaration->nextSibling;
  //
  //   }
  //
}// end of The Function Call Checking

  //Traversing Children
  if(root->firstChild!=NULL)
  populateFunctionTable(root->firstChild,st);
  temp = root->firstChild;

  if(temp!=NULL)
  {
    temp=temp->nextSibling;
    while(temp!=NULL)
    {
      populateFunctionTable(temp,st);
      temp=temp->nextSibling;
    }
  }
  //
}

void populateRecordTable(astNode *root,symbolTable* st)
{


  //Base Case
  if(root==NULL)
  return;
  astNode *temp=root;

  // ============================ record handling ==============================================

  if(strcmp(temp->keyword,"<typeDefinition>")==0 )
  {
    //populate symbol table for global Variables
    recTable* iter = st->rHead;
    //recTable* iter = st->rHead;
    //int tempOffset = 0;
    int firstTime=0;
    if(iter==NULL)
    {
      st->rHead=initRecTable();
      iter=st->rHead;
      firstTime=1;
    }
    else
    {
      while(iter->next!=NULL)
      iter=iter->next;
      iter->next=initRecTable();
      //tempOffset=iter->offset;// Offset got from previous value
      iter=iter->next;
      firstTime=0;
    }

    strcpy(iter->keyword,temp->firstChild->nextSibling->tk.lexeme);
    iter->fields=initIdfTable();
    astNode *recordFields = temp->firstChild;
    while(strcmp(recordFields->keyword,"<fieldDefinitions>")!=0)
    recordFields=recordFields->nextSibling;
    //handling first field definition
    astNode *temp2=recordFields->firstChild->firstChild->nextSibling;
    if(temp2->firstChild->tk.tkType==TK_INT)//temp2 points to primitiveDatatype
    {
      iter->fields->nType=0;
      iter->fields->width=NUM_WIDTH;
      iter->fields->offset=0;
    }
    else if(temp2->firstChild->tk.tkType==TK_REAL)
    {
      iter->fields->nType=1;
      iter->fields->width=RNUM_WIDTH;
      iter->fields->offset=0;
    }
    int tempOffset1=iter->fields->width;
    strcpy( iter->fields->keyword,temp2->nextSibling->tk.lexeme);

    iter->fields->next=initIdfTable();
    idfTable *nextemp=iter->fields->next;
    //handling second field definition
    astNode *temp3=recordFields->firstChild->nextSibling->firstChild->nextSibling;
    if(temp3->firstChild->tk.tkType==TK_INT)//temp3 points to primitiveDatatype
    {
      nextemp->nType=0;
      nextemp->width=NUM_WIDTH;
      nextemp->offset=tempOffset1;
    }
    else if(temp3->firstChild->tk.tkType==TK_REAL)
    {
      nextemp->nType=1;
      nextemp->width=RNUM_WIDTH;
      nextemp->offset=tempOffset1;
    }
    tempOffset1=nextemp->offset + nextemp->width ;
    strcpy(nextemp->keyword,temp3->nextSibling->tk.lexeme);
    // checking if record has more than two fields
    astNode *moreF=recordFields->firstChild->nextSibling->nextSibling;
    astNode *fDef;
    //
    while(strcmp(moreF->firstChild->tk.lexeme,"eps")!=0)
    {
      nextemp->next=initIdfTable();
      nextemp=nextemp->next;
      fDef=moreF->firstChild;
      astNode *temp3=fDef->firstChild->nextSibling;
      if(temp3->firstChild->tk.tkType==TK_INT)//temp3 points to primitiveDatatype
      {
        nextemp->nType=0;
        nextemp->width=NUM_WIDTH;
        nextemp->offset=tempOffset1;
      }
      else if(temp3->firstChild->tk.tkType==TK_REAL)
      {
        nextemp->nType=1;
        nextemp->width=RNUM_WIDTH;
        nextemp->offset=tempOffset1;
      }
      tempOffset1=(nextemp->offset) + (nextemp->width );
      strcpy(nextemp->keyword,temp3->nextSibling->tk.lexeme);

      moreF=moreF->firstChild->nextSibling;

    }


  }


  //Traversing Children
  if(root->firstChild!=NULL)
  populateRecordTable(root->firstChild,st);
  temp = root->firstChild;

  if(temp!=NULL)
  {
    temp=temp->nextSibling;
    while(temp!=NULL)
    {
      populateRecordTable(temp,st);
      temp=temp->nextSibling;
    }
  }

}

void populateGlobalTable(astNode *root,symbolTable* st)
{
  //Pre-Order Traversal

  //Base Case
  if(root==NULL)
  return;

  astNode *temp=root;

  // ============================GLOBAL POPULATION==============================================
  if(strcmp(temp->keyword,"<declaration>")==0 && strcmp(temp->firstChild->nextSibling->nextSibling->nextSibling->firstChild->tk.lexeme,"eps")!=0)
  {
    //populate symbol table for global Variables
    globTable* iter = st->gHead;
    int tempWidth = 0;//Summation of Previous Width to get OFFSET
    int firstTime=0;
    if(iter==NULL)
    {
      st->gHead=initGlobalTable();
      iter=st->gHead;
      firstTime=1;
      //Simulating the Record entry
      // st->rHead = initRecTable();
      // strcpy(st->rHead->keyword,"#marks");
      // st->rHead->fields = initIdfTable();
      // /*
      //   type real : maths;
      // 	type real: physics;
      // 	type real: chemistry;
      // */
      // strcpy(st->rHead->fields->keyword,"maths");
      // st->rHead->fields->width = RNUM_WIDTH;
      // st->rHead->fields->nType = 1;
      // st->rHead->fields->offset = 0;
      // st->rHead->fields->extraData = NULL;
      // st->rHead->fields->next = initIdfTable();
      // strcpy(st->rHead->fields->next->keyword,"physics");
      // st->rHead->fields->next->width = RNUM_WIDTH;
      // st->rHead->fields->next->nType = 1;
      // st->rHead->fields->next->offset = 0;
      // st->rHead->fields->next->extraData = NULL;
      // st->rHead->fields->next->next= initIdfTable();
      // strcpy(st->rHead->fields->next->next->keyword,"chemistry");
      // st->rHead->fields->next->next->width = RNUM_WIDTH;
      // st->rHead->fields->next->next->nType = 1;
      // st->rHead->fields->next->next->offset = 0;
      // st->rHead->fields->next->next->extraData = NULL;
      // st->rHead->fields->next->next->next=NULL;
      // st->rHead->next=NULL;
    }
    else
    {
      while(iter->next!=NULL)
      {
        tempWidth +=iter->width;
        iter=iter->next;
      }
      iter->next=initGlobalTable();//Giving a new Identifier Table for the information
      tempWidth+=iter->width;
      iter=iter->next;
      firstTime=0;
    }

    //Checking if Global or Not
    if(temp->firstChild->nextSibling->nextSibling->nextSibling!=NULL)
    {
      astNode* temp1 = temp->firstChild->nextSibling->nextSibling;//Ptr to TK_ID
      astNode* tIntGetter = temp->firstChild->nextSibling->firstChild->firstChild;

      if(strcmp(temp->firstChild->nextSibling->firstChild->keyword,"<primitiveDatatype>")==0)
      {
        if(tIntGetter->tk.tkType==TK_INT)
        iter->nType=0;
        else if(tIntGetter->tk.tkType==TK_REAL)
        iter->nType=1;

        strcpy(iter->keyword,temp1->tk.lexeme);

        if(iter->nType==0)
        {
          iter->width = NUM_WIDTH;
          if(firstTime==1)//Logic just to make the first entry zero
          iter->offset=0;
          else
          iter->offset=tempWidth;
        }
        else if(iter->nType==1)
        {
          iter->width = RNUM_WIDTH;
          if(firstTime==1)//Logic just to make the first entry zero
          iter->offset=0;
          else
          iter->offset=tempWidth;
        }

      }
      else if(strcmp(temp->firstChild->nextSibling->firstChild->keyword,"<constructedDatatype>")==0)
      {
        //temp is pointing to <declaration>
        //temp1 is pointing to TK_ID at this point
        astNode* temp2 = temp->firstChild->nextSibling->nextSibling;
        temp1 = temp->firstChild->nextSibling->firstChild->firstChild->nextSibling;
        iter->nType=2;
        strcpy(iter->keyword,temp2->tk.lexeme);
        iter->extraData=initIdfTable();
        //traversing the record table to find the size of RECORD
        recTable *tmp=st->rHead;
        int recWidth = 0;//Calculating the RecWidth

        while(tmp!=NULL)
        {
          if(strcmp(tmp->keyword,temp1->tk.lexeme)==0)//matched the record in input list with that in record list in symbol table
          {
            idfTable *temporary;
            temporary=tmp->fields;
            while(temporary->next!=NULL)
            {
              recWidth+=temporary->width;
              temporary=temporary->next;
            }
            recWidth += temporary->width;
            iter->width =recWidth ;
          }
          tmp= tmp->next;
        }//End of record def searching in the table
        if(firstTime==1)//Logic just to make the first entry zero
        iter->offset=0;
        else
        iter->offset=tempWidth;
        //Sabji nahi aanan chahiye
      }

    }

  }//End of global handling


  //Traversing Children
  if(root->firstChild!=NULL)
  populateGlobalTable(root->firstChild,st);
  temp = root->firstChild;

  if(temp!=NULL)
  {
    temp=temp->nextSibling;
    while(temp!=NULL)
    {
      populateGlobalTable(temp,st);
      temp=temp->nextSibling;
    }
  }
}

idfTable *findFunction(symbolTable *st, char *func_name)//returns table of locak variables
{
    funTable* temp=st->fHead;
    while(temp!=NULL)
    {
        if(strmp(temp->keyword,func_name)==0)
            return temp->localVariable;

        temp=temp->next;
    }
    return NULL;

}
int findIdentifier(idfTable *it, char *id); // returns ntype -1 if not found
{
    idfTable* temp=it;
    while(temp!=NULL)
    {
        if(strmp(temp->keyword,id)==0)
            return temp->nType;

        temp=temp->next;
    }
    return -1;

}
int findIdinGlobal(symbolTable *st, char *id); // returns ntype -1 if not found
{
    globTable* temp=st->gHead;
    while(temp!=NULL)
    {
        if(strmp(temp->keyword,id)==0)
            return temp->nType;

        temp=temp->next;
    }
    return -1;

}
idfTable *findRecordFields(symbolTable *st, char *rec_name); // returns idfTable within record table
{
    recTable* temp=st->rHead;
    while(temp!=NULL)
    {
        if(strmp(temp->keyword,rec_name)==0)
            return temp->fields;

        temp=temp->next;
    }
    return NULL;

}
