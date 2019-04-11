#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__
#include "symbolTable.h"
#endif

// NASM for read
const char *readDirective = "\
_read proc\n\
    push si\n\
    mov dx, offset inputbuf\n\
    mov bx, dx\n\
    mov byte ptr[bx], 10\n\
    mov ah, 0ah\n\
    int 21h\n\
    \n\
    mov ah, 02h\n\
    mov dl, 10\n\
    int 21h\n\
    \n\
    mov bx, offset inputbuf + 1\n\
    mov cx, 0\n\
    mov cl, [bx]\n\
    mov ax, 0\n\
    \n\
    mov si, 0\n\
    \n\
    inc bx\n\
    mov dl, [bx]\n\
    cmp dl, 45\n\
    jne _startgetting\n\
    	mov si, 1\n\
    	dec cx\n\
    	jmp _getdigitfrombuf\n\
    _startgetting:\n\
    	dec bx\n\
    _getdigitfrombuf:\n\
        inc bx\n\
        mov dx, 10\n\
        mul dx\n\
        mov dh, 0\n\
        mov dl, [bx]\n\
        sub dx, 48\n\
        add ax, dx\n\
        \n\
        loop _getdigitfrombuf\n\
    \n\
    cmp si, 1\n\
    jne _readend\n\
    	not ax\n\
    	add ax, 1\n\
    \n\
    _readend:\n\
        pop si\n\
        ret\n\
_read endp";

// NASM for write
const char *writeDirective = "\
_write proc\n\
    mov bx, ax\n\
    and bx, 8000h\n\
    cmp bx, 0\n\
    je _writesignhandled\n\
    not ax\n\
    add ax, 1\n\
    mov bx, ax\n\
    mov ah, 02h\n\
    mov dl, 45\n\
    int 21h\n\
    mov ax, bx\n\
    \n\
    _writesignhandled:\n\
    mov cx, 0\n\
    mov bx, 10\n\
    mov dx, 0\n\
    _getdigitfromnum:\n\
        div bx\n\
        \n\
        push dx\n\
        inc cx\n\
        \n\
        mov dx, 0\n\
        \n\
        cmp ax, 0\n\
        jne _getdigitfromnum\n\
        \n\
        mov ah, 02h\n\
    \n\
    _writedigit:\n\
        pop dx\n\
        add dl, 48\n\
        int 21h\n\
        \n\
        dec cx\n\
        cmp cx, 0\n\
        jne _writedigit\n\
    ret\n\
_write endp";

int ifNo = 0;
char ifNoBuf[11];
int loopNo = 0;
char loopNoBuf[11];

extern recTable *rec;

void codeGen(astNode *ast_root, symbolTable *st, FILE *output)
{
    if(ast_root==NULL)
      return;
    if(!isTerminal(ast_root->keyword))
    {
          if(strcmp(ast_root->keyword,"<program>")==0)
          {
                fprintf(output, ".model small\n.stack\n\n.code\n\n%s\n\n%s\n\n", readDirective, writeDirective);
                codeGen(ast_root->firstChild->nextSibling,st,output);
                fprintf(output, ".data\n");
                fprintf(output, "inputbuf db 12 dup(0)\n");
                codeGen(ast_root->firstChild->nextSibling->firstChild->nextSibling->firstChild->nextSibling,st,output);
                fprintf(output, "\nend main");
          }

          else if(strcmp(ast_root->keyword,"<mainFunction>")==0)
          {
                fprintf(output, "main proc\n\nmov ax, seg inputbuf\nmov ds, ax\n\n");
                codeGen(ast_root->firstChild->nextSibling,st,output);

          }

          else if(strcmp(ast_root->keyword,"<declarations>")==0)
          {

                codeGen(ast_root->firstChild,st,output);
                codeGen(ast_root->firstChild->nextSibling,st,output);

          }

          else if(strcmp(ast_root->keyword,"<declaration>")==0)
          {
                if(strcmp(ast_root->firstChild->nextSibling->firstChild->keyword,"<primitiveDatatype>")==0)
                {
                    codeGen(ast_root->firstChild->nextSibling->nextSibling,st,output);
                    fprintf(output, ": dw 0\n");
                }
                else
                {

                    recTable *rec = findRecord(st,ast_root->firstChild->nextSibling->firstChild->firstChild->nextSibling->tk.lexeme);
                    idfTable *fields = rec->fields;
                    while(fields!=NULL)
                    {

                        fprintf(output, "_%s dw 0\n", fields->keyword);
                    }
                }
            }

          else if(strcmp(ast_root->keyword,"<stmts>")==0)
          {
                codeGen(ast_root->firstChild->nextSibling->nextSibling,st,output);
                fprintf(output, "mov ah, 4ch\nmov al, 0\nint 21h\n\nmain endp\n\n");
          }

          else if(strcmp(ast_root->keyword,"<otherStmts>")==0)
          {

                codeGen(ast_root->firstChild,st,output);
                codeGen(ast_root->firstChild->nextSibling,st,output);

          }

          else if(strcmp(ast_root->keyword,"<stmt>")==0)
          {
                codeGen(ast_root->firstChild,st,output);
          }

          else if(strcmp(ast_root->keyword,"<assignmentStmt>")==0)
          {
                codeGen(ast_root->firstChild->nextSibling,st,output);
                fprintf(output, "mov ");
                codeGen(ast_root->firstChild,st,output);
                fprintf(output, ", ax\n\n");
          }

          else if(strcmp(ast_root->keyword,"<singleOrRecId>")==0)
          {
                codeGen(ast_root->firstChild,st,output);
                if(ast_root->firstChild->nextSibling->firstChild!=NULL && strcmp(ast_root->firstChild->nextSibling->firstChild->keyword,"eps")!=0)
                {
                    fprintf(output, "_");
                    codeGen(ast_root->firstChild->nextSibling->firstChild,st,output);
                }
          }

          else if(strcmp(ast_root->keyword,"<conditionalStmt>")==0)
          {
                snprintf(ifNoBuf, 10, "%d", ifNo);
                codeGen(ast_root->firstChild->nextSibling,st,output);
                fprintf(output, "cmp ax, 0\n");
                fprintf(output, "je ELSE%s\n", ifNoBuf);
                codeGen(ast_root->firstChild->nextSibling->nextSibling,st,output);
                codeGen(ast_root->firstChild->nextSibling->nextSibling->nextSibling,st,output);
                fprintf(output, "jmp ENDIF%s\n", ifNoBuf);
                fprintf(output, "ELSE%s:\n", ifNoBuf);
                codeGen(ast_root->firstChild->nextSibling->nextSibling->nextSibling->nextSibling,st,output);
                fprintf(output, "ENDIF%s:\n\n", ifNoBuf);
                ifNo++;
          }

          else if(strcmp(ast_root->keyword,"<elsePart>")==0)
          {
                if(ast_root->firstChild!=NULL && strcmp(ast_root->firstChild->keyword,"TK_ELSE")==0)
                {
                    codeGen(ast_root->firstChild->nextSibling,st,output);
                    codeGen(ast_root->firstChild->nextSibling->nextSibling,st,output);
                }
          }

          else if(strcmp(ast_root->keyword,"<ioStmt>")==0)
          {
                if(strcmp(ast_root->firstChild->keyword,"TK_READ")==0)
                {
                    fprintf(output, "call _read\n");
                    fprintf(output, "mov ");
                    codeGen(ast_root->firstChild->nextSibling,st,output);
                    fprintf(output, ", ax\n\n");
                }
                else {
                    fprintf(output, "mov ");
                    fprintf(output, "ax,");

                    codeGen(ast_root->firstChild->nextSibling,st,output);
                    fprintf(output, "\ncall _write\n\n");
                }
            }

          else if(strcmp(ast_root->keyword,"<iterativeStmt>")==0)
          {
                snprintf(loopNoBuf, 10, "%d", loopNo);
                fprintf(output, "STARTLOOP%s:\n", loopNoBuf);
                codeGen(ast_root->firstChild->nextSibling,st,output);
                fprintf(output, "cmp ax, 0\nje ENDLOOP%s\n", loopNoBuf);
                codeGen(ast_root->firstChild->nextSibling->nextSibling,st,output);
                codeGen(ast_root->firstChild->nextSibling->nextSibling->nextSibling,st,output);
                fprintf(output, "jmp STARTLOOP%s\nENDLOOP%s:\n\n", loopNoBuf, loopNoBuf);
                loopNo++;
            }

          else if(strcmp(ast_root->keyword,"<allVar>")==0)
          {
                if(strcmp(ast_root->firstChild->keyword,"<var>")==0)
                    codeGen(ast_root->firstChild,st,output);
          }

          else if(strcmp(ast_root->keyword,"<var>")==0)
          {
                codeGen(ast_root->firstChild,st,output);
          }

          else if(strcmp(ast_root->keyword,"<arithmeticExpression>")==0)
          {
                codeGen(ast_root->firstChild,st,output);
                if(strcmp(ast_root->firstChild->nextSibling->firstChild->keyword,"<lowPrecedenceOperators>")==0)
                {
                    fprintf(output, "push ax\n");
                    codeGen(ast_root->firstChild->nextSibling,st,output);
                    fprintf(output, "mov bx, ax\npop ax\n");
                    if(strcmp(ast_root->firstChild->nextSibling->firstChild->firstChild->keyword,"TK_PLUS")==0)
                        fprintf(output, "add ax, bx\n\n");
                    else
                        fprintf(output, "sub ax, bx\n\n");
                }
          }

          else if(strcmp(ast_root->keyword,"<term>")==0)
          {
                codeGen(ast_root->firstChild,st,output);
                if(strcmp(ast_root->firstChild->nextSibling->firstChild->keyword,"<highPrecedenceOperators>")==0)
                {
                    fprintf(output, "push ax\n");
                    codeGen(ast_root->firstChild->nextSibling,st,output);
                    fprintf(output, "mov bx, ax\npop ax\n");
                    if(strcmp(ast_root->firstChild->nextSibling->firstChild->firstChild->keyword, "TK_MUL")==0)
                        fprintf(output, "mul bx\n");
                    else
                        fprintf(output, "mov dx, 0\ndiv bx\n");
                }
          }

          else if(strcmp(ast_root->keyword,"<factor>")==0)
          {
              // factor will always have one child because TK_OP and TK_CL have been removed
              codeGen(ast_root->firstChild,st,output);
          }

          else if(strcmp(ast_root->keyword,"<termPrime>")==0)
          {
                if(strcmp(ast_root->firstChild->keyword,"<highPrecedenceOperators>")==0)
                {
                    codeGen(ast_root->firstChild->nextSibling,st,output);
                    if(strcmp(ast_root->firstChild->nextSibling->nextSibling->firstChild->keyword,"<highPrecedenceOperators>")==0)
                    {
                        fprintf(output, "push ax\n");
                        codeGen(ast_root->firstChild->nextSibling->nextSibling,st,output);
                        fprintf(output, "mov bx, ax\npop ax\n");
                        if(strcmp(ast_root->firstChild->nextSibling->nextSibling->firstChild->firstChild->keyword,"TK_MUL")==0)
                            fprintf(output, "mul ax, bx\n");
                        else
                            fprintf(output, "div ax, bx\n");
                    }
                }
          }

          else if(strcmp(ast_root->keyword,"<expPrime>")==0)
          {
                if(strcmp(ast_root->firstChild->keyword,"<lowPrecedenceOperators>")==0)
                {
                    codeGen(ast_root->firstChild->nextSibling,st,output);
                    if(strcmp(ast_root->firstChild->nextSibling->nextSibling->firstChild->keyword,"<lowPrecedenceOperators>")==0)
                    {
                        fprintf(output, "push ax\n");
                        codeGen(ast_root->firstChild->nextSibling->nextSibling,st,output);
                        fprintf(output, "mov bx, ax\npop ax\n");
                        if(strcmp(ast_root->firstChild->nextSibling->nextSibling->firstChild->firstChild->keyword,"TK_PLUS")==0)
                            fprintf(output, "add ax, bx\n");
                        else
                            fprintf(output, "sub ax, bx\n");
                    }
                }
          }

          else if(strcmp(ast_root->keyword,"<all>")==0)
          {
                if(ast_root->firstChild->nextSibling!=NULL && strcmp(ast_root->firstChild->nextSibling->firstChild->keyword,"eps")==0 )
                {
                    fprintf(output, "mov ax, ");
                    codeGen(ast_root->firstChild,st,output);
                    fprintf(output, "\n");
                }
          }

          else if(strcmp(ast_root->keyword,"<booleanExpression>")==0)
          {
                if(strcmp(ast_root->firstChild->keyword,"<booleanExpression>")==0)
                {
                    codeGen(ast_root->firstChild,st,output);
                    fprintf(output, "push ax\n");
                    codeGen(ast_root->firstChild->nextSibling->nextSibling,st,output);
                    fprintf(output, "pop bx\n");
                    if(strcmp(ast_root->firstChild->nextSibling->firstChild->keyword,"TK_AND")==0)
                        fprintf(output, "and ax, bx\n\n");
                    else
                        fprintf(output, "or ax, bx\n\n");
                }
                else if(strcmp(ast_root->firstChild->keyword,"TK_NOT")==0)
                {
                    codeGen(ast_root->firstChild->nextSibling,st,output);
                    fprintf(output, "not ax\n\n");
                }
                else
                {
                    fprintf(output, "mov ax, ");
                    codeGen(ast_root->firstChild,st,output);
                    fprintf(output, "\npush ax\nmov ax, ");
                    codeGen(ast_root->firstChild->nextSibling->nextSibling,st,output);
                    fprintf(output, "\nmov bx, ax\npop ax\ncmp ax, bx\n");
                    fprintf(output, "pushf\npop ax\n");
                        if(strcmp(ast_root->firstChild->nextSibling->firstChild->keyword,"TK_LT")==0)
                        {
                            fprintf(output, "and ax, 0880h\nmov cl, 3\nshr ah, cl\n");
                            fprintf(output, "mov cl, 7\nshr al, cl\n");
                            fprintf(output, "xor al, ah\nmov ah, 0\n\n");
                        }

                        if(strcmp(ast_root->firstChild->nextSibling->firstChild->keyword,"TK_LE")==0)
                        {
                            fprintf(output, "mov bl, al\nand ax, 0880h\nmov cl, 3\nshr ah, cl\n");
                            fprintf(output, "mov cl, 7\nshr al, cl\n");
                            fprintf(output, "xor al, ah\nand bl, 40h\nmov cl, 6\nshr bl, cl\n");
                            fprintf(output, "or al, bl\nmov ah, 0\n\n");
                        }

                        if(strcmp(ast_root->firstChild->nextSibling->firstChild->keyword,"TK_EQ")==0)
                        {
                            fprintf(output, "and ax, 0040h\nmov cl, 6\nshr al, cl\n\n");
                        }

                        if(strcmp(ast_root->firstChild->nextSibling->firstChild->keyword,"TK_GT")==0)
                        {
                            fprintf(output, "mov bl, al\nand ax, 0880h\nmov cl, 3\nshr ah, cl\n");
                            fprintf(output, "mov cl, 7\nshr al, cl\n");
                            fprintf(output, "xor al, ah\nnot al\nand al, 01h\nnot bl\nand bl, 40h\n");
                            fprintf(output, "mov cl, 6\nshr bl, cl\n");
                            fprintf(output, "and al, bl\nmov ah, 0\n\n");
                        }

                        if(strcmp(ast_root->firstChild->nextSibling->firstChild->keyword,"TK_GE")==0)
                        {
                            fprintf(output, "and ax, 0880h\nmov cl, 3\nshr ah, cl\n");
                            fprintf(output, "mov cl, 7\nshr al, cl\n");
                            fprintf(output, "xor al, ah\nnot al\nand ax, 0001h\n\n");
                        }

                        if(strcmp(ast_root->firstChild->nextSibling->firstChild->keyword,"TK_NE")==0)
                        {
                            fprintf(output, "and ax, 0040h\nmov cl, 6\nshr al, cl\n");
                            fprintf(output, "not al\nand ax, 0001h\n\n");
                        }

                }
              }

        }

    else
        fprintf(output, "%s", ast_root->tk.lexeme);

}
