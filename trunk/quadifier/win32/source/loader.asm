ifdef X64
.code
; This 64-bit code is run on a thread created inside the target process.
; It loads the specified DLL then exits the thread. Note that the offset
; addresses used below must match the layout of the Data structure in
; DLLInject.cpp
loaderCode PROC
	mov	qword ptr [rsp+8], rbx	; parameter (base address of structure)
	push rdi
	sub	rsp, 32
	mov	rbx, rcx
	add	rcx, 40					; pathName
	xor	edi, edi
	call qword ptr [rbx]		; LoadLibrary
	test rax, rax				; was it succesful?
	jne	done					; yes: successfully loaded
failure:
	call qword ptr [rbx+24]		; GetLastError
	mov	edi, eax				; exit code (last error)
done:
	mov	ecx, edi				; exit code
	call qword ptr [rbx+32]		; threadExitFunc
	mov	rbx, qword ptr [rsp+48]
	mov	eax, edi				; exit code
	add	rsp, 32
	pop	rdi
	ret	0
loaderCode ENDP
loaderCodeEnd PROC
	ret
loaderCodeEnd ENDP
endif;X64

END