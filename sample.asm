;nasm 2.11.08

section .data
    hello:     db 'Hello world!',10
    helloLen:  equ $-hello

section .text
	global main

main:
	mov eax,4
	mov ebx,1
	mov ecx,hello
	mov edx,helloLen
	int 80h

	mov eax,1
	int 80h
