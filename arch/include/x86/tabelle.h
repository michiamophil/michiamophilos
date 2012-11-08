/**
 * file 	arch/include/x86/tabelle.h
 * autore 	Filippo Bedendo Bordon
 * data 	22-11-11	
 */

#ifndef x86_tabelle
#define x86_tabelle


struct DTR
{
	unsigned short 	limite;
	unsigned int	base;
}__attribute__((packed));

struct idt_entry
{
	// Primi 32 bit
	unsigned short offset_0_15;
	unsigned short selettore;
	// Ultimi 32 bit
	unsigned char riservato; // La parte disponibile non è utilizzata (per ora :)
	unsigned char opzioni;   // 1xP - 2xDPL - 0 - 1xD - 110
	unsigned short offset_16_31;
} __attribute__((packed));

void init_gdt();
void init_idt();
void idt_setta_entry(unsigned char i, unsigned short segmento, unsigned char opzioni, int eip);
#endif
