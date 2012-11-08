/**
 * file		incude/fs/initrd.h
 * autore	Filippo Bedendo Bordon
 * data		25-12-11	(La notte di Natale :-)
**/

#ifndef fs_initrd
#define fs_initrd

#include <fs/vfs.h>

// Inizio e fine del modulo (informazioni prese da grub)
unsigned int initrd_start;
unsigned int initrd_end;

//* Ctrl+C e Ctrl+V dal file genera_initrd.c :)
#define MAX_CHAR 35	// Per non lasciare spazio sprecato nell'header :)
// Intestazione di ogni file
struct file_header {
	unsigned int posizione;
	unsigned int dimensione;
	unsigned char nome[MAX_CHAR];
	unsigned char magic;	// Attenzione, se si scrive su header[i].nome[35] in poi, si perdono dati!!!
};

struct posix_dir *readdir_initrd(struct f_node *nodo, unsigned int inizio);
#endif
