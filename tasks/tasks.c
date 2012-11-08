/**
 * file 	tasks/tasks.c
 * autore 	Filippo Bedendo Bordon
 * data 	05-04-12
**/

#include <tasks/tasks.h>
#include <tasks/schedule.h>
#include <x86/interrupt.h>
#include <driver/tastiera.h>
#include <shell/shell.h>

struct tasks_table *TasksT;
struct task_table *cur_task;

// Restituisce un PID libero (<256)
unsigned int setPid()
{
	// Solo perchè è un char.. comunque è possibile un loop totale se l'array è tutto pieno..
	while ( TasksT->TaskT[ TasksT->totPid ] != 0 )
		TasksT->totPid++;
	return TasksT->totPid;
}

// Rimuove il PID e di conseguenza il processo
unsigned int freePid(unsigned int pid)
{
	if( TasksT->TaskT[ pid ] != 0 )
	{
		TasksT->TaskT[ pid ] = 0;
		return 0;
	}
	return 1;
}

void setPriority(unsigned char pid, unsigned char priority)
{
	struct task_table *TaskNew = TasksT->TaskT[ pid ];
	//if ( TaskNew )
		TaskNew->Qmax = priority;
}
void suicide();
// Crea un nuovo task
unsigned int make_task(unsigned int eip)
{
	asm("cli");
	struct task_table *TaskNew = get_page(1);
	memsetb( TaskNew, 0x00, sizeof *TaskNew );
	
	// Il task è settato come new (da memsetb)
	TaskNew->pid = setPid();
	
	TaskNew->eip = eip;
	TaskNew->state = READY;
	TaskNew->esp = TaskNew->stackPage = get_page(1);
	TaskNew->esp += 0x1000;	// Parte dalla fine
	// Quanto di tempo, per ora non utilizzato
	TaskNew->Qmax = TaskNew->Qattuale = 0x5;
	
	// Aggiunge il task alla tabella generale
	TasksT->TaskT[ TaskNew->pid ] = TaskNew;
	TasksT->totTasks++;	// Numero dei task presenti
	
	// Crea lo stack e lo prepara per la prima esecuzione
	unsigned int *stack;
	stack = TaskNew->esp -= sizeof (reg);	// Punta alla cima dello stack
	memsetb( stack, 0x00, sizeof (reg) );
	reg *regs_task = (unsigned int)stack;
	regs_task->eip = TaskNew->eip;
	regs_task->esp = ( TaskNew->esp -= 0x10 );	// Toglie della roba superflua dallo stack
	regs_task->eflags = 0x206;
	regs_task->cs = 0x8;
	regs_task->ss = 0x10;
	regs_task->ds = 0x10;
	
	stack += 14;
	
	stack[0] = (unsigned int)suicide;
	
	TaskNew->fpid = 1;
	
	// Inizializza lo StdIn e lo StdOut (4kb ognuno)
	TaskNew->ptrStrOut = get_page(1);
	
	asm("sti");
	return TaskNew->pid;
}


unsigned int kill_task(unsigned int pid)
{
	asm("cli");
	struct task_table *TaskKill = TasksT->TaskT[pid];
	free_page(TaskKill->stackPage);
	free_page(TaskKill);
	// Solo se freepid == 0 allora decrementi anche il numero totale di processi
	if (! freePid(pid) )
		TasksT->totTasks--;
	asm("sti");
}

void suicide()
{
	asm("cli");
	struct task_table *Ftask = TasksT->TaskT[cur_task->fpid];
	Ftask->state = READY;
	kill_task(cur_task->pid);
	asm("sti");
	while(1);	// Attende il tick del timer
}

void init_function()
{
	asm("sti");
	video_scrivi_stringa("\n-> Multitasking avviato!\n");
	
	// Lo stdin è il buffer della tasiera
	cur_task->ptrStdIn = Kbuffer;
	
	for(;;);
		debug_stringa("\nCiclo IDLE del sistema :)");
}

void first_task_init(reg r)
{
	struct task_table *TaskNew = TasksT->TaskT[0];
	reg *cr = (unsigned int)r.esp - 0x24;	// Inizio del costrutto reg originale dallo stack
	cr->eip = TaskNew->eip;
	unsigned int top_newStack = (unsigned int)TaskNew->esp - (sizeof r);
	memcpyb(top_newStack, cr, sizeof r);
	
	asm("mov %0, %%esp" :: "r"(top_newStack));
	
	cur_task = TaskNew;
	// Rimuove l'handler
	installa_handler(36, 0x0);
	outb(0x20, 0x20);	// EOI
	asm("pop %ebx \n"
		"popa	\n"
		"add $8, %esp	\n"
		//"xchg %bx, %bx	\n"
		"sti	\n"
		"iret	\n");
}
void first_task()
{
	installa_handler(36, (unsigned int)first_task_init);
	asm("int $36");
}

void init_tasks()
{
	TasksT = get_page(1);
	memsetb( TasksT, 0x00, sizeof *TasksT );	// Pulisco l'area della tabella

	TasksT->totPid = 0;
	TasksT->nowRunning = 0;
	TasksT->totTasks = 0;
	scheNfo.nowCheck = 0;
	scheNfo.totCheck = 0;
	
	video_scrivi_stringa("make_task(init), pid:");
	video_scrivi_dec(make_task(init_function));	// Nuovo processo: Init
	
	video_scrivi_stringa("\nmake_task(shell), pid:");
	video_scrivi_dec(make_task(init_shell)); // Il task della shell
	
	
	first_task(); // Carica il primo task!
}




