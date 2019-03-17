compileAll : lexer.o syn.o gmrCall.o nAry.o parseStack.o parse.o  driver.o
	gcc -g  -o stage1exe lexer.o syn.o gmrCall.o nAry.o parseStack.o parse.o driver.o

run :
	./stage1exe

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

driver.o: driver.c
	gcc -g  -c driver.c
clean :
	rm stage1exe lexer.o syn.o gmrCall.o nAry.o parseStack.o parse.o driver.o
