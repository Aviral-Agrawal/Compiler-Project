/*
* Compiler Construction Project
* Group Number 40
* AVIRAL AGRAWAL 2016A7PS0077P
* JITVAN SHUKLA  2016A7PS0083P
* SHUBHAM SHARMA 2016A7PS0115P
* AKSHIT AGARWAL  2016A7PS0122P

*/
#ifndef __SYN_H__
#define __SYN_H__
#include "syn.h"
#endif

//List (global) initialisation
int getSetIndex(dict *dt, char *keyword)
{
  term *cur1=dt->tm;
  nonTerm *cur2=dt->nm;
  term *temp1;
  nonTerm *temp2;
  if(isTerminal(keyword))
  {
    if(dt->tCount==0)
    {
      strcpy(cur1->keyword,keyword);
      dt->tCount++;
      return 0;
    }
  while(cur1!=NULL){
    if(strcmp(cur1->keyword,keyword)==0)
    {
      return cur1->index;
    }
    if(cur1->next==NULL)
      temp1=cur1;
    cur1=cur1->next;
  }

  term *tm= (term *)malloc(sizeof(term));
  strcpy(tm->keyword,keyword);
  tm->next=NULL;
  tm->index=temp1->index+1;
  temp1->next=tm;
  dt->tCount++;
  return tm->index;
  }
  else
  {
    if(dt->ntCount==0)
    {
      strcpy(cur2->keyword,keyword);
      dt->ntCount++;
      return 0;
    }
    while(cur2!=NULL){
      if(strcmp(cur2->keyword,keyword)==0)
      {
        return cur2->index;
      }
      if(cur2->next==NULL)
        temp2=cur2;
      cur2=cur2->next;
    }

    nonTerm *nm= (nonTerm *)malloc(sizeof(nonTerm));
    strcpy(nm->keyword,keyword);
    nm->next=NULL;
    nm->index=temp2->index+1;
    temp2->next=nm;
    dt->ntCount++;
    return nm->index;
  }
}
list* outstandingfollow;

int isEpsilon(char* keyword)
{
    if(strcmp(keyword,"eps"))
        return 0;
    return 1;
}

int isTerminal(char* keyword)
{
    char ptr[4];
    if(strlen(keyword)<=3)
        return 0;
    else
    {
        int i=0;
        for(;i<3;i++)
            ptr[i]=keyword[i];
        ptr[3]='\0';
        if(strcmp(ptr,"TK_")==0)
            return 1;
    }
    return 0;
}

ft* getFirst(grammar *gmr, char* keyword)
{
    if(isTerminal(keyword)){
        ft *temp1=(ft *)malloc(sizeof(ft));
        strcpy(temp1->terminal,keyword);
        temp1->next=NULL;
        return temp1;
    }

    int i=0;
    for(;i<gmr->ftsa->count;i++){
        if(strcmp(gmr->ftsa->firstSet[i].terminal,keyword)==0)
        {
            return gmr->ftsa->firstSet[i].next;
        }
    }
    strcpy(gmr->ftsa->firstSet[gmr->ftsa->count].terminal,keyword);
    ft *temp=(ft *)malloc(sizeof(ft));
    temp->next=NULL;
    temp->terminal[0] = '\0';
    gmr->ftsa->firstSet[gmr->ftsa->count].next=temp;
    gmr->ftsa->count++;
    return temp;
}

void insertFirst(grammar *gmr, char *keyword)
{
    //If the first set of the non-terminal is
    //being called for the first time
    //printf("%s\n",keyword);
    if(gmr->ftsa==NULL || gmr->ftsa->firstSet==NULL)
    {
        gmr->ftsa=(firstSetArray *)malloc(sizeof(firstSetArray));
        gmr->ftsa->firstSet=(ft *)malloc(sizeof(ft)*MAX_FIRST_SIZE);
        int i;
        for(i=0;i<MAX_FIRST_SIZE;i++){
            //gmr->ftsa->firstSet[i].terminal=NULL;
            gmr->ftsa->firstSet[i].next=NULL;
        }
        gmr->ftsa->size=MAX_FIRST_SIZE;
        gmr->ftsa->count=0;

    }
    int prodCount=gmr->prodCount;
    int i=0;
    for(;i<prodCount;i++)
    {
        if(strcmp(gmr->nonTerm[i].nonTermName,keyword)==0)
        {

            insertFirstHelper(gmr,gmr->nonTerm[i].next,keyword,1);
            // flag is 1 only in the first instance of recursion when we should add eps also
            // if(containsEpsilon(gmr,gmr->nonTerm[i].next))
            //     insertFirstHelper(gmr,gmr->nonTerm[i].next->next,keyword,0);

        }

    }

}

void insertFirstHelper(grammar *gmr, ruleHead *rh, char *keyword, int flag){
    ft *start1=getFirst(gmr,keyword);
    ft *start=start1;
    //if(RHS of the rule in which finding First begins with a terminal)
    //In this case there would be no need to recurse down to get the first
    if(isTerminal(rh->nonTermName) || (isEpsilon(rh->nonTermName) && flag))
    {
        ft *newElement=(ft *)malloc(sizeof(ft));
        strcpy(newElement->terminal,rh->nonTermName);
        newElement->next=NULL;
        //Checking if the list is empty for firstSet
        if(start->terminal[0]=='\0')
            strcpy(start->terminal,rh->nonTermName);
        else
        {
          //Flag to check presence of this element in the First
            int check = 0;
            while(start->next!=NULL)
            {
              if(strcmp(start->terminal,newElement->terminal)==0)
                  check=1;
                start=start->next;
            }
            if(strcmp(start->terminal,newElement->terminal)==0)
                check=1;
            if(!check)
              start->next=newElement;
        }
    }

    //if(RHS of the rule in which finding First begins with a NTs)
    //Might require recusrion for finding the first set of other NTs
    else
    {
        ft *cur=getFirst(gmr,rh->nonTermName);
        //If (the first set of the NT is not yet calculated), then,
        //then recurse to find it
        if(cur->terminal[0] == '\0')
            insertFirst(gmr, rh->nonTermName);
        while(cur!=NULL)
        {
          start1=getFirst(gmr,keyword);
          if(isEpsilon(cur->terminal))
          {
            if(rh->next!=NULL)
              insertFirstHelper(gmr,rh->next,keyword,1);
            else
            {
                ft *newElement1=(ft *)malloc(sizeof(ft));
                strcpy(newElement1->terminal,cur->terminal);
                newElement1->next=NULL;

                int check = 0;
                if(start1->terminal[0]=='\0')
                {
                    strcpy(start1->terminal,cur->terminal);
                }
                else
                {
                    while(start1->next!=NULL)
                    {
                        if(strcmp(start1->terminal,newElement1->terminal)==0)
                            check=1;
                        start1=start1->next;
                    }
                    if(strcmp(start1->terminal,newElement1->terminal)==0)
                        check=1;
                    if(!check)
                        start1->next=newElement1;
                }
            }
            return;
          }
          ft *newElement1=(ft *)malloc(sizeof(ft));
          strcpy(newElement1->terminal,cur->terminal);
          newElement1->next=NULL;

          int check = 0;
          if(start1->terminal[0]=='\0')
          {
              strcpy(start1->terminal,cur->terminal);
          }
          else
          {
              while(start1->next!=NULL)
              {
                if(strcmp(start1->terminal,newElement1->terminal)==0)
                    check=1;
                start1=start1->next;
                }
                if(strcmp(start1->terminal,newElement1->terminal)==0)
                    check=1;
              if(!check)
                start1->next=newElement1;
          }
          cur=cur->next;
      }//End of the while loop
    }//End of the else stmt
}

fl* getFollow(grammar *gmr, char* keyword)
{
    int n = gmr->flsa->count, i=0;
    if(gmr==NULL || gmr->flsa==NULL)
        return NULL;
    for(i=0;i<n;i++)
    {
        //Checking if follow exists for this nonTerminal
        if(strcmp(gmr->flsa->followSet[i].terminal,keyword)==0)
            {
                fl* cur = gmr->flsa->followSet[i].next;
                //Return pointer to the first element in the follow set
                // of a non-terminal
                return cur;
            }
    }
    strcpy(gmr->flsa->followSet[n].terminal,keyword);
    gmr->flsa->count++;
    fl* temp = (fl*) malloc(sizeof(fl));
    temp->next=NULL;
    temp->terminal[0] = '\0';
    gmr->flsa->followSet[i].next = temp;
    return temp;
}

void insertFollow(grammar *gmr, char* keyword)
{
    //Memory Allocation (if not already allocated)
    if(gmr->flsa==NULL || gmr->flsa->followSet==NULL)//Also puts dollar in the follow of the start symbol
    {
        gmr->flsa = (followSetArray*) malloc(sizeof(followSetArray));
        gmr->flsa->followSet = (fl*) malloc(sizeof(fl)*MAX_FOLLOW_SIZE);
        gmr->flsa->size = MAX_FOLLOW_SIZE;
        gmr->flsa->count=0;
        int i;
        //Memory Init
        for(i=0;i<MAX_FOLLOW_SIZE;i++)
        {
            gmr->flsa->followSet[i].terminal[0] = '\0';
            gmr->flsa->followSet[i].next = NULL;
            if(i == 0)//i.e., the keyword is the start symbol
            {
                strcpy(gmr->flsa->followSet[0].terminal, keyword);
                fl* newNode = (fl*) malloc(sizeof(fl));
                strcpy(newNode->terminal, "TK_DOLLAR");
                newNode->next = NULL;
                gmr->flsa->followSet[0].next = newNode;
                gmr->flsa->count++;
            }
        }
    }

    int prodCount = gmr->prodCount;
    int i=0;
    for(i=0;i<prodCount;i++)
    {
        ruleHead* curRule = &(gmr->nonTerm[i]);
        ruleHead *temp = curRule->next;
        while(temp != NULL)
        {
            if(strcmp(temp->nonTermName,keyword)==0 )
                insertFollowHelper(gmr,temp->next,keyword, curRule->nonTermName);
            temp = temp->next;
        }//EOLoop
    }//End of for loop
}// end Of function definition

void formNewList()
{
   outstandingfollow= (list*) malloc(sizeof(list));
   outstandingfollow->first[0] = '\0';
   outstandingfollow->second[0] = '\0';
   outstandingfollow->next = NULL;
}

int strInList(char* str1, char* str2)
{
    // if(the list is empty,
    //     hence no outstanding follows)
    if(outstandingfollow->first == NULL || outstandingfollow->second == NULL)
        return 1;
    list* temp = outstandingfollow;
    while(temp != NULL)
    {
        if(strcmp(temp->first, str1) == 0 && strcmp(temp->second, str2) == 0)
            return 0;
        temp = temp->next;
    }
    return 1;
}

void deleteFromList(char* str1, char* str2)
{
    list* temp = outstandingfollow;
    //The list is empty, hence nothing to delete
    if(temp->first[0] =='\0' && temp->second[0] == '\0')
        return;
    //The list has only one node
    if(strcmp(temp->first, str1) == 0 && strcmp(temp->second, str2) == 0 && temp->next == NULL)
    {
        strcmp(temp->first, "\0");
        strcmp(temp->second, "\0");
        return;
    }
    //If the list has only a single node, dependency does not match, return
    else if((strcmp(temp->first, str1) != 0 || strcmp(temp->second, str2) != 0) && temp->next == NULL)
    {
        return;
    }
    //List has more than one nodes
    //The first node matches the strings
    if(strcmp(temp->first, str1) == 0 && strcmp(temp->second, str2) == 0 && temp->next != NULL)
    {
        outstandingfollow = temp->next;
        free(temp);
        return;
    }
    //if(The first node does not match the strings)
    list* prev = temp;
    temp = temp->next;
    //Iterating through the list
    while(temp != NULL)
    {
        if(strcmp(temp->first, str1) == 0 && strcmp(temp->second, str2) == 0)
        {
            if(temp->next != NULL)
                prev->next = temp->next;
            else
                prev->next = NULL;
            free(temp);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}//End of the function

void insertInList(char* str1, char* str2)
{
    if(outstandingfollow->first[0] == '\0' && outstandingfollow->second[0] == '\0')
    {
        strcpy(outstandingfollow->first, str1);
        strcpy(outstandingfollow->second, str2);
        return;
    }
    list* newNode = (list*) malloc(sizeof(list));
    strcpy(newNode->first, str1);
    strcpy(newNode->second, str2);
    newNode->next = NULL;
    list* temp = outstandingfollow;
    //Traversing to the end of the list
    while(temp != NULL && temp->next != NULL)
        temp = temp->next;
    temp->next = newNode;
}//End of the function

//checks in the entire follow set
int mergeHelperIsPresent(fl* ptr1, fl* flSet)
{
    while(flSet != NULL)
    {
        if(strcmp(ptr1->terminal, flSet->terminal) == 0)
            return 1;
        flSet = flSet->next;
    }
    return 0;
}

void insertInFollow(fl* itr1, fl* flSet)
{
    while(flSet->next != NULL)
        flSet = flSet->next;
    fl* newNode = (fl*) malloc(sizeof(fl));
    strcpy(newNode->terminal, itr1->terminal);
    newNode->next = NULL;
    flSet->next = newNode;
}

void mergeFollow(grammar* gmr, char* curr, char* toBeIncluded)
{
    fl* temp1 = getFollow(gmr, toBeIncluded);
    fl *temp2 = getFollow(gmr,curr);
    fl* itr1 = temp1;
    fl* itr2 = temp2;
    //Inducting into the follow of curr(temp2)
    while(itr1 != NULL)
    {
        if(!mergeHelperIsPresent(itr1, temp2))
            insertInFollow(itr1, temp2);
        itr1 = itr1->next;
    }
    //Inducting into the follow of toBeIncluded(temp1)
    while(itr2 != NULL)
    {
        if(!mergeHelperIsPresent(itr2, temp1))
            insertInFollow(itr2, temp1);
        itr2 = itr2->next;
    }
}

void insertFollowHelper(grammar *gmr, ruleHead *rh, char*keyword, char* ntHead)
{
    // On the right we may have a non-terminal
    // 3 cases need to be implememnted

    //First case: rh -> terminal
    //No need for recusion
    if(rh != NULL && isTerminal(rh->nonTermName))
    {
        ft *curFirst = getFirst(gmr,rh->nonTermName);
        //Locating the Correct Follow Set of a nonTerminal
        fl *curFollow = getFollow(gmr,keyword);
        fl* curFollowOrig = curFollow;
        fl* itr = curFollow;
        //If the follow set of the keyword is
        //being filled for the first time
        if(curFollow->terminal[0] == '\0')
        {
            strcpy(curFollow->terminal, curFirst->terminal);
            curFirst = curFirst->next;
        }
        //Travel to the last node to append list
        while(curFollow->next!=NULL)
            curFollow = curFollow->next;

        while(curFirst!=NULL)
        {
            //Checking for duplicates
            itr = curFollowOrig;
            int flag = 0;
            while(itr != NULL)
            {
                if(strcmp(itr->terminal, curFirst->terminal) == 0)
                {
                    flag = 1;
                    break;
                }
                itr = itr->next;
            }
            if(flag == 1)
            {
                curFirst=curFirst->next;
                continue;
            }
            //Allocating a new node
            fl* node = (fl*) malloc(sizeof(fl));
            strcpy(node->terminal,curFirst->terminal);
            node->next=NULL;

            curFollow->next = node;
            curFollow= curFollow->next;
            curFirst=curFirst->next;
        }
    }// End of First Test Case

    //Second case: rh -> Non-terminal (NOTE: epsilon handeled)
    //No need for recusion
    else if(rh != NULL && !isTerminal(rh->nonTermName))
    {
        ft *curFirst = getFirst(gmr,rh->nonTermName);
        fl *curFollow = getFollow(gmr,keyword);
        fl* curFollowOrig = curFollow;
        fl* itr = curFollow;
        //If the follow set of the keyword is
        //being filled for the first time
        if(curFollow->terminal[0] == '\0')
        {
            if(strcmp(curFirst->terminal, "eps") != 0)
            {
                strcpy(curFollow->terminal, curFirst->terminal);
                curFirst = curFirst->next;
            }
            else//epsilon handeled
            {
                insertFollowHelper(gmr, rh->next, keyword, ntHead);
                curFirst = curFirst->next;
            }
        }
        //Going to the end of the follow set
        while(curFollow->next!=NULL)
            curFollow = curFollow->next;
        while(curFirst != NULL)//Handle epsilon in the first set
        {
            if(strcmp(curFirst->terminal, "eps") != 0)
            {
                //Checking for duplicates
                itr = curFollowOrig;
                int flag = 0;
                while(itr != NULL)
                {
                    if(strcmp(itr->terminal, curFirst->terminal) == 0)
                    {
                        flag = 1;
                        break;
                    }
                    itr = itr->next;
                }
                if(flag == 1)
                {
                    curFirst=curFirst->next;
                    continue;
                }
                fl* node = (fl*) malloc(sizeof(fl));
                strcpy(node->terminal,curFirst->terminal);
                node->next=NULL;

                curFollow->next = node;
                curFollow= curFollow->next;
                curFirst=curFirst->next;
            }
            else//epsilon handeled
            {
                insertFollowHelper(gmr, rh->next, keyword, ntHead);
                curFirst = curFirst->next;
            }
        }
    }//End of the second case

    //Third case: rh -> NULL
    else
    {
        char* curr = keyword;
        char* toBeIncluded = ntHead;
        //if(Follow(keyword) = Follow(keyword))
        if(strcmp(curr, toBeIncluded) == 0)
            return;
        //if(circular dependency doesn't exists)
        if(strInList(toBeIncluded, curr) != 0)
        {
            fl* temp = getFollow(gmr, ntHead);
            //If the follow of the toBeIncluded has been calculated
            //If(follow(ntHead) has been calculated
            if(strcmp(temp->terminal, "\0") != 0)
            {
                //Handle whether the follow(ntHead) has been calculated fully
                fl *curFollow = getFollow(gmr,keyword);
                fl* curFollowOrig = curFollow;
                fl* itr = curFollow;
                //If the follow set of the keyword is
                //being filled for the first time
                if(curFollow->terminal[0] == '\0')
                {
                    strcpy(curFollow->terminal, temp->terminal);
                    temp = temp->next;
                }
                while(curFollow->next!=NULL)//Traversing to the end
                    curFollow = curFollow->next;
                //Adding the elements of the follow(ntHead) to follow(keyword)
                while(temp != NULL)
                {
                    //Checking for duplicates
                    itr = curFollowOrig;
                    int flag = 0;
                    while(itr != NULL)
                    {
                        if(strcmp(itr->terminal, temp->terminal) == 0)
                        {
                            flag = 1;
                            break;
                        }
                        itr = itr->next;
                    }
                    if(flag == 1)
                    {
                        temp = temp->next;
                        continue;
                    }
                    fl* node = (fl*) malloc(sizeof(fl));
                    strcpy(node->terminal,temp->terminal);
                    node->next=NULL;
                    curFollow->next = node;
                    curFollow = node;
                    temp = temp->next;
                }
            }
            //If(follow(toBeIncluded) has not been calculated yet)
            //Insert an outstanding follow request
            //i.e. the follow set of toBeIncluded is empty
            //Circular dependency created
            else
                insertInList(curr, toBeIncluded);
        }
        //else(circular dependency exists)
        else
        {
            deleteFromList(toBeIncluded, curr);
            //Handle whether the follow(ntHead) has been calculated fully
            mergeFollow(gmr, curr, toBeIncluded);
        }
    }//End of the third case
}//End of the function

//Rule Inserter
void insert1(ruleHead *rh, char *keyword){
    ruleHead *new=(ruleHead *)malloc(sizeof(ruleHead));
    strcpy(new->nonTermName,keyword);
    ruleHead *cur=rh;

    if(cur->nonTermName[0]=='\0'){
      strcpy(rh->nonTermName,keyword);
      return;
    }

    while(cur->next!=NULL)
    {
        cur=cur->next;
    }
    cur->next=new;
}

void printFirst(grammar *gmr, char* keyword)
{
  ft *cur = getFirst(gmr,keyword);
  while(cur!=NULL)
  {
      printf("%s\n",cur->terminal);
      cur=cur->next;
  }
  printf("%s is Done!!\n",keyword);
}

void printFollow(grammar* gmr, char* keyword)
{
    fl* ptr = getFollow(gmr, keyword);
    fl* cur = ptr;
    printf("\n");
    while(cur != NULL)
    {
        printf("%s\n", cur->terminal);
        cur = cur->next;
    }
    printf("%s is Done!!\n", keyword);
}

//Calcutes the first set in an automated way
void calculateFirstSet(grammar* gmr)
{
    int i = 0, below = gmr->prodCount-1;
    for(i=below;i>=0;i--){
        insertFirst(gmr, gmr->nonTerm[i].nonTermName);
    }
}

void calculateFollowSet(grammar* gmr)
{
    formNewList();
    int i=0;
    for(i=0;i<gmr->prodCount;i++)
        insertFollow(gmr,gmr->nonTerm[i].nonTermName);
}


// int main()
// {
//     grammar *gmr=(grammar *)malloc(sizeof(grammar));
//     gmr->ftsa = NULL;
//     gmr->flsa = NULL;
//     gmr->nonTerm=(ruleHead *)malloc(200*sizeof(ruleHead));
//     gmr->prodCount=10;
//     insert1(&gmr->nonTerm[0],"S");
//     insert1(&gmr->nonTerm[0],"A");
//     insert1(&gmr->nonTerm[0],"B");
//     insert1(&gmr->nonTerm[0],"TK_A");
//     insert1(&gmr->nonTerm[1],"S");
//     insert1(&gmr->nonTerm[1],"TK_B");
//     insert1(&gmr->nonTerm[1],"C");
//     insert1(&gmr->nonTerm[1],"A");
//     insert1(&gmr->nonTerm[2],"A");
//     insert1(&gmr->nonTerm[2],"TK_C");
//     insert1(&gmr->nonTerm[2],"B");
//     insert1(&gmr->nonTerm[2],"C");
//     insert1(&gmr->nonTerm[2],"D");
//     insert1(&gmr->nonTerm[3],"A");
//     insert1(&gmr->nonTerm[3],"eps");
//
//     insert1(&gmr->nonTerm[4],"B");
//     insert1(&gmr->nonTerm[4],"C");
//     insert1(&gmr->nonTerm[4],"TK_D");
//     insert1(&gmr->nonTerm[4],"A");
//     insert1(&gmr->nonTerm[5],"B");
//     insert1(&gmr->nonTerm[5],"TK_A");
//     insert1(&gmr->nonTerm[5],"TK_D");
//
//     insert1(&gmr->nonTerm[6],"C");
//     insert1(&gmr->nonTerm[6],"TK_E");
//     insert1(&gmr->nonTerm[6],"C");
//     insert1(&gmr->nonTerm[7],"C");
//     insert1(&gmr->nonTerm[7],"eps");
//
//     insert1(&gmr->nonTerm[8],"D");
//     insert1(&gmr->nonTerm[8],"TK_B");
//     insert1(&gmr->nonTerm[8],"TK_S");
//     insert1(&gmr->nonTerm[8],"TK_F");
//     insert1(&gmr->nonTerm[9],"D");
//     insert1(&gmr->nonTerm[9],"TK_A");
//
//     printf("Testing automation of the first set calculation\n");
//
//     // insertFirst(gmr,"D");
//     // insertFirst(gmr,"C");
//     // insertFirst(gmr,"B");
//     // insertFirst(gmr,"A");
//     // insertFirst(gmr,"S");
//     calculateFirstSet(gmr);
//
//     printFirst(gmr,"S");
//     printFirst(gmr,"A");
//     printFirst(gmr,"B");
//     printFirst(gmr,"C");
//     printFirst(gmr,"D");
//
//     printf("First set have been succesfully created!!\n");
//     formNewList();
//     calculateFollowSet(gmr);
//     // insertFollow(gmr,"S");
//     // insertFollow(gmr,"A");
//     // insertFollow(gmr,"B");
//     // insertFollow(gmr,"C");
//     // insertFollow(gmr,"D");
//
//     printFollow(gmr,"S");
//     printFollow(gmr,"A");
//     printFollow(gmr,"B");
//     printFollow(gmr,"C");
//     printFollow(gmr,"D");
//
//     /*Grammar used to verify
//
//     S→ABa / bCA
//
//     A→cBCD / є
//
//     B→CdA / ad
//
//     C→eC / є
//
//     D→bsf / a
//
//     */
// }
