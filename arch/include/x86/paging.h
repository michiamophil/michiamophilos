/**
 * file 	arch/include/x86/paging.h
 * autore 	Filippo Bedendo Bordon
 * data 	16-12-11
 */

#ifndef x86_paging
#define x86_paging

#define PD_LIMIT 0x400
#define PT_LIMIT 0x400

#define PD_PRESENT 0x01 

#define WRITE 0x02
#define READ 0

#define SUPERVISOR 0
#define P_USER 0x04

#define WT_ENABLE 0x08

#define CACHE_DISABLE 0x10
#define ACCESSED 0x20

#define DIRTY 0x01

#define PF_MASK 0x03

struct page_s
{
	unsigned int reserved1	: 3;
	unsigned int pwt		: 1;	// Page-level write-through
	unsigned int pcd		: 1;	// Page-level cache disable
	unsigned int reserved0	: 7;
	unsigned int fis_addr	: 20;
};
typedef struct page_s page;

void page_fault();

#endif
