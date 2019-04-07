#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__
#include "symbolTable.h"
#endif

char *typeToString(int type)
{
  char *key=(char *)malloc(10 *sizeof(char));
  if(type==0)
    strcpy(key,"int");
  else if(type==1)
    strcpy(key,"real");
  else if(type==2)
    strcpy(key,"record");
  return key;
}
int findArithmeticExpressionType(astNode *root, funTable *funPtr, symbolTable *st, astNode *leaf)
{
  if(root==NULL)
    return -1;
  else if(strcmp(root->keyword,"TK_ID")==0)
  {
    int type = findIdentifier(funPtr->localVariable, root->tk.lexeme); // singleOrRecId->TK_ID
    if(type==-1)
    {
      type = findIdentifier(funPtr->inputParams, root->tk.lexeme);
    }
    if(type==-1)
    {
      type = findIdentifier(funPtr->outputParams, root->tk.lexeme);
    }
    if(type==-1)
    {
      type = findIdinGlobal(st,root->tk.lexeme);
    }
    if(type==-1)
    {
      printf("\nLine %d:The variable <%s> is undeclared\n",root->tk.lineNo,root->tk.lexeme);
      return -1;
    }
    if(type == 2)
    {
      // handling records
      // TK_ID is a record instance
      idfTable *localRec = findRecordFields(st,root->tk.lexeme);
      // if(root->nextSibling->firstChild==NULL || root->nextSibling->firstChild->firstChild==NULL) // Equating records
      // {
      //   int sum=0;
      //   idfTable* temp=localRec;
      //   while(temp!=NULL)
      //   {
      //       sum+=temp->nType;
      //
      //       temp=temp->next;
      //   }
      //   return sum;
      // }
      if(root->nextSibling != NULL && strcmp(root->nextSibling->keyword,"<B>")==0 )
      {
        int fieldType = findIdentifier(localRec, root->nextSibling->firstChild->firstChild->tk.lexeme);
        return fieldType;
      }
    }
    leaf=root;
    return type;
  }
  // handle for records
  else if(strcmp(root->keyword,"TK_NUM")==0)
  {
    leaf=root;
    return 0;
  }
  else if(strcmp(root->keyword,"TK_RNUM")==0)
  {
    leaf=root;
    return 1;
  }
  else if(strcmp(root->keyword,"<arithmeticExpression>")==0)
  {
    return findArithmeticExpressionType(root->firstChild,funPtr,st,leaf);
  }

  // recursive calls for children and siblings
  astNode *leaf1,*leaf2;
  int type1=-1,type2=-1;
  if(root->firstChild!=NULL)
  {
    type1 = findArithmeticExpressionType(root->firstChild,funPtr,st,leaf);
    leaf1=leaf;
  }
  astNode *temp = root;

  if(temp!=NULL)
  {
    temp=temp->nextSibling;
    while(temp!=NULL)
    {
      type2 = findArithmeticExpressionType(temp,funPtr,st,leaf);
      leaf2=leaf;
      temp=temp->nextSibling;
    }
  }
  if(type1==type2)
    return type1;
  else if(type1==-1 || type2==-1)
    return -1;
  else
  {
    printf("\nLine no:%d Type Mismatch: The type  of %s: %s does not match with %s: %s\n",leaf1->tk.lineNo,leaf1->tk.lexeme,typeToString(type1),leaf2->tk.lexeme,typeToString(type2));
    return -1;
  }
}
void typeCheckerWithinFunction(astNode *root, funTable *funPtr, symbolTable *st)
{
  if(root==NULL)
    return;
  if(root->keyword!=NULL && strcmp(root->keyword,"<assignmentStmt>")==0)
  {
    int left_type = findIdentifier(funPtr->localVariable, root->firstChild->firstChild->tk.lexeme); // singleOrRecId->TK_ID
    if(left_type==-1)
    {
      left_type = findIdentifier(funPtr->inputParams, root->firstChild->firstChild->tk.lexeme);
    }
    if(left_type==-1)
    {
      left_type = findIdentifier(funPtr->outputParams, root->firstChild->firstChild->tk.lexeme);
    }
    if(left_type==-1)
    {
      left_type = findIdinGlobal(st,root->firstChild->firstChild->tk.lexeme);
    }
    if(left_type==-1)
    {
      printf("\nLine %d:The variable <%s> is undeclared\n",root->firstChild->firstChild->tk.lineNo,root->firstChild->firstChild->tk.lexeme);
      return;
    }
    astNode *leaf;
    int right_type;
    if(root->firstChild!=NULL && root->firstChild->nextSibling !=NULL)
    {
      right_type = findArithmeticExpressionType(root->firstChild->nextSibling->firstChild, funPtr, st, leaf);
    // sending arithmeticExpression's first child
    }
    else
      right_type=-1;
    if(right_type == -1)
      return;
    if(left_type == right_type)
      return;
    else if(left_type !=right_type)
    {
      printf("\nLine no:%d Type Mismatch:The type of %s: %s does not match with the right hand side with type %s\n",root->firstChild->firstChild->tk.lineNo,root->firstChild->firstChild->tk.lexeme,typeToString(left_type),typeToString(right_type));
    }

  }// end of if

  // for boolean expression only check var relOp var, other types are implicitly correct
  if(root->keyword!=NULL && strcmp(root->keyword,"<booleanExpression>")==0 && root->firstChild->keyword!=NULL && strcmp(root->firstChild->keyword,"<var>")==0)
  {
    int type1,type2;
    if(strcmp(root->firstChild->firstChild->keyword,"TK_ID")==0)
    {
      type1 = findIdentifier(funPtr->localVariable, root->firstChild->firstChild->tk.lexeme);
      if(type1==-1)
      {
        type1 = findIdentifier(funPtr->inputParams, root->firstChild->firstChild->tk.lexeme);
      }
      if(type1==-1)
      {
        type1 = findIdentifier(funPtr->outputParams, root->firstChild->firstChild->tk.lexeme);
      }
      if(type1==-1)
      {
        type1 = findIdinGlobal(st,root->firstChild->firstChild->tk.lexeme);
      }
      if(type1==-1)
      {
        printf("\nLine %d:The variable <%s> is undeclared\n",root->firstChild->firstChild->tk.lineNo,root->firstChild->firstChild->tk.lexeme);
        return;
      }
    }
    else if(strcmp(root->firstChild->firstChild->keyword,"TK_NUM")==0)
      type1=0;
    else if(strcmp(root->firstChild->firstChild->keyword,"TK_NUM")==1)
      type1=1;

      // for type 2
    if(strcmp(root->firstChild->nextSibling->nextSibling->firstChild->keyword,"TK_ID")==0)
    {
      type2 = findIdentifier(funPtr->localVariable, root->firstChild->nextSibling->nextSibling->firstChild->tk.lexeme);
      if(type2==-1)
      {
        type2 = findIdentifier(funPtr->inputParams, root->firstChild->nextSibling->nextSibling->firstChild->tk.lexeme);
      }
      if(type2==-1)
      {
        type2 = findIdentifier(funPtr->outputParams, root->firstChild->nextSibling->nextSibling->firstChild->tk.lexeme);
      }
      if(type2==-1)
      {
        type2 = findIdinGlobal(st,root->firstChild->nextSibling->nextSibling->firstChild->tk.lexeme);
      }
      if(type2==-1)
      {
        printf("\nLine %d:The variable <%s> is undeclared\n",root->firstChild->nextSibling->nextSibling->firstChild->tk.lineNo,root->firstChild->nextSibling->nextSibling->firstChild->tk.lexeme);
        return;
      }
    }
    else if(strcmp(root->firstChild->nextSibling->nextSibling->firstChild->keyword,"TK_NUM")==0)
      type2=0;
    else if(strcmp(root->firstChild->nextSibling->nextSibling->firstChild->keyword,"TK_RNUM")==0)
      type2=1;

    if(type1==type2)
      return;
    else if(type1!=type2)
    {
      printf("\nLine no:%d Type Mismatch:The type of %s:%s does not match with type of %s:%s\n",root->firstChild->firstChild->tk.lineNo,root->firstChild->firstChild->tk.lexeme,typeToString(type1),root->firstChild->nextSibling->nextSibling->firstChild->tk.lexeme,typeToString(type2));
    }
  }
  // recursive calls for children and siblings
  if(root->firstChild!=NULL)
    typeCheckerWithinFunction(root->firstChild,funPtr,st);
  astNode *temp = root->firstChild;

  if(temp!=NULL)
  {
    temp=temp->nextSibling;
    while(temp!=NULL)
    {
      typeCheckerWithinFunction(temp,funPtr,st);
      temp=temp->nextSibling;
    }
  }
}
int typeChecker(astNode *root, symbolTable *st)
{
  if(root==NULL)
    return 0;
  if(root->keyword!=NULL && strcmp(root->keyword,"<function>")==0)
  {
    funTable *funPtr=findFunction(st,root->firstChild->tk.lexeme);
    if(funPtr!=NULL)
      typeCheckerWithinFunction(root,funPtr,st);
  }
  if(root->keyword!=NULL && strcmp(root->keyword,"TK_MAIN")==0)
  {
    funTable *funPtr=findFunction(st,"_main");
    if(funPtr!=NULL)
      typeCheckerWithinFunction(root,funPtr,st);
  }
  // recursive calls for children and siblings
  if(root->firstChild!=NULL)
    typeChecker(root->firstChild,st);
  astNode *temp = root->firstChild;

  if(temp!=NULL)
  {
    temp=temp->nextSibling;
    while(temp!=NULL)
    {
      typeChecker(temp,st);
      temp=temp->nextSibling;
    }
  }
}
