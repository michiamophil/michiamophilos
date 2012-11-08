/**
 * file 	arch/include/x86/io.h
 * autore 	Filippo Bedendo Bordon
 * data 	21-11-11	
 */

#ifndef x86_io
#define x86_io

void outb(unsigned short porta, unsigned char valore);
unsigned char inb(unsigned short porta);
void memset(unsigned char *destinazione, unsigned char valore, unsigned int lunghezza);
void memcpyb(unsigned char *destinazione, const unsigned char *sorgente, unsigned int lunghezza);

#endif
