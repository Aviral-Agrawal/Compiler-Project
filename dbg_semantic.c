#ifndef __SEMANTIC_H__
#define __SEMANTIC_H__
#include "semantic.h"
#endif

void addVariable(whileVariables* wv, char* keyword)
{
  if(wv->next==NULL)
  {
      wv->keyword = keyword;
      wv->next = NULL;
      return;
      //Base Case Handled
  }

  whileVariables* temp = wv;
  while(temp->next!=NULL)
    temp = temp->next;
  temp->next = (whileVariables*)malloc(sizeof(whileVariables));
  temp = temp->next;
  temp->keyword = keyword;
  temp->next = NULL;
}

void fillWhileVariables(whileVariables* wv ,astNode* root,funTable* ft)
{
  //Case 2 is the base case

  //Case 1
  //<booleanExpression>===> <booleanExpression>  <logicalOp>  <booleanExpression>
  if(strcmp(root->firstChild->keyword,"<booleanExpression>")==0)
  {
    fillWhileVariables(wv,root->firstChild,ft);
    fillWhileVariables(wv,root->firstChild->nextSibling->nextSibling,ft);
  }
  //Case 2
  //<booleanExpression>===>  <var> <relationalOp> <var>
  else if(strcmp(root->firstChild->keyword,"<var>")==0)
  {
    astNode *t1 =root->firstChild, *t2 = root->firstChild->nextSibling->nextSibling;

    if(strcmp(t1->keyword,"TK_ID")==0)
      addVariable(wv,t1->tk.lexeme);

    if(strcmp(t2->keyword,"TK_ID")==0)
      addVariable(wv,t2->tk.lexeme);
  }
  //Case 3
  //<booleanExpression>===> TK_NOT  <booleanExpression>
  else if(strcmp(root->firstChild->nextSibling->keyword,"<booleanExpression>")==0)
  {
    fillWhileVariables(wv,root->firstChild->nextSibling,ft);
  }
}


// void variableErrorPrinter(char* keyword,astNode* root,symbolTable* st,int flag1, int flag2)
// {
//   //root is pointing to <function>
//
//   //FLAG --> LOGIC
//   // 0 local
//   // 1 global
//   // 2 record
//   if(flag1==0 && flag2==1)
//   {
//     //Local Variable is trying to redefine the global variable
//     astNode* ipar = root->firstChild->nextSibling->firstChild->nextSibling->nextSibling->nextSibling;
//     //ipar is pointing to parameter_list of inputList
//     astNode* i1 = ipar->firstChild->nextSibling;
//     while(strcmp(i1->keyword,"eps")!=0)
//     {
//       i1 = i1->->firstChild->nextSibling;
//       if(strcmp(i1->keyword,keyword)==0)
//       {
//         printf("Line %d : Variable %s is being redefined\n", i1->tk.lineNo,keyword);
//       }
//       i1= i1->nextSibling->firstChild->firstChild;
//     }
//
//     astNode* opar = root->firstChild->nextSibling->nextSibling->firstChild->nextSibling->nextSibling->nextSibling;
//     if(strcmp(opar->firstChild->keyword,"eps")!=0)
//     {
//       i1 = opar->firstChild->nextSibling;
//       while(strcmp(i1->keyword,"eps")!=0)
//       {
//         i1 = i1->->firstChild->nextSibling;
//         if(strcmp(i1->keyword,keyword)==0)
//         {
//           printf("Line %d : Variable %s is being redefined\n", i1->tk.lineNo,keyword);
//         }
//         i1= i1->nextSibling->firstChild->firstChild;
//       }
//     }
//
//     astNode* loc = root->firstChild->nextSibling->nextSibling->nextSibling->firstChild->nextSibling;
//     //loc points to <declarations>
//     i1 = loc->firstChild;
//     while(strcmp(i1->keyword,"eps")!=0)
//     {
//       astNode* i2 = i1->firstChild->nextSibling->nextSibling;
//       if(strcmp(i2->keyword,keyword)==0)
//       {
//         printf("Line %d : Variable %s is being redefined\n", i2->tk.lineNo,keyword);
//       }
//       i1=i1->nextSibling->firstChild;
//     }
//
//
//   }
//   else if(flag1==1 && flag2==1)
//   {
//     //Global variable redefined
//     //Can be implemeneted here
//   }
//   else if(flag==0 && flag2==0)
//   {
//
//     int count =0;
//
//     astNode* ipar = root->firstChild->nextSibling->firstChild->nextSibling->nextSibling->nextSibling;
//     //ipar is pointing to parameter_list of inputList
//     astNode* i1 = ipar->firstChild->nextSibling;
//     while(strcmp(i1->keyword,"eps")!=0)
//     {
//       i1 = i1->->firstChild->nextSibling;
//       if(strcmp(i1->keyword,keyword)==0)
//       {
//         if(count++ >1)
//         printf("Line %d : Variable %s is being redefined\n", i1->tk.lineNo,keyword);
//       }
//       i1= i1->nextSibling->firstChild->firstChild;
//     }
//
//     astNode* opar = root->firstChild->nextSibling->nextSibling->firstChild->nextSibling->nextSibling->nextSibling;
//     if(strcmp(opar->firstChild->keyword,"eps")!=0)
//     {
//       i1 = opar->firstChild->nextSibling;
//       while(strcmp(i1->keyword,"eps")!=0)
//       {
//         i1 = i1->->firstChild->nextSibling;
//         if(strcmp(i1->keyword,keyword)==0)
//         {
//           if(count++ >1)
//           printf("Line %d : Variable %s is being redefined\n", i1->tk.lineNo,keyword);
//         }
//         i1= i1->nextSibling->firstChild->firstChild;
//       }
//     }
//
//     astNode* loc = root->firstChild->nextSibling->nextSibling->nextSibling->firstChild->nextSibling;
//     //loc points to <declarations>
//     i1 = loc->firstChild;
//     while(strcmp(i1->keyword,"eps")!=0)
//     {
//       astNode* i2 = i1->firstChild->nextSibling->nextSibling;
//       if(strcmp(i2->keyword,keyword)==0)
//       {
//         if(count++ >1)
//         printf("Line %d : Variable %s is being redefined\n", i2->tk.lineNo,keyword);
//       }
//       i1=i1->nextSibling->firstChild;
//     }
//
//
//   }
//
//
//
// }


//finds the number of occurances of this function name
int findFunNameOcc(funTable* ft, char* funName)
{
  int countOcc = 0;
  if(funName == NULL || ft == NULL)
  {
    printf("Erreneous input received at findFunNameOcc\n");
    return -1;
  }
  while(ft != NULL)
  {
    if(strcmp(ft->keyword, funName) == 0)
    countOcc++;
    ft = ft->next;
  }
  return countOcc;
}//End of the function

funTable* getFunctionEntryInTable(funTable* fHead, char* lexeme)
{
  while(fHead!=NULL)
  {
    if(strcmp(fHead->keyword,lexeme)==0)
    return fHead;
    fHead=fHead->next;
  }
  return NULL;
}

idfTable* findIdEntry(idfTable* localVarTable, char* lexeme)//returns an entry of the idfTable
{
	idfTable* itr = localVarTable;
    if(localVarTable == NULL || lexeme == NULL)
        return NULL;
    while(itr != NULL)
    {
        if(strcmp(itr->keyword, lexeme) == 0)
			return itr;
        itr = itr->next;
    }
    return NULL;
}

globTable* findIdEntryInGlobal(symbolTable* st, char* lexeme)//returns an entry of the idfTable
{
	globTable* itr = st->gHead;
    if(st == NULL || lexeme == NULL)
        return NULL;
    while(itr != NULL)
    {
        if(strcmp(itr->keyword, lexeme) == 0)
			return itr;
        itr = itr->next;
    }
    return NULL;
}


void semanticAnalysis(astNode* root, symbolTable* st, char* functionName)
//functionName is used only when root == <function> and then it makes a recusive call
{
  //Checking if multiple Record Declared anywhere
  static int multipleRecord = 0;//Only once it will be checked
  if(multipleRecord++ == 0)
  {
    recTable* rt = st->rHead;
    int i_r = 0 ,j_r = 0;
    while(rt!=NULL)
    {
        i_r++;
        j_r=0;
      recTable* temp = st->rHead;
      int count = 0;
      int flagPrint = 0;
      while(temp!=NULL)
      {
          j_r++;
        if(strcmp(temp->keyword,rt->keyword)==0)
        {
          count++;
          if(count>1 && j_r>i_r &&!flagPrint)
          {
              flagPrint = 1;
            printf("Line %d : Record %s is being redefined\n", temp->lineNo, temp->keyword);
          }
        }
        temp = temp->next;
      }
      rt= rt->next;
    }
  }//end of record multiple checking

  //Checking if multiple Global Variables
  static int multipleGlobal = 0;//Only once it will be checked
  if(multipleGlobal++ == 0)
  {
    globTable *gt = st->gHead;
    int i_r = 0 ,j_r = 0;
    while(gt!=NULL)
    {
        i_r++;
        j_r=0;
      globTable* temp = st->gHead;
      int count = 0;
      int flagPrint = 0;
      while(temp!=NULL)
      {
          j_r++;
        if(strcmp(temp->keyword,gt->keyword)==0)
        {
          count++;
          if(count>1 && j_r>i_r &&!flagPrint)
          {
              flagPrint = 1;
              printf("Line %d : Global Variable %s is being redefined as global variable\n", temp->lineNo, temp->keyword);
          }
        }
        temp = temp->next;
      }
      gt= gt->next;
    }
  }//end of global multiple checking


  //CHECKING REDEFINITIONS OF FUNCTIONS
  static int functionChecker =0;
  if(functionChecker++ ==0)
  {
        funTable* ft = st->fHead;
        int i_r = 0 , j_r = 0;
        while(ft!=NULL)
        {
            i_r++;
            j_r=0;
            funTable* temp = st->fHead;
            int count = 0;
            int flagPrint = 0;
            while(temp!=NULL)
            {
                j_r++;
                if(strcmp(temp->keyword,ft->keyword)==0)
                {
                    count++;
                    if(count> 1 && j_r > i_r && !flagPrint)
                    {
                        flagPrint= 1;
                        printf("Line %d : The function %s is being overloaded.\n", temp->lineNo, temp->keyword);

                    }
                }
                temp = temp->next;
            }
            ft= ft->next;
        }
  }//end of function redeclaration checker

  //Concerns related to the mainFunction subtree <mainFunction>
  if(strcmp(root->keyword, "<mainFunction>") == 0)
  {
    //Extracting all the function declarations inside this function subtree
    //Requires traversal of the <otherStmts> non-terminal subtree
    astNode* temp = root;
    temp = temp->firstChild->nextSibling;//<stmts>
    temp = temp->firstChild->nextSibling->nextSibling;//<otherStmts>
    while(strcmp(temp->firstChild->keyword, "<stmt>") == 0)
    {
      //if the set of statements in this subtree are not function calls
      if(strcmp(temp->firstChild->firstChild->keyword, "<funCallStmt>") != 0)
      {
          temp = temp->firstChild->nextSibling;
          continue;
      }
      //Checking recusion
      //str1 = called function name
      char* str1 = temp->firstChild->firstChild->firstChild->nextSibling->nextSibling->tk.lexeme;
      //str2 = callee function name
      char* str2 = root->firstChild->tk.lexeme;
      int lNo = temp->firstChild->firstChild->firstChild->nextSibling->nextSibling->tk.lineNo;
      if(strcmp(str1, str2) == 0)
        printf("Line %d : The function %s is a recursive call.\n", lNo, str1);
      //Checking prior declaration
      else if(findFunNameOcc(st->fHead, str1) == 0)
        printf("Line %d : The function %s is undefined.\n", lNo, str1);

      //Handling the number and type of input and output parameters
      else
      {
        astNode* outPar = temp->firstChild->firstChild->firstChild;//<outputParameters>
        funTable* calledFun = findFunction(st, str1);
        funTable* calleeFun = findFunction(st, str2);
        idfTable* outParList = calledFun->outputParams;
        if(calledFun == NULL || calleeFun == NULL)
        {
          printf("Error in finding the function!!\n");
          exit(0);
        }
        //if the number of output parameters specified while call = 0
        //i.e., <outputParameters> -> eps
        else if(strcmp(outPar->firstChild->keyword, "<idList>") != 0 && outParList != NULL || strcmp(outPar->firstChild->keyword, "<idList>") == 0 && outParList == NULL)
        printf("Line %d : The number of output parameters at function call %s() is incorrect.\n", lNo, str1);
        else if(strcmp(outPar->firstChild->keyword, "<idList>") == 0)
        {
          outPar = outPar->firstChild;//<idList>
          while(strcmp(outPar->keyword, "<idList>") == 0)
          {
            if(outParList == NULL)
            {
              printf("Line %d : The number of output parameters at function call %s() is incorrect.\n", lNo, str1);
              break;
            }
            int varType = findIdentifier(calleeFun->localVariable, outPar->firstChild->tk.lexeme);
            if(varType == -1)
                varType = findIdinGlobal(st, outPar->firstChild->tk.lexeme);

            int varType2 = findIdentifier(calledFun->outputParams, outParList->keyword);
            if(varType == -1)
            printf("Line %d : Variable %s is undeclared.\n", lNo, outPar->firstChild->tk.lexeme);
            else if(varType != varType2)
            {
              printf("Line %d : The type ", lNo);
              if(varType == 0)
              printf("<int> ");
              else if(varType == 1)
              printf("<real> ");
              else
              printf("<record>");
              printf("of variable <%s> returned does not match with the type ", outPar->firstChild->tk.lexeme);
              if(varType2 == 0)
              printf("<int> ");
              else if(varType2 == 1)
              printf("<real> ");
              else
              printf("<record>");
              printf("of the formal output parameter <%s>.\n", outParList->keyword);
            }
            outParList = outParList->next;
            outPar = outPar->firstChild->nextSibling->firstChild;
          }//End of else if
        }
        //else, there is nothing wrong with the output parameters

        //Handling the number and type of input parameters
        astNode* inPar = temp->firstChild->firstChild->firstChild->nextSibling->nextSibling->nextSibling->nextSibling;//<inputParameters>
        idfTable* inParList = calledFun->inputParams;
        if(calledFun == NULL || calleeFun == NULL)
        {
          printf("Error in finding the function!!\n");
          exit(0);
        }
        //if the number of input parameters specified while call = 0
        //i.e., <inputParameters> -> eps
        else if(strcmp(inPar->firstChild->keyword, "<idList>") != 0 && inParList != NULL || strcmp(inPar->firstChild->keyword, "<idList>") == 0 && inParList == NULL)
        printf("Line %d : The number of input parameters at function call %s() is incorrect.\n", lNo, str1);
        else if(strcmp(inPar->firstChild->keyword, "<idList>") == 0)
        {
          inPar = inPar->firstChild;//<idList>
          while(strcmp(inPar->keyword, "<idList>") == 0)
          {
            if(inParList == NULL)
            {
              printf("Line %d : The number of output parameters at function call %s() is incorrect.\n", lNo, str1);
              break;
            }
            int varType = findIdentifier(calleeFun->localVariable, inPar->firstChild->tk.lexeme);
            if(varType == -1)
                varType = findIdinGlobal(st, inPar->firstChild->tk.lexeme);
            int varType2 = findIdentifier(calledFun->inputParams, inParList->keyword);
            if(varType == -1)
            printf("Line %d : Variable %s is undeclared.\n", lNo, inPar->firstChild->tk.lexeme);
            if(varType != varType2)
            {
              printf("Line %d : The type ", lNo);
              if(varType == 0)
              printf("<int> ");
              else if(varType == 1)
              printf("<real> ");
              else
              printf("<record>");
              printf("of variable <%s> returned does not match with the type ", inPar->firstChild->tk.lexeme);
              if(varType2 == 0)
              printf("<int> ");
              else if(varType2 == 1)
              printf("<real> ");
              else
              printf("<record>");
              printf("of the formal input parameter <%s>.\n", inParList->keyword);
            }
            inParList = inParList->next;
            inPar = inPar->firstChild->nextSibling->firstChild;
          }//End of while loop
        }//End of else if
        //else, there is nothing wrong with the input parameters
      }//End of else stmt

      temp = temp->firstChild->nextSibling;
    }//End of the while loop



    // //============DONE ================
    // //Checking for global variable shadowing by local variable
    // globTable* glb = st->gHead;
    // funTable* ft = getFunctionEntryInTable(st->fHead,root->firstChild->tk.lexeme);
    // if(ft!=NULL)
    // {
    //   //Global Checking in Input Paramaeters
    //   idfTable* idf = ft->inputParams;
    //   globTable* globalRoamer = glb;
    //
    //   while(idf!=NULL)
    //   {
    //       globalRoamer = glb;
    //     while(globalRoamer!=NULL)
    //     {
    //       if(strcmp(idf->keyword,globalRoamer->keyword)==0)
    //       {
    //         printf("Line %d : Global Variable %s is being redefined by a local variable\n", idf->lineNo, idf->keyword);
    //       }
    //       globalRoamer = globalRoamer->next;
    //     }
    //     idf=idf->next;
    //   }
    //
    //   //Global Checking in Output Paramaeters
    //   idf = ft->outputParams;
    //   globalRoamer = glb;
    //   while(idf!=NULL)
    //   {
    //       globalRoamer = glb;
    //     while(globalRoamer!=NULL)
    //     {
    //       if(strcmp(idf->keyword,globalRoamer->keyword)==0)
    //       {
    //         printf("Line %d : Global Variable %s is being redefined by a local variable\n", idf->lineNo, idf->keyword);
    //       }
    //       globalRoamer = globalRoamer->next;
    //     }
    //     idf=idf->next;
    //   }
    //
    //   //Global Checking in Local Variables
    //   idf = ft->localVariable;
    //   globalRoamer = glb;
    //   while(idf!=NULL)
    //   {
    //       globalRoamer = glb;
    //     while(globalRoamer!=NULL)
    //     {
    //       if(strcmp(idf->keyword,globalRoamer->keyword)==0)
    //       {
    //         printf("Line %d : Global Variable %s is being redefined by a local variable\n", idf->lineNo, idf->keyword);
    //
    //       }
    //       globalRoamer = globalRoamer->next;
    //     }
    //     idf=idf->next;
    //   }
    //
    // }
    //
    // //An identifier cannot be declared multiple times in the same scope.
    // ft = getFunctionEntryInTable(st->fHead,root->firstChild->tk.lexeme);
    // if(ft!=NULL)
    // {
    //   idfTable* i1 = ft->inputParams;
    //   idfTable* i2 = ft->outputParams;
    //   idfTable* i3 = ft->localVariable;
    //
    //   //traversing through all three tables
    //   int i=0;
    //   int i_r=0,j_r=0;
    //   for(i=0;i<3;i++)
    //   {
    //     if(i==1)
    //     i1=i2;
    //     else if(i==2)
    //     i1=i3;
    //     while(i1!=NULL)
    //     {
    //       i_r++;
    //       j_r=0;
    //       int flagPrint = 0;
    //       idfTable* t1 = ft->inputParams;
    //       idfTable* t2 = ft->outputParams;
    //       idfTable* t3 = ft->localVariable;
    //       int count = 0; //keeps track of the occurence of the currnt variable across all the tables
    //
    //       while(t1!=NULL)
    //       {
    //         j_r++;
    //         if(strcmp(i1->keyword,t1->keyword)==0)
    //         {
    //             count++;
    //             if(count>1 && j_r>i_r &&!flagPrint)
    //               {
    //                 flagPrint = 1;
    //                 printf("Line %d : Local Variable %s is being redefined by a local variable\n", t1->lineNo, i1->keyword);
    //                 // variableErrorPrinter(i1->keyword,root,st,0,0);
    //               }
    //         }
    //         t1 = t1->next;
    //       }
    //
    //       while(t2!=NULL)
    //       {
    //         j_r++;
    //         if(strcmp(i1->keyword,t2->keyword)==0)
    //         {
    //             count++;
    //             if(count>1 && j_r>i_r &&!flagPrint)
    //               {
    //                 flagPrint = 1;
    //                 printf("Line %d : Local Variable %s is being redefined by a local variable\n", t2->lineNo, i1->keyword);
    //                 // variableErrorPrinter(i1->keyword,root,st,0,0);
    //               }
    //         }
    //         t2 = t2->next;
    //       }
    //
    //       while(t3!=NULL)
    //       {
    //         j_r++;
    //         if(strcmp(i1->keyword,t3->keyword)==0)
    //         {
    //             count++;
    //             if(count>1 && j_r>i_r &&!flagPrint)
    //               {
    //                 flagPrint = 1;
    //                 printf("Line %d : Local Variable %s is being redefined by a local variable\n", t3->lineNo, i1->keyword);
    //                 // variableErrorPrinter(i1->keyword,root,st,0,0);
    //               }
    //         }
    //         t3 = t3->next;
    //       }
    //
    //
    //       i1 = i1->next;
    //     }
    //   }//End of for loop
    //
    //
    // }//End of local checking

    // An identifier(record type instance) must be declared before its use.
    // An identifier used beyond its scope must be viewed as undefined
    // Check in the subtree of <otherStmts>
	astNode* stmtsRoot = root->firstChild->nextSibling;//<stmts>
	//Recursing for <otherStmts>
	astNode* otherStmtsRoot = stmtsRoot->firstChild->nextSibling->nextSibling;//<otherStmts>
	while(strcmp(otherStmtsRoot->firstChild->keyword, "<stmt>") == 0)
	{
		char* tempStr = otherStmtsRoot->firstChild->firstChild->keyword;
		if(strcmp(tempStr, "<iterativeStmt>") == 0 || strcmp(tempStr, "<conditionalStmt>") == 0 || strcmp(tempStr, "<ioStmt>") == 0)
			semanticAnalysis(otherStmtsRoot->firstChild->firstChild, st, root->firstChild->tk.lexeme);
		otherStmtsRoot = otherStmtsRoot->firstChild->nextSibling;
	}

  }//End of <mainFunction> concerns

  //Concerns related to the function subtree <function>
  if(strcmp(root->keyword, "<function>") == 0)
  {
    //Extracting all the function declarations inside this function subtree
    //Requires traversal of the <otherStmts> non-terminal subtree
    astNode* temp = root;
    temp = temp->firstChild->nextSibling->nextSibling->nextSibling;//<stmts>
    temp = temp->firstChild->nextSibling->nextSibling;//<otherStmts>
    while(strcmp(temp->firstChild->keyword, "<stmt>") == 0)
    {
      //if the set of statements in this subtree are not function calls
      if(strcmp(temp->firstChild->firstChild->keyword, "<funCallStmt>") != 0)
      {
          temp = temp->firstChild->nextSibling;
          continue;
      }
      //Checking recusion
      //str1 = called function name
      char* str1 = temp->firstChild->firstChild->firstChild->nextSibling->nextSibling->tk.lexeme;
      //str2 = callee function name
      char* str2 = root->firstChild->tk.lexeme;
      int lNo = temp->firstChild->firstChild->firstChild->nextSibling->nextSibling->tk.lineNo;
      if(strcmp(str1, str2) == 0)
        printf("Line %d : The function %s is a recursive call.\n", lNo, str1);
      //Checking prior declaration
      else if(findFunNameOcc(st->fHead, str1) == 0)
        printf("Line %d : The function %s is undefined.\n", lNo, str1);

      //Handling the number and type of input and output parameters
      else
      {
        astNode* outPar = temp->firstChild->firstChild->firstChild;//<outputParameters>
        funTable* calledFun = findFunction(st, str1);
        funTable* calleeFun = findFunction(st, str2);
        idfTable* outParList = calledFun->outputParams;
        if(calledFun == NULL || calleeFun == NULL)
        {
          printf("Error in finding the function!!\n");
          exit(0);
        }
        //if the number of output parameters specified while call = 0
        //i.e., <outputParameters> -> eps
        else if(strcmp(outPar->firstChild->keyword, "<idList>") != 0 && outParList != NULL || strcmp(outPar->firstChild->keyword, "<idList>") == 0 && outParList == NULL)
        printf("Line %d : The number of output parameters at function call %s() is incorrect.\n", lNo, str1);
        else if(strcmp(outPar->firstChild->keyword, "<idList>") == 0)
        {
          outPar = outPar->firstChild;//<idList>
          while(strcmp(outPar->keyword, "<idList>") == 0)
          {
            if(outParList == NULL)
            {
              printf("Line %d : The number of output parameters at function call %s() is incorrect.\n", lNo, str1);
              break;
            }
            int varType = findIdentifier(calleeFun->localVariable, outPar->firstChild->tk.lexeme);
            if(varType == -1)
                varType = findIdinGlobal(st, outPar->firstChild->tk.lexeme);
            if(varType == -1)
                varType = findIdentifier(calleeFun->inputParams, outPar->firstChild->tk.lexeme);
            if(varType == -1)
                varType = findIdentifier(calleeFun->outputParams, outPar->firstChild->tk.lexeme);
            int varType2 = findIdentifier(calledFun->outputParams, outParList->keyword);
            if(varType == -1)
            printf("Line %d : Variable %s is undeclared.\n", lNo, outPar->firstChild->tk.lexeme);
            else if(varType != varType2)
            {
              printf("Line %d : The type ", lNo);
              if(varType == 0)
              printf("<int> ");
              else if(varType == 1)
              printf("<real> ");
              else
              printf("<record>");
              printf("of variable <%s> returned does not match with the type ", outPar->firstChild->tk.lexeme);
              if(varType2 == 0)
              printf("<int> ");
              else if(varType2 == 1)
              printf("<real> ");
              else
              printf("<record>");
              printf("of the formal output parameter <%s>.\n", outParList->keyword);
            }
            outParList = outParList->next;
            outPar = outPar->firstChild->nextSibling->firstChild;
          }//End of else if
        }
        //else, there is nothing wrong with the output parameters

        //Handling the number and type of input parameters
        astNode* inPar = temp->firstChild->firstChild->firstChild->nextSibling->nextSibling->nextSibling->nextSibling;//<inputParameters>
        idfTable* inParList = calledFun->inputParams;
        if(calledFun == NULL || calleeFun == NULL)
        {
          printf("Error in finding the function!!\n");
          exit(0);
        }
        //if the number of input parameters specified while call = 0
        //i.e., <inputParameters> -> eps
        else if(strcmp(inPar->firstChild->keyword, "<idList>") != 0 && inParList != NULL || strcmp(inPar->firstChild->keyword, "<idList>") == 0 && inParList == NULL)
        printf("Line %d : The number of input parameters at function call %s() is incorrect.\n", lNo, str1);
        else if(strcmp(inPar->firstChild->keyword, "<idList>") == 0)
        {
          inPar = inPar->firstChild;//<idList>
          while(strcmp(inPar->keyword, "<idList>") == 0)
          {
            if(inParList == NULL)
            {
              printf("Line %d : The number of output parameters at function call %s() is incorrect.\n", lNo, str1);
              break;
            }
            int varType = findIdentifier(calleeFun->localVariable, inPar->firstChild->tk.lexeme);
            if(varType == -1)
                varType = findIdinGlobal(st, inPar->firstChild->tk.lexeme);
            if(varType == -1)
                varType = findIdentifier(calleeFun->inputParams, inPar->firstChild->tk.lexeme);
            if(varType == -1)
                varType = findIdentifier(calleeFun->outputParams, inPar->firstChild->tk.lexeme);
            int varType2 = findIdentifier(calledFun->inputParams, inParList->keyword);
            if(varType == -1)
            printf("Line %d : Variable %s is undeclared.\n", lNo, inPar->firstChild->tk.lexeme);
            if(varType != varType2)
            {
              printf("Line %d : The type ", lNo);
              if(varType == 0)
              printf("<int> ");
              else if(varType == 1)
              printf("<real> ");
              else
              printf("<record>");
              printf("of variable <%s> returned does not match with the type ", inPar->firstChild->tk.lexeme);
              if(varType2 == 0)
              printf("<int> ");
              else if(varType2 == 1)
              printf("<real> ");
              else
              printf("<record>");
              printf("of the formal input parameter <%s>.\n", inParList->keyword);
            }
            inParList = inParList->next;
            inPar = inPar->firstChild->nextSibling->firstChild;
          }//End of while loop
        }//End of else if
        //else, there is nothing wrong with the input parameters
      }//End of else stmt

      temp = temp->firstChild->nextSibling;
    }//End of the while loop



    // //============DONE ================
    // //Checking for global variable shadowing by local variable
    // globTable* glb = st->gHead;
    // funTable* ft = getFunctionEntryInTable(st->fHead,root->firstChild->tk.lexeme);
    // if(ft!=NULL)
    // {
    //   //Global Checking in Input Paramaeters
    //   idfTable* idf = ft->inputParams;
    //   globTable* globalRoamer = glb;
    //
    //   while(idf!=NULL)
    //   {
    //       globalRoamer = glb;
    //     while(globalRoamer!=NULL)
    //     {
    //       if(strcmp(idf->keyword,globalRoamer->keyword)==0)
    //       {
    //         printf("Line %d : Global Variable %s is being redefined by a local variable\n", idf->lineNo, idf->keyword);
    //       }
    //       globalRoamer = globalRoamer->next;
    //     }
    //     idf=idf->next;
    //   }
    //
    //   //Global Checking in Output Paramaeters
    //   idf = ft->outputParams;
    //   globalRoamer = glb;
    //   while(idf!=NULL)
    //   {
    //       globalRoamer = glb;
    //     while(globalRoamer!=NULL)
    //     {
    //       if(strcmp(idf->keyword,globalRoamer->keyword)==0)
    //       {
    //         printf("Line %d : Global Variable %s is being redefined by a local variable\n", idf->lineNo, idf->keyword);
    //       }
    //       globalRoamer = globalRoamer->next;
    //     }
    //     idf=idf->next;
    //   }
    //
    //   //Global Checking in Local Variables
    //   idf = ft->localVariable;
    //   globalRoamer = glb;
    //   while(idf!=NULL)
    //   {
    //       globalRoamer = glb;
    //     while(globalRoamer!=NULL)
    //     {
    //       if(strcmp(idf->keyword,globalRoamer->keyword)==0)
    //       {
    //         printf("Line %d : Global Variable %s is being redefined by a local variable\n", idf->lineNo, idf->keyword);
    //
    //       }
    //       globalRoamer = globalRoamer->next;
    //     }
    //     idf=idf->next;
    //   }
    //
    // }
    //
    // //An identifier cannot be declared multiple times in the same scope.
    // ft = getFunctionEntryInTable(st->fHead,root->firstChild->tk.lexeme);
    // if(ft!=NULL)
    // {
    //   idfTable* i1 = ft->inputParams;
    //   idfTable* i2 = ft->outputParams;
    //   idfTable* i3 = ft->localVariable;
    //
    //   //traversing through all three tables
    //   int i=0;
    //   int i_r=0,j_r=0;
    //   for(i=0;i<3;i++)
    //   {
    //     if(i==1)
    //     i1=i2;
    //     else if(i==2)
    //     i1=i3;
    //     while(i1!=NULL)
    //     {
    //       i_r++;
    //       j_r=0;
    //       int flagPrint = 0;
    //       idfTable* t1 = ft->inputParams;
    //       idfTable* t2 = ft->outputParams;
    //       idfTable* t3 = ft->localVariable;
    //       int count = 0; //keeps track of the occurence of the currnt variable across all the tables
    //
    //       while(t1!=NULL)
    //       {
    //         j_r++;
    //         if(strcmp(i1->keyword,t1->keyword)==0)
    //         {
    //             count++;
    //             if(count>1 && j_r>i_r &&!flagPrint)
    //               {
    //                 flagPrint = 1;
    //                 printf("Line %d : Local Variable %s is being redefined by a local variable\n", t1->lineNo, i1->keyword);
    //                 // variableErrorPrinter(i1->keyword,root,st,0,0);
    //               }
    //         }
    //         t1 = t1->next;
    //       }
    //
    //       while(t2!=NULL)
    //       {
    //         j_r++;
    //         if(strcmp(i1->keyword,t2->keyword)==0)
    //         {
    //             count++;
    //             if(count>1 && j_r>i_r &&!flagPrint)
    //               {
    //                 flagPrint = 1;
    //                 printf("Line %d : Local Variable %s is being redefined by a local variable\n", t2->lineNo, i1->keyword);
    //                 // variableErrorPrinter(i1->keyword,root,st,0,0);
    //               }
    //         }
    //         t2 = t2->next;
    //       }
    //
    //       while(t3!=NULL)
    //       {
    //         j_r++;
    //         if(strcmp(i1->keyword,t3->keyword)==0)
    //         {
    //             count++;
    //             if(count>1 && j_r>i_r &&!flagPrint)
    //               {
    //                 flagPrint = 1;
    //                 printf("Line %d : Local Variable %s is being redefined by a local variable\n", t3->lineNo, i1->keyword);
    //                 // variableErrorPrinter(i1->keyword,root,st,0,0);
    //               }
    //         }
    //         t3 = t3->next;
    //       }
    //
    //
    //       i1 = i1->next;
    //     }
    //   }//End of for loop
    //
    //
    // }//End of local checking
    //
    // //=========== OUTPUT ALL UPDATED CHECKING ==================
    // char* fName = root->firstChild->tk.lexeme;
    // ft = getFunctionEntryInTable(st->fHead,root->firstChild->tk.lexeme);
    // idfTable* outTab = ft->outputParams;
    //
    // astNode* outputConfirmer = root->firstChild->nextSibling->nextSibling->nextSibling;
    // outputConfirmer= outputConfirmer->firstChild->nextSibling->nextSibling;
    // //points yo <otherStmts
    //
    // //Boolean array for marking true for outputParamas
    // int arrSize = 0;
    // while(outTab!=NULL)
    // {
    //   arrSize++;
    //   outTab=outTab->next;
    // }
    // outTab = ft->outputParams;
    // int* arr = (int*) malloc(sizeof(int)*arrSize);
    // memset(arr,0,sizeof(arr));
    //
    // while(strcmp(outputConfirmer->firstChild->keyword,"eps")!=0)
    // {
    //   astNode* temp = outputConfirmer->firstChild->firstChild;
    //   outTab = ft->outputParams;
    //   if(strcmp(temp->keyword,"<assignmentStmt>")==0)
    //   {
    //     char* keyword = temp->firstChild->firstChild->tk.lexeme;
    //     int pos = 0;
    //     while(outTab!=NULL)
    //     {
    //       pos++;
    //       if(strcmp(keyword,outTab->keyword)==0)
    //       {
    //         arr[pos] = 1;//Marks boolean Aray true
    //       }
    //       outTab = outTab->next;
    //     }
    //   }
    //   outputConfirmer = outputConfirmer->firstChild->nextSibling;
    // }
    //
    // //Fianlly Printing which output parameters are not being updated
    // int i = 0;
    // outTab = ft->outputParams;
    // for(i=0;i<arrSize;i++)
    // {
    //   if(arr[i]!=1)
    //   {
    //     printf("Line %d : Function: %s Output Parameter : %s Not updated\n",root->firstChild->tk.lineNo+1,fName,outTab->keyword);
    //   }
    //   outTab = outTab->next;
    // }
    //=============END OF OUTPUT PARAM CHECKER===============

    // An identifier(record type instance) must be declared before its use.
    // An identifier used beyond its scope must be viewed as undefined
    // Check in the subtree of both, <otherStmts> and <returnStmt>
	astNode* stmtsRoot = root->firstChild->nextSibling->nextSibling->nextSibling;//<stmts>
	//Recursing for <otherStmts>
	astNode* otherStmtsRoot = stmtsRoot->firstChild->nextSibling->nextSibling;//<otherStmts>
	while(strcmp(otherStmtsRoot->firstChild->keyword, "<stmt>") == 0)
	{
		char* tempStr = otherStmtsRoot->firstChild->firstChild->keyword;
		if(strcmp(tempStr, "<iterativeStmt>") == 0 || strcmp(tempStr, "<conditionalStmt>") == 0 || strcmp(tempStr, "<ioStmt>") == 0)
			semanticAnalysis(otherStmtsRoot->firstChild->firstChild, st, root->firstChild->tk.lexeme);
		otherStmtsRoot = otherStmtsRoot->firstChild->nextSibling;
	}
	//Recursing for <returnStmt>
	semanticAnalysis(stmtsRoot->firstChild->nextSibling->nextSibling->nextSibling, st, root->firstChild->tk.lexeme);
  }//End of <function> concerns

  //Concerns related to the while
  else if(strcmp(root->keyword, "<iterativeStmt>") == 0)
  {
      //<booleanExpression> handled by the type checker
      // <stmt>===> <assignmentStmt> handled by the type checker
      astNode* temp = root->firstChild->nextSibling->nextSibling;//<stmt>
      while(strcmp(temp->keyword, "<stmt>") == 0)
      {
          if(strcmp(temp->firstChild->keyword, "<iterativeStmt>") == 0)
                semanticAnalysis(temp->firstChild, st, functionName);
          else if(strcmp(temp->firstChild->keyword, "<conditionalStmt>") == 0)
                semanticAnalysis(temp->firstChild, st, functionName);
          else if(strcmp(temp->firstChild->keyword, "<ioStmt>") == 0)
                semanticAnalysis(temp->firstChild, st, functionName);
          temp = temp->nextSibling->firstChild;
      }//End of while loop

	  // //=============================WHILE VAR UPDATE=================================
	  //       //At least one variable gets updated in the while loop
	  //       astNode* boolTraversal = root->firstChild->nextSibling;
	  //       //Points to <booleanExpression>
      //
	  //       whileVariables* wv = (whileVariables*) malloc(sizeof(whileVariables));
	  //       wv->next = NULL;
	  //       wv->keyword = NULL;
	  //       //Populates all the while variables of this function
	  //       funTable* ft =  getFunctionEntryInTable(st->fHead,functionName);
	  //       fillWhileVariables(wv,boolTraversal,ft);
      //
	  //       astNode* bt2 = root->firstChild->nextSibling->nextSibling;
	  //       int whileUpdate = 0;
	  //       if(strcmp(bt2->firstChild->keyword,"<assignmentStmt>")==0)
	  //       {
	  //         bt2 = bt2->firstChild->firstChild->firstChild;
	  //         char* keyword = bt2->keyword;
	  //         whileVariables* temp = wv;
	  //         while(temp!=NULL)
	  //         {
	  //           if(strcmp(temp->keyword,keyword)==0)
	  //             whileUpdate++;
	  //           temp = temp->next;
	  //         }
	  //       }
	  //       //<stmt> pointed bt bt2
	  //       astNode* bt3 = bt2->nextSibling;
	  //       while(strcmp(bt3->firstChild->keyword,"<eps>")==0)
	  //       {
	  //         bt2 = bt3->firstChild;
	  //         bt3 = bt3->firstChild->nextSibling;
	  //         if(strcmp(bt2->firstChild->keyword,"<assignmentStmt>")==0)
	  //         {
	  //           bt2 = bt2->firstChild->firstChild->firstChild;
	  //           char* keyword = bt2->keyword;
	  //           whileVariables* temp = wv;
	  //           while(temp!=NULL)
	  //           {
	  //             if(strcmp(temp->keyword,keyword)==0)
	  //               whileUpdate++;
	  //             temp = temp->next;
	  //           }
	  //         }
      //
	  //       }
      //
	  //       if(whileUpdate==0)
	  //         printf("Error in function : %s , While Loop Variables not updated\n",functionName);

  }//End of <iterativeStmt> concerns

  //Concerns related to the if stmt
  else if(strcmp(root->keyword, "<conditionalStmt>") == 0)
  {
      //<booleanExpression> handled by the type checker
      // <stmt>===> <assignmentStmt> handled by the type checker
      astNode* temp = root->firstChild->nextSibling->nextSibling;//<stmt>
      while(strcmp(temp->keyword, "<stmt>") == 0)
      {
          if(strcmp(temp->firstChild->keyword, "<iterativeStmt>") == 0)
                semanticAnalysis(temp->firstChild, st, functionName);
          else if(strcmp(temp->firstChild->keyword, "<conditionalStmt>") == 0)
                semanticAnalysis(temp->firstChild, st, functionName);
          else if(strcmp(temp->firstChild->keyword, "<ioStmt>") == 0)
                semanticAnalysis(temp->firstChild, st, functionName);
          temp = temp->nextSibling->firstChild;
      }//End of while loop
      temp = root->firstChild->nextSibling->nextSibling->nextSibling->nextSibling;//<elsePart>
      if(temp->firstChild == NULL)//<elsePart>==> TK_ENDIF
        return;
      temp = temp->firstChild->nextSibling;//<stmt>
      while(strcmp(temp->keyword, "<stmt>") == 0)
      {
          if(strcmp(temp->firstChild->keyword, "<iterativeStmt>") == 0)
                semanticAnalysis(temp->firstChild, st, functionName);
          else if(strcmp(temp->firstChild->keyword, "<conditionalStmt>") == 0)
                semanticAnalysis(temp->firstChild, st, functionName);
          else if(strcmp(temp->firstChild->keyword, "<ioStmt>") == 0)
                semanticAnalysis(temp->firstChild, st, functionName);
          temp = temp->nextSibling->firstChild;
      }//End of while loop
  }//End of <conditionalStmt> concerns

  //Concerns related to the io stmt
  else if(strcmp(root->keyword, "<ioStmt>") == 0)
  {
    astNode* temp = root->firstChild->nextSibling;//<singleOrRecId>
    funTable* containingFunction = findFunction(st, functionName);
    if(containingFunction == NULL)
    {
        printf("findFunction returned a NULL.\n");
        return;
    }
    int lNo = temp->firstChild->tk.lineNo;
    if(temp->firstChild->nextSibling == NULL)//<singleOrRecId>===>TK_ID
    {
      int foundInInput = findIdentifier(containingFunction->inputParams, temp->firstChild->tk.lexeme);
      int foundInOutput = findIdentifier(containingFunction->outputParams, temp->firstChild->tk.lexeme);
      int foundInLocal = findIdentifier(containingFunction->localVariable, temp->firstChild->tk.lexeme);
      int foundInGlobal = findIdinGlobal(st, temp->firstChild->tk.lexeme);
      if(foundInInput == -1 && foundInOutput == -1 && foundInLocal == -1 && foundInGlobal == -1)
          printf("Line %d : The variable %s is undeclared.\n", lNo, temp->firstChild->tk.lexeme);
    }
    else//<singleOrRecId>===>TK_ID <new_24>
        //<new_24> ===> eps
        //<new_24>==> TK_DOT TK_FIELDID
    {
        int foundInInput = findIdentifier(containingFunction->inputParams, temp->firstChild->tk.lexeme);
        int foundInOutput = findIdentifier(containingFunction->outputParams, temp->firstChild->tk.lexeme);
        int foundInLocal = findIdentifier(containingFunction->localVariable, temp->firstChild->tk.lexeme);
        int foundInGlobal = findIdinGlobal(st, temp->firstChild->tk.lexeme);
        if(foundInInput == -1 && foundInOutput == -1 && foundInLocal == -1 && foundInGlobal == -1)
        {
            printf("Line %d : The variable %s is undeclared.\n", lNo, temp->firstChild->tk.lexeme);
            return;
        }
        if(temp->firstChild->nextSibling->firstChild->tk.tkType != TK_FIELDID)
            return;
        idfTable* recEntry = findIdEntry(containingFunction->localVariable, temp->firstChild->tk.lexeme);
        if(recEntry == NULL)
            recEntry = findIdEntry(containingFunction->inputParams, temp->firstChild->tk.lexeme);
        if(recEntry == NULL)
            recEntry = findIdEntry(containingFunction->outputParams, temp->firstChild->tk.lexeme);
        globTable* recEntry2;
        int flagGlobal = 0;
        if(recEntry == NULL)
        {
            recEntry2 = findIdEntryInGlobal(st, temp->firstChild->tk.lexeme);
            flagGlobal = 1;
        }
        if(flagGlobal == 0)
        {
            if(recEntry == NULL)
            {
                printf("Line %d : The record variable %s is undeclared.\n", lNo, temp->firstChild->tk.lexeme);
                return;
            }
            if(recEntry->extraData == NULL)
            {
                printf("Line %d : The field of variable %s is not defined.\n", lNo, temp->firstChild->tk.lexeme);
                return;
            }
            idfTable* fieldItr = recEntry->extraData;
            while(fieldItr != NULL)
            {
                if(strcmp(temp->firstChild->nextSibling->firstChild->tk.lexeme, fieldItr->keyword) == 0)
                    return;
            }
            printf("Line %d : The field %s is incorrect for the record variable %s.\n", lNo, temp->firstChild->nextSibling->firstChild->tk.lexeme, temp->firstChild->tk.lexeme);
        }
        if(flagGlobal == 1)
        {
            if(recEntry2 == NULL)
            {
                printf("Line %d : The record variable %s is undeclared.\n", lNo, temp->firstChild->tk.lexeme);
                return;
            }
            if(recEntry2->extraData == NULL)
            {
                printf("Line %d : The field of variable %s is not defined.\n", lNo, temp->firstChild->tk.lexeme);
                return;
            }
            idfTable* fieldItr = recEntry2->extraData;
            while(fieldItr != NULL)
            {
                if(strcmp(temp->firstChild->nextSibling->firstChild->tk.lexeme, fieldItr->keyword) == 0)
                    return;
            }
            printf("Line %d : The field %s is incorrect for the record variable %s.\n", lNo, temp->firstChild->nextSibling->firstChild->tk.lexeme, temp->firstChild->tk.lexeme);
        }
    }
  }//End of <ioStmt> concerns

  //Concerns related to the return stmt
  //The output parameter should only be returned by the return stmt
  //The number of output parameters returned and the order in which they are returned
  else if(strcmp(root->keyword, "<returnStmt>") == 0)
  {
	  if(strcmp(root->firstChild->nextSibling->firstChild->keyword, "<idList>") != 0)
	  	return;
	  astNode* returnList = root->firstChild->nextSibling->firstChild;//<idList>
	  int lNo = root->firstChild->tk.lineNo;
	  funTable* containingFunction = findFunction(st, functionName);
      idfTable* outParList = containingFunction->outputParams;
	  while(strcmp(returnList->keyword, "<idList>") == 0)
	  {
          if(outParList == NULL)
          {
              printf("Line %d : Returning incorrect number of output parameters for function %s.\n", lNo, containingFunction->keyword);
              break;
          }
          if(strcmp(outParList->keyword, returnList->firstChild->tk.lexeme) != 0)
              printf("Line %d : The variable %s does not match formal output parameter %s.\n", lNo, returnList->firstChild->tk.lexeme, outParList->keyword);
		  returnList = returnList->firstChild->nextSibling->firstChild;
          outParList = outParList->next;
	  }
  }//End of <returnStmt> concerns

  //default case for all the non-terminals that are not usefull, need for recursion
  //Excluding the main function for now
  if(root->firstChild != NULL && root->firstChild->leaf != 1 && (strcmp(root->keyword, "<function>") != 0) && (strcmp(root->keyword, "<iterativeStmt>") != 0) && (strcmp(root->keyword, "<conditionalStmt>") != 0) && (strcmp(root->keyword, "<ioStmt>") != 0) && (strcmp(root->keyword, "<returnStmt>") != 0))
    semanticAnalysis(root->firstChild, st, functionName);
  if(root->nextSibling != NULL && root->nextSibling->leaf != 1)
    semanticAnalysis(root->nextSibling, st, functionName);
}//End of the function

//     Lexeme |LineNumber | TokenName |ValueIfNumber | ParentNodeSymbol |  IsLeafNode | NodeSymbol
