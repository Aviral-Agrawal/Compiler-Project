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

#ifndef __N_ARY_H__
#define __N_ARY_H__
#include "nAry.h"
#endif

#ifndef __GMR_CALL_H__
#define __GMR_CALL_H__
#include "gmrCall.h"
#endif

// void removeComments(char *testcaseFile)
// {
// 	FILE* fp = fopen(testcaseFile, "r");
// 	if(fp==NULL)
// 	{
// 		printf("testcaseFile entered not open.\n");
// 		return;
// 	}
//     FILE* fp2 = fopen("cleanCode.txt", "w");
// 	if(fp2==NULL)
// 	{
// 		printf("cleanFile could not be opened.\n");
// 		return;
// 	}
//
// 	BUFF = getStream(fp);
// 	while(BUFF[0] != '\0')
// 	{
// 		int bufpos = 0;
// 		int bufLen = strlen(BUFF);
// 		//Iterate over the currently filled buffer
// 		while(bufpos < bufLen)
// 		{
// 			//if(comment found) then, skip the remaining portion of the line
// 			if(BUFF[bufpos] == '%')
// 			{
// 				//if(comment is on a new line), then,
// 				//simply omit it including it's '\n'
// 				//bufpos == 0 means that the first line of the code is a comment
// 				//(BUFF[bufpos-1] == '\n') checks if the comment is only thing on this line
// 				if(bufpos == 0 || BUFF[bufpos-1] == '\n')
// 				{
// 					while(BUFF[bufpos] != '\n')
// 						bufpos++;
// 					while(BUFF[bufpos] == '\n' || BUFF[bufpos] == '\r')
// 						bufpos++;
// 				}
// 				//if(comment has some code before it on the same line), then,
// 				//we want to print the '\n' at the end
// 				else
// 				{
// 					while(BUFF[bufpos] != '\n')
// 						bufpos++;
// 					while(BUFF[bufpos] == '\n' || BUFF[bufpos] == '\r')
// 						bufpos++;
// 					bufpos--;//to be able to print the '\n' of this line
// 				}
// 			}
// 			//if(valid code found) then, print it
// 			else
// 				fprintf(fp2,"%c", BUFF[bufpos++]);
// 		}//End of the while loop
// 		free(BUFF);
// 		BUFF = getStream(fp);
// 	}//End of the while loop
// 	fclose(fp);
// 	fclose(fp2);
// 	char cwd[400];
// 	if (getcwd(cwd, sizeof(cwd)) != NULL) {
// 	    printf("\nFile Written in : %s/cleanCode.txt\n\n", cwd);
// 	} else {
//      perror("getcwd() error");
//      return;
//  	}
// }//End of the function


void pop_grammar(grammar* r)
{


int i=0;
ruleHead* temp;
int c=0;
while(i<(r->prodCount))
{
	temp=(r->nonTerm[i]).next;
	c=0;
	printf("%s ---> ",(r->nonTerm[i]).nonTermName);
	//Print each production by iterating over it
	while(c<r->tokenCount[i]-2)
	{
		printf("%s ---> ",temp->nonTermName);
		temp=(temp->next);
		c++;

	}
	printf("%s\n",temp->nonTermName);

i++;

}

}

grammar* file_open_one_prod(char file_name[32])
{
	int initial_size=INIT_GMR_LEN;
    grammar *r;
	r=(grammar*)malloc(sizeof(grammar));
	int i;
	r->nonTerm=(ruleHead*)malloc(sizeof(ruleHead)*initial_size);
	r->tokenCount=(int*)malloc(sizeof(int)*initial_size);
	r->ftsa=NULL;
	r->flsa=NULL;
	char* production;
	production=(char*)malloc(512);

        FILE* fp=fopen(file_name,"r");
	 int c=0;
	char buf;
	char *cat="  ";//double space to count for eps as well
     while(fscanf(fp,"%[^\n]",production)!=EOF)// && c<no_rules)
     {

	strcat(production,cat);
	//printf("%s",production);//whitespace added since used in furthur functions
	extrac(production,r,c);
        //printf("%s %ld \n",production,strlen(production));
	fscanf(fp,"%c",&buf);
	c++;
	//c++;


     }
	int no_rules;
	printf("the number of rules are %d \n",c);
	printf("the value of size variable that is the maximum number of productions it can hold at this point of time is %d \n",initial_size);
	no_rules=c;
	r->prodCount=no_rules;
	//  calculateFirstSet(r);
	//  calculateFollowSet(r);
	//     // printFirst(r,"<moreFields>");
	//     // printFirst(r,"<term>");
	//     // printFirst(r,"<relationalOp>");
	// /*printFollow(r,"<moreFields>");
	//     printFollow(r,"<term>");
	//     printFollow(r,"<relationalOp>");*/
	// 	//removeComments("testcase4.txt");
	// 	FILE *fp1=fopen("testcase1.txt","r");
	// 	FILE *fp2=fopen("fp2.txt","w");
	// 	FILE *fp3=fopen("fp3.txt","w");
	// 	if(!fp1 || !fp2 || !fp3)
	// 		{printf("Error openeing in File\n");
	// 	exit(-1);}
	//
	// 	treeNode* tree = parse(fp1,r,fp2);
	//     printTree(tree,fp3);
	//     printf("\n");
	// 	fclose(fp1);
	//pop_grammar(r);
        fclose(fp);
		return r;

}

//inserting tokens of one rule into a linked list
ruleHead* make_livl(char* token,ruleHead* head,int c)
{
		head->next=(ruleHead*)malloc(sizeof(ruleHead));
		head=head->next;
		strcpy(head->nonTermName,token);
		head->next=NULL;
		return head;


}

//extracting tokens from one rule
void extrac(char* rule,grammar* r,int cur_rule)
{
	char *token;
	int i;
	char ch[512];
	for(i=0;i<512;i++)
		ch[i]='\0';
	int tokenCount=0;
	int counter_ang_brac=0;
	int c=0;
	int j;

	ruleHead head=r->nonTerm[cur_rule];
	token=(char*)malloc(512);
	ruleHead* n;
	for(j=0;j<512;j++)
	{
		token[i]='\0';

	}

	for(i=0;i<strlen(rule)-2;i++)
	{

		if(rule[i]=='<'|| (rule[i]=='T' && rule[i+1]=='K') || ( rule[i]=='e' && rule[i+1]=='p' && rule[i+2]=='s'))//starting the counter for token
			{

				counter_ang_brac=1;

				for(j=0;j<512;j++)
					{
						token[j]='\0';

					}

			}

		if(counter_ang_brac==1)//adding characters to token
			{
				token[c++]=rule[i];
				//printf("%s\n",token);

			}
		if((rule[i]=='>' && rule[i-1]!='='  ) || (rule[i]>='A' && rule[i]<='Z' && rule[i+1]==' ') ||( rule[i]=='s' && rule[i+1]==' '))//ending the counter for token
			{

				counter_ang_brac=0;
				tokenCount++;
				//printf("%s %ld \n",token,c); //can also use to find the length of token
				if(tokenCount==1)
					{

						n=&(r->nonTerm[cur_rule]);
						strcpy(n->nonTermName,token);
						//printf("%s",head.nonTermName);
						n->next=NULL;//(ruleHead*)malloc(sizeof(ruleHead));;

					}
				else
					n=make_livl(token,n,c);//c is passed to determine the length of token

				c=0;

				for(j=0;j<512;j++)
					{
						token[j]='\0';

					}

			}


	}
	r->tokenCount[cur_rule]=tokenCount;

}

//handling bars in between

void removeBar(char* file_name)
{

	 FILE* fp=fopen(file_name,"r");//for reading
        //fseek(0);
	char* production;
	int i;
	production=(char*)malloc(512);
	char* nonTerm;
	char buf;
	nonTerm=(char*)malloc(512);

	FILE* fp1=fopen("new_grammar.txt","w");
	 while(fscanf(fp,"%[^\n]",production)!=EOF)
	     {

		for(i=0;i<strlen(production);i++)//capturing the starting non terminal of the rule
			{
				nonTerm[i]=production[i];
				if(production[i]=='>')
					break;
			}
		for(i=0;i<strlen(production);i++)
			{
				if(production[i]!='|')
					fprintf(fp1,"%c",production[i]);
				else
				{
					fprintf(fp1,"\n%s==>",nonTerm);

				}
			}

		for(i=0;i<512;i++)
		{
		nonTerm[i]='\0';

		}
		fscanf(fp,"%c",&buf);
		fprintf(fp1,"\n");

	     }

		fclose(fp);
		fclose(fp1);


}

// int main()
// {
// 	removeBar("old_grammar.txt");
// 	file_open_one_prod("new_grammar.txt");
// 	printf("END of Main function");
// 	return 0;
// }
