/**
 * file 	arch/include/x86/mem.h
 * autore 	Filippo Bedendo Bordon
 * data 	30-11-11
 */

#ifndef x86_mem
#define x86_mem

unsigned int *bitmap;
unsigned int entry_bitmap;		// Numero di int della bitmap
struct bitmap_inf bitmap_info;
extern k_end;					// Fine del kernel (vedi first_alloc())
unsigned int kernel_end; // = (unsigned int) &k_end;	// A noi interessa la posizione in memoria di k_end, non il suo valore

struct bitmap_inf	// Provvisorio
{
	unsigned short curr_index;	// curr_index: Da partire per settare una nuova pagina
	unsigned int start_page[1000];
	unsigned int n_pages[1000];
};
#endif
