/**
 * file 	arch/x86/pic.c
 * autore 	Filippo Bedendo Bordon
 * data 	26-11-11	
 */

#include <x86/pic.h>

// Rimappa il pic
void init_pic()
{
	outb(MASTER_DAT, 0x11);
    outb(SLAVE_DAT, 0x11);
    
    outb(MASTER_CMD, 0x20);
    outb(SLAVE_CMD, 0x28);
    
    outb(MASTER_CMD, 0x04);
    outb(SLAVE_CMD, 0x02);
    
    outb(MASTER_CMD, 0x01);
    outb(SLAVE_CMD, 0x01);
    
    // Abilita tutte le linee del pic (?Penso sia temporaneo?)
    outb(MASTER_CMD, 0x00);
    outb(SLAVE_CMD, 0x00);	
}

void pic_EOI_master()
{
	outb(MASTER_DAT, EOI);
}

void pic_EOI_slave()
{
	outb(SLAVE_DAT, EOI);
}
