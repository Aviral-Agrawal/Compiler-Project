section .model small
section .stack
section .data
inputbuf: times 12 db 0
 b2 :dw 0
 c2 :dw 0
 d2: dw 0
 b3: dw 0
section .text
global _read
global _write
global main

_read :
    push si;
    mov dx, [inputbuf];
    mov bx, dx;
    mov byte [bx], 10;
    mov ah, 0ah;


    int 21h

    mov ah, 02h;
    mov dl, 10;
    int 21h

    mov bx, [inputbuf + 1]
    mov cx, 0
    mov cl,byte [bx]
    mov ax, 0

    mov si, 0

    inc bx
    mov dl,byte [bx]
    cmp dl, 45
    jne _startgetting
    	mov si, 1
    	dec cx
    	jmp _getdigitfrombuf
    _startgetting:
    	dec bx
    _getdigitfrombuf:
        inc bx
        mov dx, 10
        mul dx
        mov dh, 0
        mov dl,byte [bx]
        sub dx, 48
        add ax, dx

        loop _getdigitfrombuf

    cmp si, 1
    jne _readend
    	not ax
    	add ax, 1

    _readend:
        pop si
        ret

_write :
    mov bx, ax
    and bx, 8000h
    cmp bx, 0
    je _writesignhandled
    not ax
    add ax,1
    mov bx, ax
    mov ah, 02h
    mov dl, 45
    int 21h
    mov ax, bx

    _writesignhandled:
    mov cx, 0
    mov bx, 10
    mov dx, 0
    _getdigitfromnum:
        div bx

        push dx
        inc cx

        mov dx, 0

        cmp ax, 0
        jne _getdigitfromnum

        mov ah, 02h

    _writedigit:
        pop dx
        add dl, 48
        int 21h

        dec cx
        cmp cx, 0
        jne _writedigit
    ret

main:
mov ax, inputbuf
mov ds, ax

call _read
mov word[b2], ax

mov word[c2], ax

call _read
mov word[d2], ax

mov ax, [b2]
push ax
mov ax, [c2]
push ax
mov ax, [d2]
mov bx, ax
pop ax
add ax, bx
mov bx, ax
pop ax
add ax, bx

mov word[b3], ax

mov ax,[b3]
call _write

mov ah, 4ch
mov al, 0
int 80h
