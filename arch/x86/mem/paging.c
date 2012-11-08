/**
 * file 	arch/x86/mem/paging.c
 * autore 	Filippo Bedendo Bordon
 * data 	16-12-11
 */

#include <x86/paging.h>
#include <driver/video/vesa.h>

unsigned int *pageDir;
unsigned int *pageTable;
unsigned int *pageTable2;

void init_paging()
{
	unsigned int i;
	unsigned int fis_addr = 0;

	// Con 1 pageTable copro 4 mega
	pageDir = first_alloc_align(0x1000);	// La page dir si trova dopo k_end (allineato a 4kb)
	pageTable = first_alloc_align(0x1000);	// La pageTable invece è dopo la pageDir
	pageTable2 = first_alloc_align(0x1000);	// La pageTable invece è dopo la pageDir

	// Scrivo a schermo gli indirizzi
	video_scrivi_stringa("\nAddr pageDir: ");		//DEBUG
	video_scrivi_hex(pageDir);						//DEBUG
	video_scrivi_stringa("\nAddr pageTable: ");		//DEBUG
	video_scrivi_hex(pageTable);					//DEBUG

	// Popola la pageTable :)
	for(i = 0; i < 1024; i++)
	{
		pageTable[i] = fis_addr | 3;	// 3 = 011b (ring 0, r/w, presente)
		fis_addr += 0x1000;
	}
	
	// Anche la 2 :D
	for(i = 0; i < 1024; i++)
	{
		pageTable2[i] = fis_addr | 3;	// 3 = 011b (ring 0, r/w, presente)
		fis_addr += 0x1000;
	}
	
	// La prima entry della pageDir è la pageTable settata prima
	pageDir[0] = pageTable;
	pageDir[0] = pageDir[0] | 3;
	
	pageDir[1] = pageTable2;
	pageDir[1] = pageDir[1] | 3;

	// Finisce di riempire la pageDir con pagine non presenti (causeranno un page fault)
	for(i = 2; i < 1024; i++) pageDir[i] = 0 | 2;	// 2 = 010b (ring 0, r/w, non presente)

	// Questa riga riguarda gli indirizzi ricorsivi (???Se ho capito bene???)
	pageDir[1023] = pageDir[0];

	// Mappo l'area per la modalità vesa
	if ( IsvesaOn )
	{
		unsigned int *pageTable;
		unsigned int x, y;
		unsigned int vesaddr = vmi->phys_base_ptr;
		// Panico...
		if(vesaddr % 0x1000)
		{
			debug_stringa("(Indirizzo fisico memoria vesa) % 0x1000 != 0");
			for(;;);
		}
		
		x = (vesaddr / 0x1000) / 1024;
		pageTable = get_page(1);
		pageDir[x] = (unsigned int)pageTable | 1; //(vesaddr / 0x1000) / 1024;
		
		y = (vesasize / 0x1000) % 1024;
		for(i = 0; i < y; i++)
		{
			pageTable[i] = vesaddr | 3;	// 3 = 011b (ring 0, r/w, presente)
			vesaddr += 0x1000;
		}
	}

	unsigned int cr0;
	// Carica nel registro cr3 la pagedir corrente
	asm("mov %0, %%cr3":: "r"((unsigned int)pageDir));
	// E abilita il paging!
	asm("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000; // Setta il bit31
	asm("mov %0, %%cr0":: "r"(cr0));

	// Attiva la gestione dei page fault
	installa_handler(14, page_fault);
}

void page_fault(unsigned int error)	// Prelevato dallo stack
{
	debug_stringa("Page_fault()\n");
	
	unsigned int cr2, cr3, x, y;// Offset. x da pageDir, y da pageTable
	unsigned int *pageDir, *pageTable, *page;	// La pageDir corrente (da CR3)

	video_scrivi_stringa("\nEccezione(14): Page fault ");
	asm("movl %%cr2, %0": "=r"(cr2));	// Copio il valore di cr2 nella var
	video_scrivi_stringa("\nCR2: ");
	video_scrivi_hex(cr2);	// E stampo il suo contenuto
	video_scrivi_stringa("\nERROR: ");
	video_scrivi_hex(error);


	// Traduco il valore di CR2 nella rispettiva pageDir e pageTable
	x = (cr2 / 0x1000) / 1024;
	y = (cr2 / 0x1000) % 1024;

	// Prendo l'indirizzo della pageDir corrente dal registro CR3
	asm("movl %%cr3, %0": "=r"(cr3));
	pageDir = ((cr3 >> 12) * 0x1000);


	// Se non è mappata
	if (!(pageDir[x] >> 12)) {
	pageTable = get_page(1);	// Prende una pagina da 4kb, pageTable contiene il suo indirizzo in ram
	pageDir[x] = (unsigned int)pageTable | 1;	// La pageDir[x] è mappata e punta a pageTable
	} else {	// Se no prende l'indirizzo dalla pageDir
		pageTable = (pageDir[x] >> 12) * 0x1000;
	}

	page = get_page(1);	// La pagina fisica
	pageTable[y] = 	(unsigned int)page | 3;	// La pageTable[y] è mappata e punta a page

	// Ora ho una pagina libera da 4kb (page) e le relative pageDir e la pageTable con il bit presente abilitati e
	// Posso terminare la gestione del page fault
}
