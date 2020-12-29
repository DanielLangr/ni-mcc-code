	.global store
	.global load

	.text

store:
	mov w1, 0x0000
	strh w1, [x0]
	mov w1, 0x0101
	strh w1, [x0]
	ret

load:
	ldrh w0, [x0]
	ret
