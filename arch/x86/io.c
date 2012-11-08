/**
 * file 	arch/x86/io.c
 * autore 	Filippo Bedendo Bordon
 * data 	7-10-11	
 */
 
#include <x86/io.h>

void outb(unsigned short porta, unsigned char valore)
{
    asm volatile ("outb %1, %0" : : "dN" (porta), "a" (valore));
}

unsigned char inb(unsigned short porta)
{
    unsigned char valore;
    asm volatile("inb %1, %0" : "=a" (valore) : "dN" (porta));
    return valore;
}

// Copia in un'area di memoria un valore
void memsetb(unsigned char *destinazione, unsigned char valore, unsigned int lunghezza)
{
	for (; lunghezza != 0; lunghezza--) *destinazione++ = valore;
}

// Copia un'area di memoria in un'altra
void memcpyb(unsigned char *destinazione, const unsigned char *sorgente, unsigned int lunghezza)
{
	// Copia finche il contatore lunghezza > 0
    for(; lunghezza != 0; lunghezza--) *destinazione++ = *sorgente++;	
}

/*
unsigned int leggi_cr0()
{
	unsigned int valore;
	asm volatile("movl %%cr0, %0" : "=r" (valore));
	return valore;
}

unsigned int leggi_cr3()
{
	unsigned int valore;
	asm volatile("movl %%cr3, %0" : "=r" (valore));
	return valore;
}

void scrivi_cr0(unsigned int valore)
{
	asm volatile("movl %0, %%cr0" : : "r" (valore));
}


void scrivi_cr3(unsigned int valore)
{
	asm volatile("movl %0, %%cr3" : : "r" (valore));
}
*/
