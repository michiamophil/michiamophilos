/**
 * file 	arch/include/x86/video.h
 * autore 	Filippo Bedendo Bordon
 * data 	14-10-11	
 */

#ifndef driver_video_vga
#define driver_video_vga

// Dimensioni dello schermo
#define MAX_X 160 // Lavora con char e non con short (valore reale = 80)
#define MAX_Y 25
#define MAX_SCHERMO (MAX_X * MAX_Y)

// Da 0 a F in ordine crescente
enum{ NERO, BLU, VERDE, CELESTE, ROSSO, VIOLA, MARRONE, BIANCO_SPORCO, GRIGIO, BLU_CHIARO, VERDE_CHIARO, CELESTINO, ARANCIONE, ROSA, GIALLO, BIANCO };

// Funzioni
void video_cancella();
void video_colore(unsigned char colore_caratteri, unsigned char colore_sfondo);
void video_scroll();
void video_scrivi_stringa(char *s);
void video_scrivi_dec(int n);
void video_scrivi_hex(unsigned int n);
#endif
