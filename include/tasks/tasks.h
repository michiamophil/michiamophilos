/**
 * file 	include/tasks/tasks.h
 * autore 	Filippo Bedendo Bordon
 * data 	05-04-12
**/


struct tasks_table {
	unsigned int *TaskT[256];
	unsigned int nowRunning;	// Processo in esecuzione (indice)
	unsigned char totPid;		// Variabile per set/freePID
	unsigned char totTasks;		// Numero totale di task presenti nell'array
};

struct task_table {
	unsigned int esp, eip;
    
	unsigned char pid;
	
	unsigned char state;
	// Quanto di tempo che il processo sta in esecuzione
	unsigned int Qattuale, Qmax;
	//unsigned int *pageDir;
	
	// Contiene l'indirizzo iniziale di get_page(), così che il free_page non sia un problema
	unsigned int stackPage;
	
	// Puntatori ai buffer
	unsigned int *ptrStrOut;
	unsigned int *ptrStdIn;
	
	// Indici dei buffer
	unsigned short Iout;
	unsigned short Iin;
	unsigned char fpid;
};

struct pushareg {
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; 
}__attribute__((packed));


extern struct tasks_table *TasksT;
extern struct task_table *cur_task;

void init_function();
