
;########### LISTA DE FUNCIONES IMPORTADAS
extern save_result

;########### SECCION DE DATOS
section .data

filename_5: db 'results_5',0
filename_9: db 'results_9',0

;########### SECCION DE TEXTO (PROGRAMA)
section .text

;########### LISTA DE FUNCIONES EXPORTADAS
global alternate_sum_5
global alternate_sum_9

;########### DEFINICION DE FUNCIONES
; la funcion debe devolver el resultado de x1 - x2 + x3 - x4 + x5
; uint32_t alternate_sum_5(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4, uint32_t x5);
; registros: x1[rdi], x2[rsi], x3[rdx], x4[rcx], x5[r8]
alternate_sum_5:
	;prologo
	;recordar que si la pila estaba alineada a 16 al hacer la llamada
	;con el push de RIP como efecto del CALL queda alineada a 8       
	push rbp	                                                       ; se hace un stack frame nuevo
	mov rbp, rsp                                                       ; 

	sub rsp, 8	                                                        ; alineamos a 16
	xor rax, rax	; limpiamos rax
	add eax, edi	; realizamos las sumas alternadas                   x1
	sub eax, esi                                                    ;   x1 - x2
 

	add eax, edx                                                    ;    x1 - x2 + x3
	sub eax, ecx                                                    ;    x1 - x2 + x3 - x4
	add eax, r8d	                                                ;    x1 - x2 + x3 - x4 + x5


	mov r12d, eax                                                   ; r12d <- eax         	R12D ES NO VOLATIL!!!!!!!!!!
	mov rdi, filename_5                                             ; rdi <- 0x0000         convencion C: metemos su primer parametro en edi
	mov esi, eax                                                    ; esi <- eax            covnecnion, metemos a result en esi
	call save_result                                                ; save result esta en C, convencion al reves! llama asm a funcion en C
	mov eax, r12d                                                   ; eax <- r12d    (nunca restaura el calor de r12, no se repstea el invariante de funcion con r12)
	
	;epilogo
	add rsp, 8	


	pop rbp                                                         ; bajo de stack frame
	ret


	
; la funcion debe devolver el resultado de x1 - x2 + x3 - x4 + x5 - x6 + x7 - x8 + x9
; uint32_t alternate_sum_9(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4, uint32_t x5, uint32_t x6, uint32_t x7, uint32_t x8, uint32_t x9);	
; registros y pila: x1[rdi], x2[rsi], x3[rdx], x4[rcx], x5[r8], x6[r9], x7[rbp + 0x10], x8[rbp + 0x18], x9[rbp + 0x20]
alternate_sum_9:
	;prologue
	push rbp	
	mov rbp, rsp     ; nuevo stack frame
	push rbx	     ; pushea todos los registros no volatiles
	push r12	
	push r13	
	push r14	
	push r15	

	sub rsp, 8	     ; padding a 16, aca esta alineada

	lea rbx, [rbp + 0x10]	; rbx <- rbp + 16
	mov r10d, [rbx]		; r10d <- x7                        
	lea rbx, [rbp + 0x18]	; rbx <- rbp + 24
	mov r11d, [rbx]		; r11d <- x8
	lea rbx, [rbp + 0x20]	; rbx <- rbp + 32
	mov r12d, [rbx]		; r12d <- x9



	xor rax, rax	; limpiamos rax
	add eax, edi	; sumamos todos los elementos
	sub eax, esi
	add eax, edx
	sub eax, ecx
	add eax, r8d
	sub eax, r9d
	add eax, r10d
	sub eax, r11d
	add eax, r12d	; el resultado queda en eax
	
	mov r13d, eax
	mov rdi, filename_9
	mov esi, eax
	call save_result
	mov eax, r13d
	
	;epilogue
	add rsp, 8


	pop r14
	pop r15	  ;; popea mal! NO conserva los valores de r15 y r14!! Los intercambio . Este es un error grande, pero igual fijate los alineamientos :)
	pop r13
	pop r12
	pop rbx
	
	pop rbp	
	ret
	
