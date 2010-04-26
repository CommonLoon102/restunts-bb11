; This is a small asm-proxy for servicing int16 calls. 
; Calls kb_int16_handler_c() with ax as the parameter.
.model medium
	extrn kb_int16_handler_c:proc
cseg segment byte public 'CODE' use16
	assume cs:cseg
	assume es:nothing, ss:nothing, ds:nothing

	public kb_int16_handler

kb_int16_handler proc far
	push bx
	push ds
	push ax
	call kb_int16_handler_c
	add sp, 2
	pop ds
	pop bx
	iret
kb_int16_handler endp

cseg ends
end
