/**
 * file 	tasks/schedule.c
 * autore 	Filippo Bedendo Bordon
 * data 	25-04-12
**/

#include <tasks/tasks.h>
#include <tasks/schedule.h>

struct schedule_info scheNfo;

void temp_scheduler()
{
	void incrIndex()
	{
		scheNfo.nowCheck++;
		scheNfo.totCheck++;
	}
	
	cur_task = TasksT->TaskT[ TasksT->nowRunning ];	// Task Corrente
	struct task_table *TaskNew;

	//~ Sceglie il nuovo task ~
	// Se dopo il processo corrente ce ne è uno valido lo carichi come TaskNew
	if ( TasksT->TaskT[ scheNfo.nowCheck ] != 0 )
	{
		TaskNew = TasksT->TaskT[ scheNfo.nowCheck ];
		debug_stringa("a");
	}
		
	// Ne ha trovato uno vuoto, controlla se dopo non ce ne sono altri
	else if ( scheNfo.totCheck < TasksT->totTasks )
	{
		scheNfo.nowCheck++;
		debug_stringa("b");
		// Sistema lo stack
		asm("add $0x20, %esp");
		temp_scheduler();
		return;
	}
	
	// Altrimenti riparte dall'inizio
	else
	{
		scheNfo.nowCheck = 0;
		scheNfo.totCheck = 0;
		debug_stringa("c");
		asm("add $0x20, %esp");
		temp_scheduler();
		return;
	}
	
	//~ Controlla se con questo task ci si può fare il CS ~
		
	switch (TaskNew->state)
	{
		case NEW:
			TaskNew->state = READY;
			debug_stringa("d");
			asm("add $0x20, %esp");
			temp_scheduler();
			return;
			
		case READY:
			//~ Salvo il vecchio task
			if ( cur_task->state == RUNNING )
				cur_task->state = READY;
			asm("mov %%esp, %0" : "=r"(cur_task->esp));	// Salva lo stack del task corrente
			
			cur_task = TaskNew;	// Ora questo è il nuovo task
			
			//~ E carico quello nuovo
			TaskNew->state = RUNNING;
			TasksT->nowRunning = scheNfo.nowCheck;	
			incrIndex();
			// Carico il nuovo stack
			asm("mov %0, %%esp" :: "r"(TaskNew->esp + 0x10));
			// EOI
			asm("movb $0x20, %ax	\n"
				"outb %ax, $0x20	\n");
			// Return
			asm("pop %ebx \n"
				"popa	\n"
				"add $8, %esp	\n"
				"sti	\n"
				"iret	\n");
			break;
			
		case RUNNING:
			incrIndex();
			debug_stringa("f");
			asm("add $0x20, %esp	\n");
			temp_scheduler();
			return;
		case BUSY:
			incrIndex();
			debug_stringa("z");
			asm("add $0x20, %esp	\n");
			temp_scheduler();
			return;
	}
}
