/**
 * file 	arch/include/x86/pic.h
 * autore 	Filippo Bedendo Bordon
 * data 	26-11-11	
 */
 
#ifndef x86_pic
#define x86_pic

#define EOI 0x20
#define MASTER_DAT	0X20
#define MASTER_CMD 	0X21
#define SLAVE_DAT	0XA0
#define SLAVE_CMD	0XA1

void init_pic();
void pic_EOI_master();
void pic_EOI_slave();
#endif
