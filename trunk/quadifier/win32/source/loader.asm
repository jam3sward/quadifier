ifdef X64
.code
; This 64-bit code is run on a thread created inside the target process.
; It loads the specified DLL then exits the thread.
;
; This function receives a single parameter in RCX which is a pointer
; to the base address of the Data structure. Note that the offset
; addresses used below must match the layout of the Data structure in
; DLLInject.cpp
;
loaderCode PROC
	mov	qword ptr [rsp+8], rbx	; save RBX in spill
	push rdi					; save RDI
	sub	rsp, 32					; set up stack frame (register spill)
	mov	rbx, rcx				; base address of structure
	add	rcx, 40					; RCX = pathName
	xor	edi, edi				; EDI = 0
	call qword ptr [rbx]		; RAX = LoadLibrary( pathName )
	test rax, rax				; was it succesful?
	jne	done					; yes: successfully loaded
failure:
	call qword ptr [rbx+24]		; EAX = GetLastError()
	mov	edi, eax				; EDI = exit code (last error)
done:
	mov	ecx, edi				; ECX = exit code (0 or last error)
	call qword ptr [rbx+32]		; threadExitFunc( ECX )
	mov	rbx, qword ptr [rsp+48]	; restore RBX
	mov	eax, edi				; exit code in EAX for return
	add	rsp, 32					; clean up stack frame
	pop	rdi						; restore RDI
	ret	0
loaderCode ENDP
loaderCodeEnd PROC
	ret
loaderCodeEnd ENDP
endif;X64

END