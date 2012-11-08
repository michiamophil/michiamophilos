/**
 * file 	arch/include/x86/mem_map.h
 * autore 	Filippo Bedendo Bordon
 * data 	26-11-11	
 * doc		http://wiki.osdev.org/Memory_Map_(x86)
 */

// Posizione di strutture nella ram < 1 mb
#ifndef x86_memMap
#define x86_memMap

#define IDT_ADDR 0x500
#define IDT_SIZE 256 * 8

// La gdt in memoria stà subito dopo l'idt 
#define GDT_ADDR (IDT_ADDR + IDT_SIZE)
#define GDT_SIZE 3 * 8

// Vettore di int (4 byte) ogni handler, dimensione della idt 
#define HANDLER_ADDR (GDT_ADDR + GDT_SIZE)
#define HANDLER_SIZE 256 * 4	// Int!


// La bitmap è subito dopo la fine del kernel (k_end)

// Buffer temporaneo della tastiera (Circa 0x1118)
#define KEY_BUFFER (HANDLER_ADDR + HANDLER_SIZE)
#define KEY_SIZE 256 * 1 // Byte

// Prossimo indirizzo a (KEY_BUFFER + KEY_SIZE)

// -- Non superare 0x7BFF --
#endif
