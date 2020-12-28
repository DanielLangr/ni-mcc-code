	.intel_syntax noprefix 

	.global inc
	.global atomic_inc

	.text

inc:
	add QWORD PTR [rdi], 1
	ret

atomic_inc:
	lock add QWORD PTR [rdi], 1
	ret
