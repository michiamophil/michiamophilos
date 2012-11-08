/**
 * file		fs/initrd.c
 * autore	Filippo Bedendo Bordon
 * data		27-12-11
**/

#include <fs/initrd.h>
#include <fs/vfs.h>

extern *root_node;
unsigned char n_file;
struct file_header *header;
struct posix_dir readdir_dir;

unsigned char init_initrd(unsigned int indr)
{
	// Il primo byte dell'initrd contiene il numero dei file presenti nel fs
	n_file = *(unsigned char *) initrd_start;
	header = (struct file_header *) (initrd_start + 1); // Definisco gli header per il fs
}


struct posix_dir* readdir_initrd(struct f_node *nodo, unsigned int indice)
{
	video_scrivi_stringa("\nReaddir_initrd()\n");		// DEBUG
	memsetb(&readdir_dir, 0, sizeof(struct posix_dir));	// Pulisce l'area della dir

	if (( !indice ) && ( nodo == root_node ))
	{
		strcpy(readdir_dir.name, "dev");
		readdir_dir.inode = 0;
		return &readdir_dir;
	}
	
	// Ritorna 0 se supero il limite massimo di file presenti nell'initrd
	if ((indice - 1) >= n_file)
		return 0;

	strcpy(readdir_dir.name, header[indice-1].nome);
	return &readdir_dir;
}
