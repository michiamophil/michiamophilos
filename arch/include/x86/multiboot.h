#ifndef SP_MULTIBOOT_H
#define SP_MULTIBOOT_H

typedef unsigned char 	     u_char;
typedef unsigned int 	     u_int;
typedef unsigned int 	     u_int32;
typedef unsigned short       u_int16;
typedef signed char          int8_t;
typedef unsigned char        uint8_t;
typedef short                int16_t;
typedef unsigned short       uint16_t;
typedef int                  int32_t;
typedef unsigned             uint32_t;
typedef long long            int64_t;
typedef unsigned long long   uint64_t;
typedef unsigned char        byte;
typedef unsigned short int   word;
typedef unsigned int         dword;
typedef unsigned long long   qword;
typedef unsigned 	     	 size_t;
typedef int32_t				 gid_t;
typedef int32_t				 uid_t;
typedef int32_t				 pid_t;


typedef struct multiboot_module
{
  unsigned long mod_start;
  unsigned long mod_end;
  unsigned long string;
  unsigned long reserved;
} multiboot_module_t;

/**
 * Struttura dell'header multiboot
 */
typedef struct multiboot_info
{
	uint32_t flags;					///< Flag del bootloader
	uint32_t mem_lower;				///< Memoria bassa
	uint32_t mem_upper;				///< Memoria alta
	uint32_t boot_device;			///< Dispositivo di boot
	uint32_t cmdline;				///< Argomenti passati al kernel
	uint32_t mods_count;			///< Grandezza modulo
	uint32_t mods_addr;				///< Indirizzo di ubicazione del modulo
	
	// Elf Info
	uint32_t elf_num;
	uint32_t elf_size;
	uint32_t elf_addr;
	uint32_t elf_shndx;

	uint32_t mmap_length;			
	uint32_t mmap_addr;
	uint32_t drives_length;
	uint32_t drives_addr;
	uint32_t config_table;
	
	uint32_t boot_loader_name;		///< Nome del bootloader
	uint32_t apm_table;				///< Tabella specifiche apm
	uint32_t vbe_control_info;		///< Struttura controllo vesa bios
	uint32_t vbe_mode_info;			///< Struttura info vesa bios
	uint32_t vbe_mode;				///< Modalita vesa bios attiva
	uint32_t vbe_interface_seg;		///< Segmento interfaccia vesa bios
	uint32_t vbe_interface_off;		///< Offset interfaccia vesa bios
	uint32_t vbe_interface_len;		///< Lunghezza interfaccia vesa bios
} __attribute__((packed)) multiboot_info_t;

#endif
