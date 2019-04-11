
section .stack

section .bss

inp_buf resb 256



section .data
msg2: db 'You entered: '
msg2_size: equ $-msg2

msg: db 'Input some data: '


msg_size: equ $-msg


 b2 :dw 0
 c2 :dw 0
 d2: dw 0
 b3: dw 0
section .text
global _read
global _write
global main

_read :
mov  eax,4		;sys_wite
mov  ebx,1		;To stdout
mov  ecx,msg		;'Input some data: '
mov  edx,msg_size
int  80h		;Call kernel

mov  ax,3		;sys_read. Read what user inputs
mov  ebx,0		;From stdin
mov  ecx,inp_buf	;Save user input to buffer.
int    80h

_write :
push ax

mov  eax,4
mov  ebx,1
mov  ecx,msg2		;'You entered: '
mov  edx,msg2_size
int    80h

mov  ax,4
mov  ebx,1
mov  ecx,inp_buf
pop  edx
int  80h

mov  ax,1
mov  ebx,0
int  80h

main:
call _read
call _write


int 80h
