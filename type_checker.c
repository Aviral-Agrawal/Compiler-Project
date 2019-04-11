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
  else
  {
    char test[5];
    sprintf(test,"%d",type);
    strcpy(key,"record type");
    strcat(key,test);
   }
    return key;
    
}
storeLeafType *findArithmeticExpressionType(astNode *root, funTable *funPtr, symbolTable *st, astNode *leaf)
{
  storeLeafType *node=(storeLeafType *)malloc(sizeof(storeLeafType));
  node->type=-1;
  node->leaf=NULL;
  if(root==NULL)
    return node;
  else if(strcmp(root->keyword,"TK_ID")==0)
  {
    if(root->nextSibling != NULL && strcmp(root->nextSibling->firstChild->keyword,"TK_FIELDID")==0 )
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
           return node;
        }
        
        idfTable *localRec = findRecordFields(st,type);
        int fieldType = findIdentifier(localRec, root->nextSibling->firstChild->firstChild->tk.lexeme);
        node->leaf=root->nextSibling->firstChild->firstChild;
        node->type=fieldType;
        return node;
    }
    else
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
      return node;
    }
    if(type!=-1)
    {
      node->leaf=root;
      node->type=type;
      return node;
    }
    
    }
  }
  // handle for records
  else if(strcmp(root->keyword,"TK_NUM")==0)
  {
    node->leaf=root;
    node->type=0;
    return node;
  }
  else if(strcmp(root->keyword,"TK_RNUM")==0)
  {
    node->leaf=root;
    node->type=1;
    return node;
  }
  else if(strcmp(root->keyword,"<arithmeticExpression>")==0)
  {
    return findArithmeticExpressionType(root->firstChild,funPtr,st,leaf);
  }

  // recursive calls for children and siblings
  astNode *leaf1=NULL,*leaf2=NULL;
  storeLeafType *type1,*type2;
  type1=(storeLeafType *)malloc(sizeof(storeLeafType));
  type1->type=-1;
  type1->leaf=NULL;
  type2=(storeLeafType *)malloc(sizeof(storeLeafType));
  type2->type=-1;
  type2->leaf=NULL;
  int flag1=0,flag2=0;
  if(root->firstChild!=NULL)
  {
    type1 = findArithmeticExpressionType(root->firstChild,funPtr,st,leaf);
    leaf1=leaf;
    flag1=1;
  }
  astNode *temp = root;

  if(temp!=NULL)
  {
    temp=temp->nextSibling;

    type2 = findArithmeticExpressionType(temp,funPtr,st,leaf);
    leaf2=leaf;
    flag2=1;

  }
  if(type1->type==-1 && type2->type==-1)
  {
      return node;
  }
  else if(type1->type==type2->type)
    return type1;
  else if(type1->type==-1 && type2->type!=-1)
    return type2;
  else if(type2->type==-1 && type1->type!=-1)
    return type1;
  else
  {
    if(type1->leaf!=NULL && type2->leaf!=NULL)
      printf("\nLine no:%d Type Mismatch: The type  of %s: %s does not match with %s: %s\n",type1->leaf->tk.lineNo,type1->leaf->tk.lexeme,typeToString(type1->type),type2->leaf->tk.lexeme,typeToString(type2->type));
    return node;
  }

}
void typeCheckerWithinFunction(astNode *root, funTable *funPtr, symbolTable *st)
{
  int left_type;
  int rec_type=-1;
  int flag=1; // for printing field or record
  if(root==NULL)
    return;
  if(root->keyword!=NULL && strcmp(root->keyword,"<assignmentStmt>")==0)
  {
    if(strcmp(root->firstChild->firstChild->nextSibling->firstChild->keyword,"eps")==0)
    {
      
      left_type = findIdentifier(funPtr->localVariable, root->firstChild->firstChild->tk.lexeme); // singleOrRecId->TK_ID
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
    }
    else if(strcmp(root->firstChild->firstChild->nextSibling->firstChild->keyword,"TK_FIELDID")==0 )
    {
      flag=0;
      int type = findIdentifier(funPtr->localVariable, root->firstChild->firstChild->tk.lexeme); // singleOrRecId->TK_ID
      if(type==-1)
      {
        type = findIdentifier(funPtr->inputParams, root->firstChild->firstChild->tk.lexeme);
      }
      if(type==-1)
      {
        type = findIdentifier(funPtr->outputParams, root->firstChild->firstChild->tk.lexeme);
      }
      if(type==-1)
      {
        type = findIdinGlobal(st,root->firstChild->firstChild->tk.lexeme);
      }
      if(type==-1)
      {
        printf("\nLine %d:The variable <%s> is undeclared\n",root->firstChild->firstChild->tk.lineNo,root->firstChild->firstChild->tk.lexeme);
        return;
      }
      idfTable *it = findRecordFields(st, type);
      if(it==NULL)
      {
        printf("\nLine %d: Record not declared, but instance %s used", root->firstChild->firstChild->tk.lineNo,root->firstChild->firstChild->tk.lexeme);
        return;
      }
      
      left_type=findIdentifier(it,root->firstChild->firstChild->nextSibling->firstChild->tk.lexeme);
    }
    
    
    astNode *leaf=NULL;
    storeLeafType *right_type;
    if(root->firstChild!=NULL && root->firstChild->nextSibling !=NULL)
    {
      right_type = findArithmeticExpressionType(root->firstChild->nextSibling->firstChild, funPtr, st, leaf);
    // sending arithmeticExpression's first child
    }
    else
      right_type->type=-1;
    if(right_type->type == -1)
      return;
    if(left_type == right_type->type)
      return;
    else if(left_type !=right_type->type)
    {
      if(flag==1)
      {
      printf("\nLine no:%d Type Mismatch:The type of %s: %s does not match with the right hand side with type %s\n",root->firstChild->firstChild->tk.lineNo,root->firstChild->firstChild->tk.lexeme,typeToString(left_type),typeToString(right_type->type));
      }
      else
      {
        printf("\nLine no:%d Type Mismatch:The type of %s: %s does not match with the right hand side with type %s\n",root->firstChild->firstChild->tk.lineNo,root->firstChild->firstChild->tk.lexeme,typeToString(left_type),typeToString(right_type->type));
        }
        
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

    if(type1!=type2)
    {
      printf("\nLine no:%d Type Mismatch:The type of %s:%s does not match with type of %s:%s\n",root->firstChild->firstChild->tk.lineNo,root->firstChild->firstChild->tk.lexeme,typeToString(type1),root->firstChild->nextSibling->nextSibling->firstChild->tk.lexeme,typeToString(type2));
    }
  }
  // recursive calls for children and siblings
  typeCheckerWithinFunction(root->firstChild,funPtr,st);
  typeCheckerWithinFunction(root->nextSibling,funPtr,st);
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
  astNode *temp = root;

  if(temp!=NULL)
  {
    temp=temp->nextSibling;
    typeChecker(temp,st);
  }
}
