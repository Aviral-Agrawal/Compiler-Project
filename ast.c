#ifndef __AST_H__
#define __AST_H__
#include "ast.h"
#endif

int isUseless(treeNode *node)//Returns 1 if the node is useless
{
    if(node == NULL)
        return 1;
    else if(node->leaf == 0)
        return 0;
    else if(node->leaf == 1)
    {
        switch(node->tk.tkType)
        {
            case TK_ASSIGNOP:
            case TK_COMMENT:
            case TK_WITH:
            case TK_END:
            case TK_SQL:
            case TK_SQR:
            case TK_COMMA:
            case TK_SEM:
            case TK_COLON:
            case TK_DOT:
            case TK_ENDWHILE:
            case TK_OP:
            case TK_CL:
            case TK_THEN:
            case TK_ENDIF:
            case TK_ENDRECORD:
                return 1;
                break;
            default:
                return 0;
                break;
        }
    }
}//End of the function

void createAbstractSyntaxTree(astNode *ast_root, treeNode *p_root)
{
    if(p_root == NULL)
    {
        printf("Received NULL tree!!\n");
        return;
    }
    else if(p_root->leaf==1)
    {
      //equate
      astNode* astChild = (astNode*) malloc(sizeof(astNode));
      astChild->keyword = p_root->keyword;
      astChild->tk = p_root->tk;
      astChild->parent = ast_root;
      astChild->nextSibling = NULL;
      astChild->firstChild = NULL;
      astChild->leaf = 1;
      return;
    }
    else
    {
        treeNode *p_child_itr = p_root->firstChild;
        astNode* astChild = (astNode*) malloc(sizeof(astNode));
        ast_root->firstChild = astChild;
        int firstIteration = 1;//Use to not execute some code of the while loop in some iteration
        astNode* ast_child_itr = astChild;

        while(p_child_itr != NULL)
        {
            if(isUseless(p_child_itr))
            {
                p_child_itr = p_child_itr->nextSibling;
                continue;
            }
            if(!firstIteration)
                astChild=(astNode *)malloc(sizeof(astNode));
            astChild->keyword = p_child_itr->keyword;
            astChild->tk = p_child_itr->tk;
            astChild->leaf = 0;
            astChild->parent = ast_root;
            astChild->nextSibling = NULL;
            astChild->firstChild = NULL;
            if(p_child_itr->leaf == 0)
                createAbstractSyntaxTree(astChild, p_child_itr);
            else
                astChild->leaf = 1;
            if(firstIteration)
                ast_child_itr = astChild;
            else
            {
                ast_child_itr->nextSibling = astChild;
                ast_child_itr = astChild;
            }
            p_child_itr = p_child_itr->nextSibling;
            firstIteration = 0;
        }//End of while loop
        return;
    }//End of else statement

}//End of the function

void printASTinFile(astNode* root, FILE* fp)
{
    // static int i =1;
    // printf("%d izz\n",i++);
    //Base Case
    if(root==NULL)
        return;
    //Call to first child
    if(root->firstChild!=NULL)
        printASTinFile(root->firstChild,fp);


    if(root->leaf==1)
    {
        if(strcmp(root->tk.lexeme,"eps")==0)
            fprintf(fp,"%-32s | ","------");
        else
            fprintf(fp,"%-32s | ",root->tk.lexeme);

        fprintf(fp,"%-32d | ",root->tk.lineNo);

        if(root->tk.tkType==TK_ERROR)
            fprintf(fp,"%-32s | ","eps");
        else
            fprintf(fp,"%-32s | ",enumToString(root->tk.tkType));

        // %-32s | %-32s | %-10s | %-32s\n
        if(root->tk.tkType==TK_NUM || root->tk.tkType==TK_RNUM)
            fprintf(fp,"%-32s | ",root->tk.lexeme);
        else fprintf(fp,"%-32s | ","NaN");
        fprintf(fp,"%-32s | ",root->parent->tk.lexeme);
        fprintf(fp,"%-15s | ","Yes");
        fprintf(fp,"%-32s\n","------");
    }
    else
    {
        fprintf(fp,"%-32s | %-32d | %-32s | ","------",root->tk.lineNo,"------");
        // %-32s | %-32s | %-10s | %-32s\n
        if(root->tk.tkType==TK_NUM || root->tk.tkType==TK_RNUM)
            fprintf(fp,"%-32s | ",root->tk.lexeme);
        else fprintf(fp,"%-32s | ","NaN");

        if(root->parent==NULL)
            fprintf(fp,"%-32s | ","ROOT");
        else
            fprintf(fp,"%-32s | ",root->parent->tk.lexeme);
        fprintf(fp,"%-15s | ","No");
        fprintf(fp,"%-32s\n",root->tk.lexeme);
    }

    //Call to next Siblings
    astNode* temp = root->firstChild;
    if(temp!= NULL)//Error Check if root is a leaf node
    {
        temp=temp->nextSibling;
        while(temp!=NULL)
        {
            printASTinFile(temp,fp);
            temp=temp->nextSibling;
        }
    }

}//End of function
