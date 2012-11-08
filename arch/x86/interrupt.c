/**
 * file 	arch/x86/interrupt.c
 * autore 	Filippo Bedendo Bordon
 * data 	25-11-11	
 */
 
#include <x86/mem_map.h>
#include <x86/pic.h>
#include <x86/interrupt.h>

int *handlers = (int *) HANDLER_ADDR;
char *isr_errori[] = 
{
	"Errore di divisione", "Eccezione di debugging", "Non Maskable Interrupt", "Breakpoint/Debugging",
	"Overflow", "Bound Range Exceeded", "Codice operativo non valido (istruzione sconosciuta)",
	"Coprocessore non disponibile", "Doppio difetto", "Superamento del segmento di coprocessore",
	"TSS non valido", "Segmento non presente", "Eccezione di stack","Protezione generale", "Page Fault", 
	"Intel Reserved", "Floating Point Error (Math Fault)", "Alignment Check", "Machine Check", 
	"Streaming SIMD Extensions", "Intel Reserved", "Intel Reserved", "Intel Reserved", "Intel Reserved",
	"Intel Reserved", "Intel Reserved", "Intel Reserved", "Intel Reserved", "Intel Reserved", "Intel Reserved",
	"Intel Reserved", "Intel Reserved"
};

void installa_handler(char i, int eip)
{
	handlers[i] = eip;
}

void isr_handler(reg r)
{
	if (handlers[r.int_no] != 0) {
		hdler handler = handlers[r.int_no];
		handler(r);
	} else {
	// Solo se non c'è l'handler di gestione dell'isr r.int_no
		video_cancella();
		video_scrivi_stringa("Le ultime parole famose: Kernel Panic...\n");
		video_scrivi_stringa("Qualcosa non è andato per il verso giusto e io, Kernel :) ho il dovere di freezare il tuo computer...\n");
		video_scrivi_stringa("Sono spiacente... Magari prova a riavviarmi :)\n");
		video_scrivi_stringa("\n\nInformazioni sull'errore: ");
		video_scrivi_stringa(isr_errori[r.int_no]);
		video_scrivi_stringa("\n");
		reg_dump(r);
		for(;;);
	}
}

void irq_handler(reg r)
{
	if(handlers[r.int_no] != 0)
	{
		hdler handler = handlers[r.int_no];
		handler(r);
		//call_scheduler(r);
	}
	
	// End Of Interrupt to
	if(r.int_no >= 40)
		pic_EOI_slave();
    pic_EOI_master();
}
