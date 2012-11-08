/**
 * file		fs/vfs.c
 * autore	Filippo Bedendo Bordon
 * data		25-12-11 (La sera di Natale :-)
**/

#include <fs/vfs.h>
#include <fs/initrd.h>


unsigned int root_posizione;
struct f_node *root_node;
struct f_node *dev_node;

// Definisco il nodo fondamentale
void init_vfs()
{
	root_posizione = malloc(sizeof(struct f_node));	// Malloc() scandalosa D:, allocca comunque 4kb...
	// Alla posizione contenuta nella variabile root_node è definita la struttura root_node
	root_node = (struct f_node *) root_posizione;
	memsetb(root_node, 0, sizeof(struct f_node) * 2);	// Pulisco l'area riservata ai nodi (root e dev)

	// Inizializzo il nodo (/):
	strcpy(root_node->nome, "rootdir");
	root_node->attributi = ATTR_DIR;	// E' una directory
	root_node->readdir = (unsigned int)readdir_initrd;

	// Definisco la dir /dev
	dev_node = (struct f_node *) root_posizione + sizeof(struct f_node);	// Punta appena dopo il root_node (usufruisce dei 4kb :()
	strcpy(dev_node->nome, "dev");
	dev_node->attributi = ATTR_DIR;
	dev_node->readdir = (unsigned int)readdir_initrd;

}

unsigned int read_fs(struct f_node *nodo, unsigned int offset, unsigned int dimensione, unsigned char *buffer)
{
	// Se è stata settata la funzione per gestire nodo la chiamo
	if (nodo->read)
		return nodo->read(nodo, offset, dimensione, buffer);
	else 
		return 0;
}

unsigned int write_fs(struct f_node *nodo, unsigned int offset, unsigned int dimensione, unsigned char *buffer)
{
	if (nodo->write)
		return nodo->write(nodo, offset, dimensione, buffer);
	else
		return 0;
}

struct posix_dir *readdir_fs(struct f_node *nodo, unsigned int inizio)
{
	if (((nodo->attributi & 0x7) == ATTR_DIR) && nodo->readdir)
		return nodo->readdir(nodo, inizio);
	else
		return 0;
}
