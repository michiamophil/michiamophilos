;* file 	arch/x86/interrupt.s
;* autore 	Filippo Bedendo Bordon
;* data 	25-11-11	


;Descrizioni errori prese da http://wiki.unix-below.net/doku.php?id=la_idt
;			 |		CAUSA		   |   CODICE ERRORE	| 	
global isr0		;Divide Error	 		No
global isr1		;Debug	 				No
global isr2		;Non Maskable Interrupt
global isr3 	;BreakPoint	 			No
global isr4		;OverFlow	 			No
global isr5		;Bound Range Exceeded	No
global isr6		;Invalid Opcode	 		No
global isr7		;Coprocessore matematico non disponibile	 No
global isr8		;Double Fault	 		Si (0)
global isr9		;Coprocessor Segment Overrun (reserved)	 No
global isr10	;Invalid TS	 			Si
global isr11	;Segment Not Present	Si
global isr12	;Stack Segment Fault	Si
global isr13	;General Protection		Si
global isr14	;Page Fault	 			Si
; Il numero 15 fa parte di quelle riservate alla intel
global isr16	;Floating Point Error (Math Fault)	 No
global isr17	;Alignment Check	 	Si (0)
global isr18	;Machine Check	 		No
global isr19	;Streaming SIMD Extensions	 No
global isr_reserved; Intel Reserved

global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

isr0:
	cli							; Disabilita gli interrupt
	push byte 0					; Carica un codice d'errore
	push byte 0					; Carica il numero dell'interrupt
	jmp gestione_isr			; Va all'handler

isr1:
	cli
	push byte 0
	push byte 1
	jmp gestione_isr

isr2:
	cli
	push byte 0
	push byte 2
	jmp gestione_isr

isr3:
	cli
	push byte 0
	push byte 3
	jmp gestione_isr

isr4:
	cli
	push byte 0
	push byte 4
	jmp gestione_isr

isr5:
	cli
	push byte 0
	push byte 5
	jmp gestione_isr

isr6:
	cli
	push byte 0
	push byte 6
	jmp gestione_isr

isr7:
	cli
	push byte 0
	push byte 7
	jmp gestione_isr

isr8:
	cli
	push byte 0
	push byte 8
	jmp gestione_isr

isr9:
	cli
	push byte 0
	push byte 9
	jmp gestione_isr

isr10:	; Per ora non carica l'errore
	cli
	push byte 0
	push byte 10
	jmp gestione_isr

isr11:	; Per ora non carica l'errore
	cli
	push byte 0
	push byte 11
	jmp gestione_isr

isr12:	; Per ora non carica l'errore
	cli
	push byte 0
	push byte 12
	jmp gestione_isr

isr13:	; Per ora non carica l'errore
	cli
	push byte 0
	push byte 10
	jmp gestione_isr

isr14:	; ; ###NO push byte 0### L'errore lo carica già la cpu!!!!!
	cli
	push byte 14
	jmp gestione_isr

isr16:
	cli
	push byte 0
	push byte 16
	jmp gestione_isr

isr17:
	cli
	push byte 0
	push byte 17
	jmp gestione_isr

isr18:
	cli
	push byte 0
	push byte 18
	jmp gestione_isr

isr19:
	cli
	push byte 0
	push byte 19
	jmp gestione_isr

isr20:
	cli
	push byte 0
	push byte 20
	jmp gestione_isr

isr21:
	cli
	push byte 0
	push byte 21
	jmp gestione_isr


isr_reserved:
	cli
	push byte 0
	push byte 15
	jmp gestione_isr

; Timer che chiama lo scheduler (Infesta il meno possibile lo stack :P)
irq0:
	cli
	push byte 0
	push byte 32
	jmp timer_irq_handler

irq1:
	cli
	push byte 0
	push byte 33
	jmp gestione_irq

irq2:
	cli
	push byte 0
	push byte 34
	jmp gestione_irq

irq3:
	cli
	push byte 0
	push byte 35
	jmp gestione_irq

irq4:
	cli
	push byte 0
	push byte 36
	jmp gestione_irq

irq5:
	cli
	push byte 0
	push byte 37
	jmp gestione_irq

irq6:
	cli
	push byte 0
	push byte 38
	jmp gestione_irq

irq7:
	cli
	push byte 0
	push byte 39
	jmp gestione_irq

irq8:
	cli
	push byte 0
	push byte 40
	jmp gestione_irq

irq9:
	cli
	push byte 0
	push byte 41
	jmp gestione_irq

irq10:
	cli
	push byte 0
	push byte 42
	jmp gestione_irq

irq11:
	cli
	push byte 0
	push byte 43
	jmp gestione_irq

irq12:
	cli
	push byte 0
	push byte 44
	jmp gestione_irq

irq13:
	cli
	push byte 0
	push byte 45
	jmp gestione_irq

irq14:
	cli
	push byte 0
	push byte 46
	jmp gestione_irq

irq15:
	cli
	push byte 0
	push byte 47
	jmp gestione_irq

extern isr_handler
gestione_isr:
	pusha			; Salva nello stack edi,esi,ebp,esp,ebx,edx,ecx,eax
	mov ax, ds		; ds = (short) eax 
	push eax		; salva il semento del codice

	mov ax, 0x10	; carica il segmento del kernel
mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	call isr_handler	; Esegue la gestione dell'interrupt in C
	pop ebx			; Ricarica il segmento percedentemente salvato
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx

	popa			; Ripristina tutti i registri general purpose
	add esp, 8		; Rimuove 4 byte per argomento (errore e numero interrupt)
	sti				; Riabilita gli interrupt
	iret			; Ritorna dall'interrupt

extern irq_handler
extern debug_break
gestione_irq:
	pusha
	mov ax, ds
	push eax

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	call irq_handler	; Esegue la gestione dell'interrupt in C
	pop ebx
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx
	
	popa
	add esp, 8
	sti
	iret
extern timer_tick
timer_irq_handler:
	;mov edi, 0xFFFFFFFF
	pusha
	mov ax, ds
	push eax
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	call timer_tick
	pop ebx
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx
	
	popa
	add esp, 8
	sti
	iret
	
