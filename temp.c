astNode* createAbstractSyntaxTree(astNode *ast_root, treeNode *p_root)
{
    else
    {
        treeNode *p_child_itr = p_root->firstChild;
        astNode* astChild = (astNode*) malloc(sizeof(astNode));
        ast_root->firstChild = astChild;
        int firstIteration = 1;//Use to not execute some code of the while loop in some iteration
        astNode* ast_child_itr = ast_root;

        while(p_child_itr != NULL)
        {
            if(isUseless(p_child_itr))
                continue;
            if(!firstIteration)
                astChild=(astNode *)malloc(sizeof(astNode));
            astChild->keyword = p_child_itr->keyword;
            astChild->tk = p_child_itr->tk;
            astChild->parent = ast_root;
            astChild->nextSibling = NULL;
            astChild->firstChild = NULL;
            if(p_child_itr->leaf == 0)
                createAbstractSyntaxTree(astChild, p_child_itr);
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
    }//End of else statement

}//End of the function
