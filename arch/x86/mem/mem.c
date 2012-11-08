/**
 * file 	arch/x86/mem/mem.c
 * autore 	Filippo Bedendo Bordon
 * data 	01-12-11
 */

#include <x86/debug.h>
#include <x86/mem.h>
#include <x86/mem_map.h>



// °°DA FARE:°°
//	°Rendere bitmap_info dinamico
//	°Vedi FIXME

void init_mem(unsigned long bassa, unsigned long alta)
{
	unsigned int i, pages_res;	// Contatore
	unsigned long long tot_ram = bassa + alta;	// in Kbyte
	// Se un bit di un entry (int) = 4 Kbyte
	// Io ho il totale della ram in Kbyte, divido per 4 e trovo il numero delle pagine
	// Divido ancora per 32 perchè un int contiene 32 bit (8 bit * 4 Byte)
	// E trovo il numero di int da alloccare
	entry_bitmap = (tot_ram / 4) / 32;

	// Se la divisione ha resto allocchi una pagina in più
	// Per ora è meglio non avere massimo 31 pagine nella bitmap, che restituire puntatori a indirizzi fuori dai limiti
	// ##FIXME settare una var max_page## per risolvere il problema##
	// entry_bitmap = ((tot_ram / 4) % 32) ? entry_bitmap++ : entry_bitmap;

	bitmap = (unsigned int) first_alloc(sizeof(int) * entry_bitmap);
	video_scrivi_stringa("\nPosizione bitmap: ");
	video_scrivi_hex(bitmap);

	// Pulisce l'area della bitmap
	// ##FIXME: Modificare che parta da sopra i primi 2 Mb e finisca 2 Mb prima. (usare il cast nel primo argomento !!)##
	memsetb(bitmap, 0x00, (entry_bitmap * 4));

	
	pages_res = ((kernel_end / 0x1000) / 32) + 3;
	for(i = 0; i < pages_res; i++) bitmap[i] = 0xFFFFFFFF;


}


