PUBLIC	udiv128
PUBLIC	sdiv128

_TEXT	SEGMENT
udiv128	PROC
	mov rax,rdx
	mov rdx,rcx
	div r8
	mov [r9],rdx
	ret
udiv128	ENDP
_TEXT	ENDS

_TEXT	SEGMENT
sdiv128	PROC
	mov rax,rdx
	mov rdx,rcx
	idiv r8
	mov [r9],rdx
	ret
sdiv128	ENDP
_TEXT	ENDS
END
