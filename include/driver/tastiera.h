/**
 * file 	include/driver/tastiera.h
 * autore 	Filippo Bedendo Bordon
 * data 	27-01-12
**/



#ifndef INC_TASTIERA
#define INC_TASTIERA

#include <x86/mem_map.h>

#define IRQ_TASTIERA 33	// E' l'irq1

// La dimensione massima di caratteri che può contenere il buffer è:
// 4kb -> 4096 byte; 1 carattere = 1 byte; MAX_CHAR = 4096
#define KMAX_CHAR 256

#define ESC				1
#define	SHIFT_LEFT		0x2A
#define	SHIFT_RIGHT		0x36
#define LOCK_CAPS		0x3A
#define LOCK_NUM		0x45
#define LED_NUM			2
#define LED_CAPS		4

#define FRECCIA_SU
#define FRECCIA_GIU
#define FRECCIA_DESTRA
#define FRECCIA_SINISTRA

// Porta per lettura e per scrittura
#define P_TASTIERA_LETTURA		0x64
#define P_TASTIERA_SCRITTURA	0x60

typedef struct _flags_t
{
	unsigned char shift		: 1;
	unsigned char control	: 1;
	unsigned char alt		: 1;
	unsigned char capslock	: 1;
	unsigned char numlock	: 1;
	unsigned char esc		: 1;
	unsigned char free		: 2;	// Libero e attualmente non usato
} flags_t;

// Buffer circolare
unsigned char *Kbuffer;	// Dimensione massima 256 caratteri
extern flags_t fl;
unsigned char ctrScrittura;	// Utilizzato dall'handler
unsigned char ctrStato;		// Usato nelle funzioni di lettura del buffer, se maggiore di 0 allora il buffer viene letto
unsigned char ctrLettura;	// Usato nella funzione di lettura, indica la posizione corrente da leggere.


#endif
