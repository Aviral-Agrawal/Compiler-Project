#ifndef __SEMANTIC_H__
#define __SEMANTIC_H__
#include "semantic.h"
#endif


void variableErrorPrinter(char* keyword,astNode* root,symbolTable* st,int flag1, int flag2)
{
  //root is pointing to <function>

  //FLAG --> LOGIC
  // 0 local
  // 1 global
  // 2 record
  if(flag1==0 && flag2==1)
  {
    //Local Variable is trying to redefine the global variable
    astNode* ipar = root->firstChild->nextSibling->firstChild->nextSibling->nextSibling->nextSibling;
    //ipar is pointing to parameter_list of inputList
    astNode* i1 = ipar->firstChild->nextSibling;
    while(strcmp(i1->keyword,"eps")!=0)
    {
      i1 = i1->->firstChild->nextSibling;
      if(strcmp(i1->keyword,keyword)==0)
      {
        printf("Line %d : Variable %s is being redefined\n", i1->tk.lineNo,keyword);
      }
      i1= i1->nextSibling->firstChild->firstChild;
    }

    astNode* opar = root->firstChild->nextSibling->nextSibling->firstChild->nextSibling->nextSibling->nextSibling;
    if(strcmp(opar->firstChild->keyword,"eps")!=0)
    {
      i1 = opar->firstChild->nextSibling;
      while(strcmp(i1->keyword,"eps")!=0)
      {
        i1 = i1->->firstChild->nextSibling;
        if(strcmp(i1->keyword,keyword)==0)
        {
          printf("Line %d : Variable %s is being redefined\n", i1->tk.lineNo,keyword);
        }
        i1= i1->nextSibling->firstChild->firstChild;
      }
    }

    astNode* loc = root->firstChild->nextSibling->nextSibling->nextSibling->firstChild->nextSibling;
    //loc points to <declarations>
    i1 = loc->firstChild;
    while(strcmp(i1->keyword,"eps")!=0)
    {
  astNode* i2 = i1->firstChild->nextSibling->nextSibling;
      if(strcmp(i2->keyword,keyword)==0)
      {
        printf("Line %d : Variable %s is being redefined\n", i2->tk.lineNo,keyword);
      }
      i1=i1->nextSibling->firstChild;
    }


  }
  else if(flag1==1 && flag2==1)
  {
    //Global variable redefined
    //Can be implemeneted here
  }
  else if(flag==0 && flag2==0)
  {

    int count =0;

    astNode* ipar = root->firstChild->nextSibling->firstChild->nextSibling->nextSibling->nextSibling;
    //ipar is pointing to parameter_list of inputList
    astNode* i1 = ipar->firstChild->nextSibling;
    while(strcmp(i1->keyword,"eps")!=0)
    {
      i1 = i1->->firstChild->nextSibling;
      if(strcmp(i1->keyword,keyword)==0)
      {
        if(count++ >1)
          printf("Line %d : Variable %s is being redefined\n", i1->tk.lineNo,keyword);
      }
      i1= i1->nextSibling->firstChild->firstChild;
    }

    astNode* opar = root->firstChild->nextSibling->nextSibling->firstChild->nextSibling->nextSibling->nextSibling;
    if(strcmp(opar->firstChild->keyword,"eps")!=0)
    {
      i1 = opar->firstChild->nextSibling;
      while(strcmp(i1->keyword,"eps")!=0)
      {
        i1 = i1->->firstChild->nextSibling;
        if(strcmp(i1->keyword,keyword)==0)
        {
          if(count++ >1)
            printf("Line %d : Variable %s is being redefined\n", i1->tk.lineNo,keyword);
        }
        i1= i1->nextSibling->firstChild->firstChild;
      }
    }

    astNode* loc = root->firstChild->nextSibling->nextSibling->nextSibling->firstChild->nextSibling;
    //loc points to <declarations>
    i1 = loc->firstChild;
    while(strcmp(i1->keyword,"eps")!=0)
    {
      astNode* i2 = i1->firstChild->nextSibling->nextSibling;
      if(strcmp(i2->keyword,keyword)==0)
      {
        if(count++ >1)
          printf("Line %d : Variable %s is being redefined\n", i2->tk.lineNo,keyword);
      }
      i1=i1->nextSibling->firstChild;
    }


  }



}


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

void semanticAnalysis(astNode* root, symbolTable* st, char* functionName)
//functionName is used only when root == <function> and then it makes a recusive call
{
    //Concerns related to the function subtree <function>
    if(strcmp(root->keyword, "<function>") == 0)
    {
        //creating an entry into the function table
        int funOcc = findFunNameOcc(st->fHead, root->firstChild->tk->lexeme);
        //Handling function overloading
        if(funOcc != 0)
        {
            printf("Line %d : The function %s is being overloaded.", root->firstChild->tk->lineNo, root->firstChild->tk->lexeme);
            return;
        }
        //Extracting all the function declarations inside this function subtree
        //Requires traversal of the <otherStmts> non-terminal subtree
        astNode* temp = root;
        temp = temp->firstChild->nextSibling->nextSibling->nextSibling;//<stmts>
        temp = temp->firstChild->nextSibling->nextSibling;//<otherStmts>
        while(strcmp(temp->firstChild->keyword, "<stmt>") == 0)
        {
            //if the set of statements in this subtree are not function calls
            if(strcmp(temp->firstChild->firstChild->keyword, "<funCallStmt>") != 0)
                continue;
            //Checking recusion
            //str1 = called function name
            char* str1 = temp->firstChild->firstChild->firstChild->nextSibling->nextSibling->tk->lexeme;
            //str2 = callee function name
            char* str2 = root->firstChild->tk->lexeme;
            int lNo = temp->firstChild->firstChild->firstChild->nextSibling->nextSibling->tk->lineNo;
            if(strcmp(str1, str2) == 0)
                printf("Line %d : The function %s is a recursive call.\n", lNo, str1);
            //Checking prior declaration
            else if(findFunNameOcc(st->fHead, str1) == 0)
                printf("Line %d : The function %s is undefined.", lNo, str1);

            //Handling the number and type of output parameters
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
                        int varType = findIdentifier(calleeFun->localVariable, outPar->firstChild->tk->lexeme);
                        int varType2 = findIdentifier(calledFun->outputParams, outParList->keyword);
                        if(varType == -1)
                            printf("Line %d : Variable %s is undeclared.\n", lNo, outPar->firstChild->tk->lexeme);
                        else if(varType != varType2)
                        {
                            printf("Line %d : The type ", lNo);
                            if(varType == 0)
                                printf("<int> ");
                            else if(varType == 1)
                                printf("<real> ");
                            else
                                printf("<record>");
                            printf("of variable <%s> returned does not match with the type ", outPar->firstChild->tk->lexeme);
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
            }

            //Handling the number and type of input parameters
            else
            {
                astNode* inPar = temp->firstChild->firstChild->firstChild->nextSibling->nextSibling->nextSibling->nextSibling;//<inputParameters>
                funTable* calledFun = findFunction(st, str1);
                funTable* calleeFun = findFunction(st, str2);
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
                        int varType = findIdentifier(calleeFun->localVariable, inPar->firstChild->tk->lexeme);
                        int varType2 = findIdentifier(calledFun->outputParams, inParList->keyword);
                        if(varType == -1)
                            printf("Line %d : Variable %s is undeclared.\n", lNo, inPar->firstChild->tk->lexeme);
                        if(varType != varType2)
                        {
                            printf("Line %d : The type ", lNo);
                            if(varType == 0)
                                printf("<int> ");
                            else if(varType == 1)
                                printf("<real> ");
                            else
                                printf("<record>");
                            printf("of variable <%s> returned does not match with the type ", inPar->firstChild->tk->lexeme);
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
                //else, there is nothing wrong with the output parameters
            }//End of else stmt


            temp = temp->firstChild->nextSibling;
        }//End of the while loop


        //Checking for global variable shadowing
        globTable* glb = st->gHead;
        funTable* ft = getFunctionEntryInTable(st->fHead,ft->keyword);
         if(ft!=NULL)
        {
          //Global Checking in Input Paramaeters
              idfTable* idf = ft->inputParams;
              globTable* globalRoamer = glb;

              while(idf!=NULL)
              {
                while(globalRoamer!=NULL)
                {
                   globalRoamer = glb;
                  if(strcmp(idf->keyword,globalRoamer->lexeme)==0)
                  {
                    //Check for multiple occurences of the variable and global variables
                    variableErrorPrinter(idf->keyword,root,st,0,1);
                  }
                  globalRoamer = globalRoamer->next;
                }
                idf=idf->next;
              }

              //Global Checking in Output Paramaeters
              idf = ft->outputParams;
              globalRoamer = glb;
              while(idf!=NULL)
              {
                while(globalRoamer!=NULL)
                {
                   globalRoamer = glb;
                  if(strcmp(idf->keyword,globalRoamer->lexeme)==0)
                  {
                    //===LINE NO====
                    int lineNo ;
                    printf("Line %d : Variable %s is being redefined", lineNo, idf->keyword);
                  }
                  globalRoamer = globalRoamer->next;
                }
                idf=idf->next;
              }

              //Global Checking in Local Variables
              idf = ft->localVariable;
              globalRoamer = glb;
              while(idf!=NULL)
              {
                while(globalRoamer!=NULL)
                {
                   globalRoamer = glb;
                  if(strcmp(idf->keyword,globalRoamer->lexeme)==0)
                  {
                    //===LINE NO====
                    int lineNo ;
                    printf("Line %d : Variable %s is being redefined", lineNo, idf->keyword);
                  }
                  globalRoamer = globalRoamer->next;
                }
                idf=idf->next;
              }

        }

        //An identifier cannot be declared multiple times in the same scope.
        ft = getFunctionEntryInTable(st->fHead,ft->keyword);
        if(ft!=NULL)
        {
          idfTable* i1 = ft->inputParams;
          idfTable* i2 = ft->outputParams;
          idfTable* i3 = ft->localVariable;

//traversing through all three tables
          int i=0;
          for(i=0;i<3;i++)
          {
              if(i==1)
                i1=i2;
              else if(i==2)
                i1=i3;
            while(i1!=NULL)
            {
              idfTable* t1 = ft->inputParams;
              idfTable* t2 = ft->outputParams;
              idfTable* t3 = ft->localVariable;
              int count = 0; //keeps track of the occurence of the currnt variable across all the tables

              while(t1!=NULL)
              {
                if(strcmp(i1->keyword,t1->keyword)==0)
                  count++;
                t1 = t1->next;
              }

              while(t2!=NULL)
              {
                if(strcmp(i1->keyword,t2->keyword)==0)
                  count++;
                t2 = t2->next;
              }

              while(t3!=NULL)
              {
                if(strcmp(i1->keyword,t3->keyword)==0)
                  count++;
                t3 = t3->next;
              }

              if(count>1)
  variableErrorPrinter(i1->keyword,root,st,0,0);

              i1 = i1->next;
            }
        }//End of for loop


    }//End of local checking

        //An identifier(record type instance) must be declared before its use.
        //An identifier used beyond its scope must be viewed as undefined
        //Traverse the AST to find every variable used and check it's scope and prior declaration
        //check in the subtree of both, <otherStmts> and <returnStmt>
        //Path - <function> -> <stmts> -> <otherStmts> -> <stmt>
        //<stmt>===> <assignmentStmt> //already handled in the type-checker
        //<stmt>==> <iterativeStmt> //invoke a recusive call for this block
        //<stmt>==><conditionalStmt> //invoke a recursive call for this block
        //<stmt>==><ioStmt> //invoke a recursive call for this block
        //<stmt>==> <funCallStmt> //already handled while finding the types of the input/output parameters




    }//End of <function> concerns
    //Concerns related to the while
    else if(strcmp(root->keyword, "<iterativeStmt>") == 0)
    {

    }
    //Concerns related to the if stmt
    else if(strcmp(root->keyword, "<conditionalStmt>") == 0)
    {

    }
    //Concerns related to the if stmt
    else if(strcmp(root->keyword, "<ioStmt>") == 0)
    {
        astNode* temp = root->firstChild->nextSibling;//<singleOrRecId>
        if(temp->firstChild->nextSibling == NULL)//<singleOrRecId>===>TK_ID
        {
            int found = findIdentifier()
        }
        else if(temp->firstChild->nextSibling->nextSibling == NULL)//<singleOrRecId>===>TK_ID <new_24>
                                                                   //<new_24> ===> eps
                                                                   //<new_24>==> TK_DOT TK_FIELDID
        {

        }
        else//<singleOrRecId>==> TK_ID TK_DOT  TK_FIELDID
        {

        }
    }
    else//default case for all the non-terminals that are not usefull
    {

    }
}//End of the function

//     Lexeme |LineNumber | TokenName |ValueIfNumber | ParentNodeSymbol |  IsLeafNode | NodeSymbol
