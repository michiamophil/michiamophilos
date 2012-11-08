/**
 * file		include/shell/shell.h
 * autore	Filippo Bedendo Bordon
 * data		29-01-12
**/

#ifndef shell_shell
#define shell_shell

unsigned char* Cbuffer;
unsigned int keyboard_read_with_output(char *destBuffer);
unsigned int Clenght;

unsigned char argc;	// Numero di argomenti per il comando corrente
// Per argv si puo utilizzare Cbuffer, tanto la dimensione è la stessa (a parte per qualche \0 in piu)
unsigned char *argv[128];
unsigned char *Abuffer;	// Puntatore al buffer dell'argc corrente

struct shell_vesa_info {
	unsigned int x;
	unsigned int y;
	unsigned int forecolor;
	unsigned int backcolor;
	unsigned int max_x;
	unsigned int max_y;
};

struct shell_vesa_info shV;
void init_shell();
void make_args();
void exec_cmd();
#endif
