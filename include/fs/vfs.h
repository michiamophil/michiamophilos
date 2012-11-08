/**
 * file		include/fs/vfs.h
 * autore	Filippo Bedendo Bordon
 * data		25-12-11 (La sera di Natale :-)
**/

#ifndef fs_vfs
#define fs_vfs

#define NOME_LENGTH 64

// Attributi da assegnare ai nodi
#define ATTR_FILE 0x01
#define ATTR_DIR 0x02
#define FS_MOUNTPOINT 0x03

struct f_node;
struct posix_dir;

typedef unsigned int (*read_f)(struct f_node*, unsigned int, unsigned int, unsigned char*);
typedef unsigned int (*write_f)(struct f_node*, unsigned int, unsigned int, unsigned char*);
typedef struct posix_dir *(*read_d)(struct f_node*, unsigned int);

// La struttura di un file generico
struct f_node {
	unsigned char nome[NOME_LENGTH];
	unsigned int uid;
	unsigned int gid;
	unsigned int attributi;	// File, directory, ecc.
	unsigned int dimensione;
	read_f read;
	write_f write;
	read_d readdir;
	//(*read_dir)(char *) read_dir;
};

struct posix_dir {
	char name[128];
	unsigned int inode;
};

#endif
