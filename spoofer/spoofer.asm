PUBLIC SpooferStub

.code

SpooferStub PROC
    pop r11

    add rsp, 8
    mov rax, [rsp + 24]
    
    mov r10, [rax]
    mov [rsp], r10
    
    mov r10, [rax + 8]
    mov [rax + 8], r11
    
    mov [rax + 16], r14
    lea r14, Fixup
    mov [rax], r14
    mov r14, rax
    
    jmp r10

Fixup:
    sub rsp, 16
    mov rcx, r14
    mov r14, [rcx + 16]
    jmp QWORD PTR [rcx + 8]
SpooferStub ENDP

END
