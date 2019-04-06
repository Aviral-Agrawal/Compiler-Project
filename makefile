compileAll : lexer.o syn.o gmrCall.o nAry.o parseStack.o parse.o ast.o dbg_sym.o driver.o
	gcc -g  -o stage1exe lexer.o syn.o gmrCall.o nAry.o parseStack.o parse.o ast.o dbg_sym.o driver.o

compileLexer : lexer.o
	gcc -g -o lexerexe lexer.o

run :
	./stage1exe

remove :
	rm *.o *.gch exe

debug :
	gdb stage1exe

lexer.o: lexerDef.h lexer.h lexer.c
	gcc -g  -c lexerDef.h lexer.h lexer.c

syn.o: syn.h syn.c
	gcc -g  -c  syn.h syn.c

gmrCall.o: gmrCall.h gmrCall.c
	gcc -g -c  gmrCall.h gmrCall.c

nAry.o: nAry.h nAry.c
	gcc -g  -c nAry.h nAry.c

parseStack.o: parseStack.c
	gcc -g  -c parseStack.c

parse.o: parse.c
	gcc -g  -c parse.c

ast.o: ast.h ast.c
	gcc -g -c ast.h ast.c

dbg_sym.o: symbolTable.h dbg_sym.c
	gcc -g -c symbolTable.h dbg_sym.c

driver.o: driver.c
	gcc -g  -c driver.c

clean :
	rm stage1exe lexer.o syn.o gmrCall.o nAry.o parseStack.o parse.o ast.o driver.o
