/*
* Compiler Construction Project
* Group Number 40
* AVIRAL AGRAWAL 2016A7PS0077P
* JITVAN SHUKLA  2016A7PS0083P
* SHUBHAM SHARMA 2016A7PS0115P
* AKSHIT AGARWAL  2016A7PS0122P

*/
#ifndef __N_ARY_H__
#define __N_ARY_H__
#include "nAry.h"
#endif

treeNode* createTreeNode(char* keyword, treeNode* parent)
{
    treeNode* node = (treeNode*) malloc(sizeof(treeNode));
    node->keyword = keyword;
    strcpy(node->tk.lexeme,node->keyword);
    node->tk.tkType =TK_ERROR;
    node->tk.lineNo=-1;
    node->parent= parent;
    node->firstChild= NULL;
    node->nextSibling= NULL;
    node->leaf = 0;// By default non-leaf
    return node;
}

treeNode* locateNextNonTerminal(treeNode* curNode)
{
    treeNode* up=curNode->parent;
    treeNode* right=curNode->nextSibling;
    //Either needs to go right first or go up the level
    while(right!=NULL && right->leaf!=0)
    {
        right= right->nextSibling;
    }
    //Two terminating conditions
    //Case 1
    if(!right)
    {
        //No next Termnial Sibling exists
        // Need to go up by one levelkeyword,root->
        if(up)
            return locateNextNonTerminal(up);
        else return NULL;//Should Not Execute
    }
    else if(right->leaf==0)
        return right;
}

void printTree(treeNode* root, FILE* fp)
{
    // static int i =1;
    // printf("%d izz\n",i++);
    //Base Case
    if(root==NULL)
        return;
    //Call to first child
    if(root->firstChild!=NULL)
        printTree(root->firstChild,fp);


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
    treeNode* temp = root->firstChild;
    if(temp!= NULL)//Error Check if root is a leaf node
    {
        temp=temp->nextSibling;
        while(temp!=NULL)
        {
            printTree(temp,fp);
            temp=temp->nextSibling;
        }
    }

}

void printParseTree(treeNode* root,char* outfile)
{
    FILE* fp = fopen(outfile,"w");
    if(!fp)
        {
            printf("Error Writing Parse Tree\nExiting....\n");
            return ;
        }
    fprintf(fp,"%-32s | %-32s | %-32s | %-32s | %-32s | %-15s | %-32s\n\n","Lexeme","LineNumber","Token Name","Numerical Value","Parent Node Symbol","Leaf?","Node Symbol");
    printTree(root,fp);
    fclose(fp);
}
