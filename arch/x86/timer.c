/**
 * file 	arch/x86/timer.c
 * autore 	Filippo Bedendo Bordon
 * data 	24-12-11
 */

#include <x86/timer.h>
#include <x86/interrupt.h>
#include <tasks/tasks.h>


void timer_tick()
{
	
	//debug_stringa("tick timer!\n");
	if ( cur_task->Qattuale == 0 )
	{
		cur_task->Qattuale = cur_task->Qmax;
		asm("add $0x10, %esp	\n");
		temp_scheduler();
	}
	else
		cur_task->Qattuale--;
	pic_EOI_master();
}

void init_timer()
{
	installa_handler(32, (unsigned int)timer_tick);
}

