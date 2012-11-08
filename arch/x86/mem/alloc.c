/**
 * file		arch/x86/mem/alloc.c
 * autore	Filippo Bedendo Bordon
 * data		01-12-11
 */

#include <x86/mem.h>
#include <x86/alloc.h>


// Usata prima dell'inizializzazione della bitmap
unsigned int first_alloc(unsigned int dim)
{
	// Ritorna alla funzione chiamante l'inizio dello spazio alloccato
	unsigned int temp = kernel_end;	
	kernel_end += dim;	// Aumenta la posizione della fine del kernel
	return temp;
}

// Stessa funzione, ma controlla che l'indirizzo restituito sia allineato a 4kb
unsigned int first_alloc_align(unsigned int dim)
{
	if(kernel_end % 0x1000)	// Se la divisione ha resto
	{
		kernel_end &= 0xFFFFF000;	// Azzera le prime tre cifre
		kernel_end += 0x1000;		// E aumenta di 1
	}
	unsigned int temp = kernel_end;
	kernel_end += dim;	// Aumenta la posizione della fine del kernel
	return temp;
}


// Allocca n_pages richieste
unsigned int get_page(unsigned int n_pages)
{
	unsigned int i, page = find_page(n_pages);	// Restituisce la prima pagina libera
	if(!page)	// Se find_page() ritorna 0
	{
		video_scrivi_stringa("\nImpossibile soddisfare la richiesta... (get_page()) => Loop");
		debug_stringa("\nImpossibile soddisfare la richiesta... (get_page()) => Loop");
		for(;;);
	}

	// Setta la pagine come occupate nella bitmap
	for(i = 0; i < n_pages; i++) setP_bitmap(page + i);

	// Cerca la prima entry libera del costrutto (Riutilizza i)
	for(i = bitmap_info.curr_index; i < 500; i++) if(bitmap_info.n_pages[i] == 0) break;
	if(i == 500)	// Se il costrutto bitmap_inf è pieno ç_ç
	{
		video_scrivi_stringa("\nBitmap_info piena... => Loop");
		debug_stringa("\nBitmap_info piena... => Loop");
		for(;;);
	}

	// Adesso il nostro nuovo indice sarà i
	bitmap_info.start_page[i] = page;	// Entry della bitmap di partenza
	bitmap_info.n_pages[i] = n_pages;
	// Richiamando un'altra volta la funzione get_page(), nel costrutto si partirà a cercare da i + 1
	bitmap_info.curr_index = i + 1;

	return page*0x1000;	// Ritorna l'indirizzo fisico
}


// Deallocca n_pagine, partendo dalla prima, passata come argomento
void free_page(unsigned int fis_addr)
{
	// Contiene l'entry corrispondente all'indirizzo da liberare
	unsigned int page = fis_addr / 0x1000;
	unsigned char i = 0;	// Indice
	unsigned int j;	// Contatore

	short noLoop = 500;
	// Cerca la pagina nel costrutto (o meglio, cerca l'indice)
	while(1)
	{
		if(bitmap_info.start_page[i] == page) break;	// Ho trovato l'indice, cioè i ed esco
		i++;
		if( noLoop-- == 0)
		{
			video_scrivi_stringa("Call: free_page(");
			video_scrivi_hex(fis_addr);
			video_scrivi_stringa(") -> Index not found!\n");
			return;
		}
	}

	// Libera le pagine dalla bitmap
	for(j = 0; j < bitmap_info.n_pages[i]; j++) freeP_bitmap(page + j);

	// Ora, se non azzero curr_index succederanno malanni, 
	// Ma siccome ad ogni get c'è anche poco dopo un free non ha tanto senso tenere 
	// Questa variabile che sarebbe uguale quasi sempre a 0
	// Il compromesso è: se i che azzero è maggiore di curr_index lascio il valore
	// Se è minore o uguale allora curr_index = i
	bitmap_info.curr_index = (i > bitmap_info.curr_index) ? bitmap_info.curr_index : i;

	bitmap_info.start_page[i] = bitmap_info.n_pages[i] = 0; // Pulisce il costrutto
}

// Molto spartane :(
unsigned int malloc(unsigned int size)
{
	if ( !size ) return 0;
	unsigned int n_pages = (size % 0x1000) ? ((size / 0x1000) + 1) : (size / 0x1000);
	unsigned int ptr = get_page(n_pages);
	return ptr;
}

void free(unsigned int ptr)
{
	free_page(ptr);
}


/*// Butto giù un po' di codice per il malloc (spero di appoggiarmi il più possibile a get_page()...)
unsigned int malloc(unsigned int size)
{
	//* Sicuramente questo codice sarà da migliorare in futuro...
	// Tengo una parte di ogni pagina (su 4096 byte ne tengo 128)
	// La parte iniziale funge da bitmap con pagine da 4 byte
	// 4byte ogni 32 pagine(che sono in totale 1024, cioè 4096 / 4)
	// Significa che 1024/32 = 32 int, cioè 128byte
	unsigned int n_pages = (size % 4) ? (size / 4) + 1 : (size / 4); // Allocco n pagine da 4 byte

	// QUESTO SOLO SE DEVO RICHIEDERE UNA NUOVA k4_page
	unsigned int *k4_page = (unsigned int *) get_page(1); // Allocco una nuova pagina da 4kb
	struct mini_bitmap m_bitmap;	// Inizializzo la mini bitmap
	m_bitmap.b4_page_free = 992; // (1024 - 32) Numero di pagine iniziali disponibili (DA OTTIMIZZARE :P)
	// Ci riservo lo spazio per la mini bitmap iniziale
	*k4_page = m_bitmap;
	// Allocco le 32 pagine per la bitmap (INUTILE :)
	

	return ptr;
}
*/
