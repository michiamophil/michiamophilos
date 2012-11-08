/**
 * file 	arch/x86/boot/start.c
 * autore 	Filippo Bedendo Bordon
 * data 	19-11-11	
**/

//#include <fs/initrd.h>
#include <fs/tmpfs.h>
#include <fs/fat.h>
#include <x86/mem.h>
#include <x86/tabelle.h>
#include <x86/debug.h>
#include <x86/multiboot.h>
#include <driver/tastiera.h>
#include <driver/video/raw.h>
#include <driver/video/vesa.h>

extern code, data, bss, k_end, root_posizione;
extern *root_node, *dev_node;



void arch_init(struct multiboot_info *grub_inf)
{
	
	video_cancella();
	
	/*#ifdef DEBUG
	video_scrivi_stringa("Arch_init()\n");
	video_scrivi_stringa("Area codice: ");
	video_scrivi_hex(&code);
	video_scrivi_stringa(" -> ");
	video_scrivi_hex(&data);
	video_scrivi_stringa("\nArea dati: ");
	video_scrivi_hex(&data);
	video_scrivi_stringa(" -> ");
	video_scrivi_hex(&bss);
	video_scrivi_stringa("\nArea stack(?): ");
	video_scrivi_hex(&bss);
	video_scrivi_stringa(" -> ");
	video_scrivi_hex(&k_end);
	video_scrivi_stringa("\nFine spazio kernel in ram: ");
	video_scrivi_hex(&k_end);
	video_scrivi_stringa("\n");
	#endif*/

	init_gdt();
	init_idt();
	init_pic();
	init_timer();
	init_tastiera();
	//mouse_init();	// TEST
	

	// Stampa la dimensione della ram
	video_scrivi_stringa("Ram: ");	
	// (ram / 1024) => da kb a mb
	video_scrivi_dec(((grub_inf->mem_lower + grub_inf->mem_upper) / 1024) + 1);
	video_scrivi_stringa(" mb\n");


	// Ricevo le informazioni sulla posizione in ram dell'initrd
	if (grub_inf->mods_count > 0) {	// Solo se grub riesce a caricare l'initrd
		tmpfs_start = *((unsigned int *) grub_inf->mods_addr);	// Il valore del primo elemento
		tmpfs_end = *(unsigned int *)(grub_inf->mods_addr + 4);	// Nell'int successivo (dopo 4byte) c'è la fine del modulo
		// Evito di sovrascrivere l'initrd spostando la fine del kernel alla fine dell'initrd
		kernel_end = (((unsigned int) &k_end) < tmpfs_end) ? tmpfs_end : ((unsigned int) &k_end);
	}


	video_scrivi_stringa("(Tmpfs) Start: ");
	video_scrivi_hex(tmpfs_start);
	video_scrivi_stringa(" - End: ");
	video_scrivi_hex(tmpfs_end);

	init_mem(grub_inf->mem_lower, grub_inf->mem_upper);
	
	if(grub_inf->vbe_control_info != 0) vesa_init(grub_inf->vbe_control_info, grub_inf->vbe_mode_info);
	// ##############(D'ORA IN POI LA PAGINAZIONE E' ATTIVA)#################
	init_paging();
	
	
	//~ // PAGE FAULT TEST: 
	//~ unsigned int *up = (unsigned int *) 0x400000;
	//~ *up = 0x6f;
	//~ unsigned int *uppj = (unsigned int *) 0x402000;
	//~ *uppj = 0x5d;
	//~ unsigned int *upp = (unsigned int *) 0x402050;
	//~ *upp = 0x3a;


	//~ // Inizializza la root (/) dir (( MAGARI IN FUTURO...))
	//~ init_vfs();
	//~ init_initrd();
	//~ video_scrivi_stringa(readdir_fs(root_node,0));video_scrivi_stringa("\n2)");
	//~ video_scrivi_stringa(readdir_fs(root_node,1));video_scrivi_stringa("\n3)");
	//~ video_scrivi_stringa(readdir_fs(root_node,2));

	// Tmpfs
	init_tmpfs();
	

	//~ // Test di lettura caratteri della tastiera:
	//~ unsigned char *Zbuffer = (unsigned char *)0x301000;
	//~ video_scrivi_stringa("keyboard_read(); Prova di lettura caratteri:\n->Immetti 5 caratteri: ");
	//~ keyboard_read(Zbuffer, 5);
	//~ video_scrivi_stringa(Zbuffer);
	
	if(grub_inf->vbe_control_info != 0)
	{		
		unsigned char *buffer = malloc(tmpfs_size("back.raw"));
		tmpfs_read("back.raw", buffer);
		render_raw_image(buffer, 0, 0);
		free(buffer);
		
	}
	
	video_scrivi_stringa("\nTmpfs_read() (Provo a leggere il file \"hello\" dell'initrd:D\n"); 
	unsigned char *buffer = malloc(tmpfs_size("hello"));
	tmpfs_read("hello", buffer);
	video_scrivi_stringa("Output -> ");
	video_scrivi_stringa_safe(buffer, tmpfs_size("hello"));
	free(buffer);
	
	// Prova sul filesystem FAT
	video_scrivi_stringa("\nInfo_FAT filesystem file"); 
	unsigned char *FATbuffer = malloc(tmpfs_size("msdos.fs"));
	tmpfs_read("msdos.fs", FATbuffer);
	fat_info(FATbuffer);
	//free(FATbuffer);

	// Completamente from scratch :)
	init_tasks();
	//test_task();
	
	asm("sti");
	
	video_scrivi_stringa("\nErrore nell'avviare i task");
	
	while(1);
}



/*
void draw_curs(int x, int y)
{
	unsigned char *mouseb = malloc(tmpfs_size("curs"));
	tmpfs_read("curs", mouseb);
	render_raw_image(mouseb, x, y);
	free(mouseb);
	
	
					if (layout_normale[scancode] == 'w')
						draw_curs(mouse_x, (mouse_y-=10));
					else if (layout_normale[scancode] == 'a')
						draw_curs((mouse_x-=10), mouse_y);
					else if (layout_normale[scancode] == 's')
						draw_curs(mouse_x, (mouse_y+=10));
					else if (layout_normale[scancode] == 'd')
						draw_curs((mouse_x+=10), mouse_y);
					
}
*/
