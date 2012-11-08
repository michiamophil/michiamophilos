/**
 * file		fs/fat.c
 * autore	Filippo Bedendo Bordon
 * data		08-09-12
 * info		Supporto minimale al filesystem FAT
**/  

#include <fs/fat.h>
struct FATdir *root_dir;
struct FATbootrecord *FATBR;
unsigned char *ext;
unsigned short bs, fs, rs, cn;
unsigned char sc, fat_type;
unsigned int fds;

void fat_info(unsigned char *addr)
{
	FATBR = (unsigned int)addr;
	
	// Definisco le variabili più usate
	bs = FATBR->byte_per_sector;
	sc = FATBR->sector_per_cluser;
	fs = FATBR->fat_number * FATBR->sectors_per_fat;	// Settori riservati alle FAT
	rs = (FATBR->dir_number * DIR_SIZE) / bs;	// Settori riservati alla root_dir
	cn = (FATBR->sector_number - (FATBR->reserved_sectors + fs + rs)) / sc;	// Numero totale di cluster (riservati ai dati)
	fds = ((FATBR->reserved_sectors + fs + rs) * bs) + (unsigned int)addr;
	
	// A seconda del numero di cluster, definisco il tipo di FAT
	if (cn < 4085)
		fat_type = 12;
	else
	{
		if (cn < 65525)
			fat_type = 16;
		else
			fat_type = 32;
	}
	
	// Definisco la root_dir, a seconda del tipo di fat
	if (fat_type == 12 || fat_type == 16)
	{
		ext = (FATEBR16*)&FATBR->ext;
		root_dir = (unsigned int)((FATBR->reserved_sectors + fs) * bs + addr);
	} else {
		ext = (FATEBR32*)&FATBR->ext;
		root_dir = (unsigned int)(((((FATEBR32*)ext)->root_cluster) * sc * bs) + addr);
	}
	
	// Test sull'allineamento dell'extended boot record:
	if (((FATEBR16*)ext)->signature != 0x28 && ((FATEBR16*)ext)->signature != 0x29)
	{
		video_scrivi_stringa("\n***FAT->Abort - Extended boot loader non allineato***");
		return;
	}
	
	video_scrivi_stringa("\n - Lettura del primo file presente in root: ");
	video_scrivi_stringa_safe(root_dir[1].label, LABEL_SIZE);
}

void fat_list()
{
	video_scrivi_hex(root_dir);
	video_scrivi_stringa(" - Lista file presenti nel FS fat: ");
	unsigned int i = 0;
	while(*(root_dir[i]).label != 0)
	{
		if (*(root_dir[i]).label == 0x41 || *(root_dir[i]).label == 0xe5)	// Long file name (vfat) || file eliminato
		{
			i++;
			continue;
		}
		video_scrivi_stringa("\n - ");
		video_scrivi_stringa_safe(root_dir[i].label, LABEL_SIZE);
		video_scrivi_stringa(" - Size: ");
		video_scrivi_hex(root_dir[i].size);
		i++;
	}
}



void fat_fileread(char *nome, unsigned char *buffer)
{
	unsigned int i = 0;
	while(*(root_dir[i]).label != 0)
	{
		if (*(root_dir[i]).label == 0x41 || *(root_dir[i]).label == 0xe5)	// Long file name (vfat) || file eliminato
		{
			i++;
			continue;
		}
		if (strcmpFILE(nome, root_dir[i].label) )
		{
			unsigned int *sorgente = (unsigned int*)(((root_dir[i].cluster_number32 - 2) * sc * bs) + fds);
			memcpyb(buffer, sorgente, root_dir[i].size);
			return root_dir[i].size;
		}
		i++;
	}
	return 0;
}

unsigned int fat_filesize(char *nome)
{
	unsigned int i = 0;
	while(*(root_dir[i]).label != 0)
	{
		if (*(root_dir[i]).label == 0x41 || *(root_dir[i]).label == 0xe5)	// Long file name (vfat) || file eliminato
		{
			i++;
			continue;
		}
		if (strcmpFILE(nome, root_dir[i].label) )
		{
			return root_dir[i].size;
		}
		i++;
	}
	return 0;
}

void fat_fileout(char *nome)
{
	unsigned int filesize = fat_filesize(nome);
	unsigned char *filebuffer = malloc(filesize);
	fat_fileread(nome, filebuffer);
	video_scrivi_stringa_safe(filebuffer, filesize);
	free(filebuffer);
}
void fat_init(unsigned char *addr)
{
	struct FATbootrecord *FATBR = (unsigned int)addr;
	
}

//~ video_scrivi_stringa_safe(FATBR->fat_type, 8);
	//~ video_scrivi_stringa("\n - Is bootable? ");
	//~ if (FATBR->bootable_signature == 0xAA55)
		//~ video_scrivi_stringa("Yes");
	//~ else
		//~ video_scrivi_stringa("No");
	//~ video_scrivi_stringa("\n - Bytes per sector: ");
	//~ video_scrivi_hex(FATBR->byte_per_sector);
	//~ video_scrivi_stringa("\n - Sectors per cluster: ");
	//~ video_scrivi_hex(FATBR->sector_per_cluser);
	//~ video_scrivi_stringa("\n - Etichetta del filesystem: ");
	//~ video_scrivi_stringa_safe(FATBR->label, 11);
	//~ video_scrivi_stringa("\n - Reserved sectors: ");
	//~ video_scrivi_hex(FATBR->reserved_sectors);
	//~ video_scrivi_stringa("\n - Root entry count: ");
	//~ video_scrivi_hex(FATBR->dir_number);
	//~ video_scrivi_stringa("\n - Numero di FAT: ");
	//~ video_scrivi_hex(FATBR->fat_number);
	//~ video_scrivi_stringa("\n - Numero di settori: ");
	//~ video_scrivi_hex(FATBR->sector_number);
	//~ video_scrivi_stringa("\n - Dimensione fat: ");
	//~ video_scrivi_hex(FATBR->sectors_per_fat);
	//~ 
	//~ 
	//~ // Inizio test
	//~ struct FATdir *root_dir = (unsigned int)(((FATBR->reserved_sectors + (FATBR->fat_number * FATBR->sectors_per_fat)) * FATBR->byte_per_sector) + addr);
	//~ unsigned int first_data_sector = (FATBR->dir_number * DIR_SIZE) + ((FATBR->reserved_sectors + (FATBR->fat_number * FATBR->sectors_per_fat)) * FATBR->byte_per_sector);
	//~ video_scrivi_stringa("\n - Lettura del primo file presente in root: ");
	//~ video_scrivi_stringa_safe(root_dir[1].label, 7);
	//~ video_scrivi_stringa("\n -* Cluster del relativo file: ");
	//~ video_scrivi_hex(root_dir[1].cluster_number32 - 2);
	//~ video_scrivi_stringa("\n -* Calcolo della posizione del file in ram: ");
	//~ unsigned int *buffer_file = (unsigned int) ((root_dir[1].cluster_number32 - 2)* (FATBR->sector_per_cluser * FATBR->byte_per_sector)) + first_data_sector + addr;
	//~ video_scrivi_hex(first_data_sector); video_scrivi_hex(root_dir);
	//~ video_scrivi_stringa("\n -* Provo a fare un print dei dati a quella posizione:\n");
	//~ debug_break();
	//~ video_scrivi_stringa_safe(buffer_file, root_dir[1].size);
	//~ video_scrivi_stringa("\n*** Exit ***\n\n");
