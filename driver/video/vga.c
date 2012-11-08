/**
 * file		driver/caratteri/video.c
 * autore	Filippo Bedendo Bordon
 * data		29-11-11
 */

#include <driver/video/vga.h>

// Area di memoria della Ram Video
unsigned char *vidmem = (unsigned char *) 0xB8000;
// Si estende per 4000 byte, fino a (0xB8000 + 0xAF0)
// Ho deciso di usare una variabile unica per le coordinate x, y
unsigned short posizione = 0;
// Colore di un carattere (lamp + background + foreground)
unsigned char colore = 0x17;


// Muove il cursore lampeggiante
void muovi_cursore()
{
	unsigned short pos = posizione / 2;
	outb(0x3D4, 14);
	outb(0x3D5, (pos >> 8));
	outb(0x3D4, 15);
	outb(0x3D5, pos);
}


// Pulisce tutto lo schermo
void video_cancella()
{
	// Vado a pulire la Ram Video
	unsigned short i;
	for(i = 0; i < MAX_SCHERMO;)
	{
		vidmem[i++] = 0x20;		// Carattere ascii " "
		vidmem[i++] = colore;	// Attributi relativi al carattere
	}
	// Azzero anche le variabili
	posizione = 0;	// x = y = 0
	muovi_cursore();
}


// Fa lo scroll di 1 riga verso l'alto
void video_scroll() 
{
	unsigned int i;
	// Se sullo schermo c'è stampata più di una riga
	if(posizione > MAX_X / 2)
	{
		// Sposta ogni (riga) alla posizione (riga - 1)
		for(i = 0; i < posizione - MAX_X;)
		{
			vidmem[i] = vidmem[i + MAX_X];
			i++;
		}
		//debug_break();
		// Pulisce l'ultima riga
		for(i = (posizione - MAX_X); i < posizione;)
		{
			vidmem[i++] = 0x20;
			vidmem[i++] = colore;
		}
		posizione -= MAX_X; // La posizione corrente del cursore sarà (riga - 1)
	}
}


void put_char(char c)
{
	// Va a capo
	if (c == '\n') posizione += MAX_X - (posizione % MAX_X);
	else if (c == '\b') vidmem[(posizione -= 2)] = '\0';	// Cancello il carattere precedente
	// Se è un carattere scrivibile lo scrivi
	else if(c >= 0x20)
	{
		vidmem[posizione++] = c;
		vidmem[posizione++] = colore;
	}
	if(posizione >= MAX_SCHERMO) video_scroll();
}


void video_scrivi_stringa(char *s)
{
	int i = 0;
	while(s[i]) put_char(s[i++]);
	muovi_cursore();
}

void video_scrivi_stringa_safe(unsigned char *s, unsigned int i)
{
	unsigned ctr = 0;
	
	while(i--) put_char(s[ctr++]);
	muovi_cursore();
}



void video_scrivi_dec(int n)
{

	if (n == 0)
    {
        video_scrivi_stringa("0");
        return;
    }

    int acc = n;
    char c[32];
    int i = 0;
    while (acc > 0)
    {
        c[i] = '0' + acc%10;
        acc /= 10;
        i++;
    }
    c[i] = 0;
	// Inverte le entry nell'array
    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while(i >= 0) c2[i--] = c[j++];
    video_scrivi_stringa(c2);

}


void video_scrivi_hex(unsigned int n)
{
    int tmp;

    video_scrivi_stringa("0x");

    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4)
    {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0) continue;
    
        if (tmp >= 0xA)
        {
            noZeroes = 0;
            put_char (tmp-0xA+'a' );
        }
        else
        {
            noZeroes = 0;
             put_char( tmp+'0' );
        }
    }
  
    tmp = n & 0xF;
    if (tmp >= 0xA) put_char(tmp-0xA+'a');

    else put_char(tmp+'0');

	// La funzione utilizza anche put_char
	muovi_cursore(); 
}


void video_colora(unsigned char colore_caratteri, unsigned char colore_sfondo, unsigned char lampeggio)
{
	unsigned int i;
	colore = (lampeggio << 7) | (colore_sfondo << 4) | (colore_caratteri & 0x0F);
	
	for(i = 1; i < MAX_SCHERMO * 2; i += 2) vidmem[i] = colore;
}
