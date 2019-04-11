global main

;Declare used libc functions
extern exit
extern puts
extern scanf
extern printf

section .text

main:

;Arguments are passed in reversed order via stack (for x86)
;For x64 first six arguments are passed in straight order
;  via RDI, RSI, RDX, RCX, R8, R9 and other are passed via stack
;The result comes back in EAX/RAX
push dword msg
call puts
;After passing arguments via stack, you have to clear it to
;  prevent segfault with add esp, 4 * (number of arguments)
add esp, 4

push dword a
push dword b
push dword msg1
call scanf
add esp, 12
;For x64 this scanf call will look like:
;  mov rdi, msg1
;  mov rsi, b
;  mov rdx, a
;  call scanf

mov eax, dword [a]
add eax, dword [b]
push eax
push dword msg2
call printf
add esp, 8

push dword 0
call exit
add esp, 4
ret

section .data
msg  : db "An example of interfacing with GLIBC.",0xA,0
msg1 : db "%d%d",0
msg2 : db "%d", 0xA, 0

section .bss
a resd 1
b resd 1
