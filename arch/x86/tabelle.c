/**
 * file 	arch/x86/tabelle.c
 * autore 	Filippo Bedendo Bordon
 * data 	22-11-11	
 */

#include <x86/tabelle.h>
#include <x86/mem_map.h>
#include <x86/interrupt.h>
#include <x86/debug.h>

struct idt_entry *idt = (struct idt_entry *) IDT_ADDR;	// Struttura da 8 byte

void init_gdt()
{
	#ifdef DEBUG
	debug_stringa("Init_gdt()");
	#endif
		
	// Puntatore alla destinazione della gdt (var da 8 byte)
	unsigned long long *gdt = (unsigned long long *) GDT_ADDR;
	struct DTR gdtr; // Struttura da 6 byte (limite & indirizzo)
	
	// 1) Descrittore nullo
	*gdt++ = 0x0;
	// 2) Descrittore del codice (vedi doc/gdt)
	*gdt++ = 0x00CF9A000000FFFF;
	// 3) Descrittore dei dati
	*gdt++ = 0x00CF92000000FFFF;
	
	// Setta la variabile da caricare nel registro gdtr
	gdtr.limite = GDT_SIZE - 1;
	gdtr.base = GDT_ADDR;
	
	// Procedura che setta il registro gdtr. Da per scontato che i vari
	// Registri siano stati settati precedentemente da grub
	asm("lgdtl %0 			\n"
		"ljmp $0x08, $go 	\n" 
		"go: 				  " 
		: : "m" (gdtr) : "eax");
}

void init_idt()
{
	idt_setta_entry(0, 0x08, 0x8E, (int)isr0);
	idt_setta_entry(1, 0x08, 0x8E, (int)isr1);
	idt_setta_entry(2, 0x08, 0x8E, (int)isr2);
	idt_setta_entry(3, 0x08, 0x8E, (int)isr3);
	idt_setta_entry(4, 0x08, 0x8E, (int)isr5);
	idt_setta_entry(5, 0x08, 0x8E, (int)isr6);
	idt_setta_entry(6, 0x08, 0x8E, (int)isr6);
	idt_setta_entry(7, 0x08, 0x8E, (int)isr7);
	idt_setta_entry(8, 0x08, 0x8E, (int)isr8);
	idt_setta_entry(9, 0x08, 0x8E, (int)isr9);
	idt_setta_entry(10, 0x08, 0x8E, (int)isr10);
	idt_setta_entry(11, 0x08, 0x8E, (int)isr11);
	idt_setta_entry(12, 0x08, 0x8E, (int)isr12);
	idt_setta_entry(13, 0x08, 0x8E, (int)isr13);
	idt_setta_entry(14, 0x08, 0x8E, (int)isr14);
	idt_setta_entry(15, 0x08, 0x8E, (int)isr_reserved);
	idt_setta_entry(16, 0x08, 0x8E, (int)isr16);
	idt_setta_entry(17, 0x08, 0x8E, (int)isr17);
	idt_setta_entry(18, 0x08, 0x8E, (int)isr18);
	idt_setta_entry(19, 0x08, 0x8E, (int)isr19);
	idt_setta_entry(20, 0x08, 0x8E, (int)isr_reserved);
	idt_setta_entry(21, 0x08, 0x8E, (int)isr_reserved);
	idt_setta_entry(22, 0x08, 0x8E, (int)isr_reserved);
	idt_setta_entry(23, 0x08, 0x8E, (int)isr_reserved);
	idt_setta_entry(24, 0x08, 0x8E, (int)isr_reserved);
	idt_setta_entry(25, 0x08, 0x8E, (int)isr_reserved);
	idt_setta_entry(26, 0x08, 0x8E, (int)isr_reserved);
	idt_setta_entry(27, 0x08, 0x8E, (int)isr_reserved);
	idt_setta_entry(28, 0x08, 0x8E, (int)isr_reserved);
	idt_setta_entry(29, 0x08, 0x8E, (int)isr_reserved);
	idt_setta_entry(30, 0x08, 0x8E, (int)isr_reserved);
	idt_setta_entry(31, 0x08, 0x8E, (int)isr_reserved);
	idt_setta_entry(32, 0x08, 0x8E, (int)irq0);
	idt_setta_entry(33, 0x08, 0x8E, (int)irq1);
	idt_setta_entry(34, 0x08, 0x8E, (int)irq2);
	idt_setta_entry(35, 0x08, 0x8E, (int)irq3);
	idt_setta_entry(36, 0x08, 0x8E, (int)irq4);
	idt_setta_entry(37, 0x08, 0x8E, (int)irq5);
	idt_setta_entry(38, 0x08, 0x8E, (int)irq6);
	idt_setta_entry(39, 0x08, 0x8E, (int)irq7);
	idt_setta_entry(40, 0x08, 0x8E, (int)irq8);
	idt_setta_entry(41, 0x08, 0x8E, (int)irq9);
	idt_setta_entry(42, 0x08, 0x8E, (int)irq10);
	idt_setta_entry(43, 0x08, 0x8E, (int)irq11);
	idt_setta_entry(44, 0x08, 0x8E, (int)irq12);
	idt_setta_entry(45, 0x08, 0x8E, (int)irq13);
	idt_setta_entry(46, 0x08, 0x8E, (int)irq14);
	idt_setta_entry(47, 0x08, 0x8E, (int)irq15);
	
	unsigned short i;	// Pulisce l'area di ram rimanente all'idt
	for(i = 48; i < 256; i++) idt_setta_entry(i, 0, 0, 0);
	
	struct DTR idtr; // Variabile di 48bit che verrà caricata nel registro idtr
	idtr.limite = IDT_SIZE - 1;
	idtr.base = IDT_ADDR;
		
	asm("lidtl %0" : : "m" (idtr)); // Carica la nuova idt!
}

void idt_setta_entry(unsigned char i, unsigned short segmento, unsigned char opzioni, int eip)
{
	idt[i].offset_0_15 	= (unsigned int)(eip & 0xFFFF);
	idt[i].selettore 	= segmento;
	idt[i].riservato  	= 0x0;
	idt[i].opzioni 		= opzioni;
	idt[i].offset_16_31 = (unsigned int)((eip >> 16) & 0xFFFF);
}  

