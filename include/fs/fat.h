/**
 * file		include/fs/tmpfs.h
 * autore	Filippo Bedendo Bordon
 * data		14-01-12
**/  

#ifndef fs_FAT
#define fs_FAT

#define DIR_SIZE 32
#define LABEL_SIZE 11
#define FAT_SIZE 8
// Struttura del BootRecord
struct FATbootrecord {
	unsigned char jump[3];
	unsigned char oem_identifier[FAT_SIZE];
	unsigned short byte_per_sector;
	unsigned char sector_per_cluser;
	unsigned short reserved_sectors;
	unsigned char fat_number;
	unsigned short dir_number;
	unsigned short sector_number;
	unsigned char media_descriptor_type;
	unsigned short sectors_per_fat;
	unsigned short sectors_per_track;
	unsigned short heads_on_storage;
	unsigned int hidden_sectors;
	unsigned int extended_sectors;	// Is set only if sector_number == 0
	unsigned char *ext;
}__attribute__((packed));

struct FAT16extendedBootRecord {	
	// Extended Boot Record FAT12-FAT16
	unsigned char disk_number;
	unsigned char NT_reserved;
	unsigned char signature;
	unsigned int volume_id;
	unsigned char label[LABEL_SIZE];
	unsigned char fat_type[FAT_SIZE];
}__attribute__((packed));

struct FAT32extendedBootRecord {
	unsigned int		table_size_32;
	unsigned short		extended_flags;
	unsigned short		fat_version;
	unsigned int		root_cluster;
	unsigned short		fat_info;
	unsigned short		backup_BS_sector;
	unsigned char 		reserved_0[12];
	unsigned char		drive_number;
	unsigned char 		reserved_1;
	unsigned char		signature;
	unsigned int 		volume_id;
	unsigned char		label[LABEL_SIZE];
	unsigned char		fat_type[FAT_SIZE];
}__attribute__((packed));

struct FATdir {
	unsigned char label[LABEL_SIZE];
	unsigned char attributo;
	unsigned char reserved_NT;
	unsigned char creation_time;
	unsigned short ora_creazione;
	unsigned short data_creazione;
	unsigned short data_last_access;
	unsigned short cluster_number16;
	unsigned short time_last_access;
	unsigned short time_last_access2;
	unsigned short cluster_number32;
	unsigned int size;
}__attribute__((packed));;

typedef struct FAT16extendedBootRecord FATEBR16;
typedef struct FAT32extendedBootRecord FATEBR32;
void fat_info(unsigned char *addr);
void fat_list();
#endif
