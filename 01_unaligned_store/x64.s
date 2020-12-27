	.intel_syntax noprefix 

	.global store
	.global load

	.text

store:
	mov eax, 0
	mov WORD PTR [rdi], ax
	mov eax, 0x0101
	mov WORD PTR [rdi], ax
	ret

load:
	movzx eax, WORD PTR [rdi]
	ret
