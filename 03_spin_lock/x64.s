	.intel_syntax noprefix 

	.global spin_lock_naive
	.global spin_lock

	.text

spin_lock_naive:
        mov   edx, 1
loop1:
	add  qword ptr [rsi], 1
	mov  eax, edx
	xchg al, byte ptr [rdi]
	test al, al
	jne  loop1
	mov  byte ptr [rdi], 0
	ret

spin_lock:
        mov   edx, 1
loop2:
	add   qword ptr [rsi], 1
	movzx eax, byte ptr [rdi] 
	test  al, al
	jne   loop2
	add   qword ptr [rsi], 1
	mov   eax, edx
	xchg  al, byte ptr [rdi]
	test  al, al
	jne   loop2
	mov  byte ptr [rdi], 0
	ret
