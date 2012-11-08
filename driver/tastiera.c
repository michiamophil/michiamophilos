/**
 * file 	driver/caratteri/tastiera.c
 * autore 	Filippo Bedendo Bordon
 * data 	27-01-12
 */

#include <driver/tastiera.h>

flags_t fl;	// Contiene le variabili booleane di stato
unsigned int mouse_x = 512;
unsigned int mouse_y = 300;

char layout_normale[] = 
{
		0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '\'', 0, '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',   
		0, 0, '\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 0, 0, 0, 0, 0, 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', 
		'-', 0, 0, 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'U', 0,'-', 'S', '5', 'D', '+', 0, 'G', 0, 0, 0, 0, 0, 0, 0, 0 
};

char layout_shifted[] = 
{
		0, 0, '!', '"', 0, '$', '%', '&', '/', '(', ')', '=', '?', '^', 0, '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[',
		']', '\n', 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', '@', '#', 0, 0, '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ';', ':', '_', 0,
		0, 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,'-', 0, '5', 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 
};

void tastiera_imposta_luci(unsigned char stato)
{
 	while((inb(0x64)&2)!=0);
 	outb(0x60,0xED);
 
 	while((inb(0x64)&2)!=0);
 	outb(0x60,stato);
 }


void tastiera_handler(void)
{
	unsigned char scancode = inb(0x60);
	
	// E' stato rilasciato un tasto
	if(scancode & 0x80)
	{
		scancode &= 0x7F;
		switch(scancode)
		{
			case SHIFT_LEFT:	// Rilascia lo shift
			case SHIFT_RIGHT:
				fl.shift = 0;
				break;
		}
	}
	
	// E' stato premuto un tasto
	else
	{
		switch(scancode)
		{
			case LOCK_CAPS:	// Attiva o disattiva il capslock
				fl.capslock = !fl.capslock;
				tastiera_imposta_luci(LED_CAPS | LED_NUM);
				break;
				
			case LOCK_NUM:	// Attiva o disattiva il numlock
				fl.numlock = !fl.numlock;
				tastiera_imposta_luci(LED_CAPS | LED_NUM);
				break;
				
			case SHIFT_LEFT:// Attiva lo shift
			case SHIFT_RIGHT:
				fl.shift = 1;
				break;
			//// Freccie, vedi doc/tasti direzione
			//case 
			// Se è premuto il tasto esc termina la shell (vedi idle)
			case ESC:
				fl.esc = 1;
				break;
			
			
			default:		// Altrimenti è un carattere e lo scrive nel buffer
				if((fl.shift) || (fl.capslock))
					Kbuffer[ctrScrittura++] = layout_shifted[scancode];
				else
					Kbuffer[ctrScrittura++] = layout_normale[scancode];
				ctrStato++;
				break;
		}
	}
}


unsigned int init_tastiera()
{
	Kbuffer = (unsigned char *)KEY_BUFFER;
	ctrScrittura = ctrLettura = ctrStato = 0;
	installa_handler(IRQ_TASTIERA, (unsigned int)tastiera_handler);	// Attiva la gestione dell'handler
}

unsigned char get_char()
{
	char c;
	if ( ctrStato > 0 ) // Leggo il buffer
	{
		c = Kbuffer[ctrLettura++];
		ctrStato--;
		return c;
	} else
		return 0;
}

unsigned int keyboard_read(char *destBuffer, unsigned int nChar)
{
	unsigned int ctr = 0;
	unsigned char c;
	while (ctr < nChar)
	{
		c = get_char(); 
		if (c != 0)
			destBuffer[ctr++] = c;
	}
	return ctr;	// Numero di caratteri letti
}
