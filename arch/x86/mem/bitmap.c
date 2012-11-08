/**
 * file 	arch/x86/mem/bitmap.c
 * autore 	Filippo Bedendo Bordon
 * data 	04-12-11
 */

#include <x86/debug.h>
#include <x86/mem.h>


void setP_bitmap(unsigned int page)
{
	unsigned int x = page / 32;
	unsigned int y = page % 32;
	bitmap[x] |= 1 << y;
}

void freeP_bitmap(unsigned int page)
{
	unsigned int x = page / 32;
	unsigned int y = page % 32;
	bitmap[x] ^= 1 << y;
}

char testP_bitmap(unsigned int page)
{
	unsigned int x = page / 32;
	unsigned int y = page % 32;
	return (bitmap[x] >> y);
}

unsigned int find_page(unsigned int n_pages)
{
	// (09/12/11) Giorno memore :D Trovato l'errore: mancava l'AND con 0x1, così da considerare solo un bit!!
	unsigned int  x, n_free_contiguous, page;
	unsigned char y;
	x = y = n_free_contiguous = 0;
	
	// Finchè non trova uno spazio adatto per soddisfare la richiesta di n_pages
	while(n_free_contiguous < n_pages)
	{
		// Se (..) è vera, cioè non è libera allora riazzera la var, se no l'incrementi
		n_free_contiguous = ((bitmap[x] >> y++) & 0x1) ? 0 : n_free_contiguous + 1;
		// Controlla che la y sia < 32 e che non ci sia un'int pieno, altrimenti incrementi x
		if((y == 32) || (bitmap[x] == 0xFFFFFFFF))
		{
			y = 0;
			x++;
		}
		if(x > entry_bitmap) return 0;	// Panico :) n_pagine libere non disponibili... ç_ç
	}

	page = (x * 32) + y;
	page -= n_pages;	// La funzione deve restituire la prima pagina, non l'ultima! :)
	return page;
}

