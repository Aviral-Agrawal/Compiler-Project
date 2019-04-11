;to run the code
; nasm -f elf code1.asm -o code1.o
; gcc -m32 code1.o -o code1.out
; ./code1.out
section .stack

section .bss

inp_buf :resb 256
inp_buf1 resb 256



section .data


msg2_size: equ $-msg2
msg: db 'Input some data: '
msg_size: equ $-msg
msg1 : db "%d",0
msg2 : db "%d", 0xA, 0


 b2 :dd 0
 c2 :dd 0
 d2: dd 0
 b3: dd 0
section .text
global _read
global _write
global main
extern printf
extern scanf
extern exit

_read :

mov  eax,3		;sys_read. Read what user inputs
mov  ebx,0		;From stdin
mov  ecx,inp_buf	;Save user input to buffer.
int    80h
ret

_write :
push eax

mov  eax,4
mov  ebx,1
mov  ecx,inp_buf
pop  edx
int  80h

ret

main:


push eax
push dword msg1
call scanf
add esp, 8
mov dword[b2],eax

push eax
push dword msg2
call printf

add esp, 8

mov dword[c2],20d


push eax
push msg1
call scanf
add esp, 8
mov dword[d2],eax


mov eax, dword[b2]
push eax
mov eax,dword[c2]
push eax
mov eax, dword[d2]
mov ebx, eax
pop eax

add eax, ebx

mov ebx, eax
pop eax
add eax, ebx

mov dword[b3], eax

mov eax,dword[b3]

push eax
push dword msg2
call printf

add esp, 8



push dword 0
call exit
add esp, 4
ret