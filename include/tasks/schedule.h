/**
 * file 	include/tasks/tasks.h
 * autore 	Filippo Bedendo Bordon
 * data 	05-04-12
**/

#define NEW		0
#define READY	1
#define RUNNING 2
#define ENDED	3
#define BUSY	4

void temp_scheduler();

struct schedule_info{
	unsigned char totCheck;	// Numero di processi validi passati (da 0)
	unsigned char nowCheck;	// Indice del prossimo processo
	unsigned int nSwitch;	// Numero di cambi di contesto fatti fin'ora
};

extern struct schedule_info scheNfo;
