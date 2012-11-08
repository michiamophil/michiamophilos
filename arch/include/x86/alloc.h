/**
 * file		include/x86/alloc.h
 * autore	Filippo Bedendo Bordon
 * data		01-12-11
**/

struct mini_bitmap {
	unsigned int b4_bitmap[32];
	unsigned int b4_page_free;	// Numero di pagine libere
};
