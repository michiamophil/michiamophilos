/**
 * file 	fs/genera_initrd.c
 * autore 	Filippo Bedendo Bordon
 * data 	30-01-12
**/

#include <stdio.h> 
// Genera la pixmap iniziale per snake


void main()
{
	unsigned int i = 0;
	unsigned short valore;

	FILE *pixmap = fopen("./pixmap.sk", "w");
	// Allocca la dimensione di uno screen, cioè 0x1000
	unsigned short *screen = (unsigned short *) malloc(0x1000);

	// Pulisco l'area della ram
	memset(screen, 0x00, 0x1000);

	// Ora ci disegno sopra:
	// -> La prima riga è composta da una barra con i punti e il livello (sfondo nero, testo bianco)
	for ( i = 0; i < 80; i++ )
	{
		screen[i] = 0x0F20;	// 20 è ' ', 0F è il colore di sfondo
	}
	
	screen[80] = 0x06C9;	// Cornice TOP-LEFT
	for ( i = 81; i < 159; i++ )
	{
		screen[i] = 0x06CD;	// Cornice TOP
	}
	screen[159] = 0x06BB;	// Cornice TOP-RIGHT

	// Cornice LEFT
	for ( i = 2; i < 24; i++ )
	{
		screen[i*80] = 0x06BA;
	}
	screen[24*80] = 0x06C8;	// Cornice BOTTOM-LEFT

	// Cornice BOTTOM
	for ( i = 24*80; i < 24; i++ )
	{
		screen[i*80] = 0x06BA;
	}

	fwrite(screen, 0x1000, 1, pixmap);
	fclose(pixmap);
	free(screen);
}
