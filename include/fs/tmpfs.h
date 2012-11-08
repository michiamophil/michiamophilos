/**
 * file		include/fs/tmpfs.h
 * autore	Filippo Bedendo Bordon
 * data		14-01-12
**/  

#ifndef fs_tmpfs
#define fs_tmpfs

// Posizioni del fs caricato da grub in ram
unsigned int tmpfs_start;
unsigned int tmpfs_end;

// Variabili globali sulla struttura del fs
unsigned char tmpfs_nFile;
struct file_header *tmpfs_Header;

// Spazio degli indirizzi per ogni file
#define MAX_CHAR 35
struct file_header {
	unsigned int posizione;
	unsigned int dimensione;
	unsigned char nome[MAX_CHAR];
	unsigned char magic;
};


#endif
