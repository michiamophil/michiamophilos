/**
 * file		arch/x86/debug.c
 * autore	Filippo Bedendo Bordon
 * data		22-11-11	
 * info		Utile guida per il debug con bochs: http://www.osdev.it/?p=49
**/

#include <x86/debug.h>
#include <tasks/tasks.h>
#include <x86/interrupt.h>

// In base al debugger settato
void debug_stringa(const char *s)
{
#ifdef BOCHS_DEBUGGER
	unsigned int i;
    for(i = 0; s[i] != '\0'; i++)
        outb(0xE9, s[i]);	// Porta speciale di bochs 
#else
	video_scrivi_stringa("debug_stringa(): nessun debugger trovato!");
#endif
}

void debug_hex(unsigned int n)
{
	int tmp;

    debug_stringa("0x");

    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4)
    {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0) continue;
    
        if (tmp >= 0xA)
        {
			noZeroes = 0;
            outb(0xE9, tmp-0xA+'a');
        }
        else
        {
            noZeroes = 0;
             outb(0xE9, tmp+'0');
        }
    }
  
    tmp = n & 0xF;
    if (tmp >= 0xA) outb(0xE9, tmp-0xA+'a');

    else outb(0xE9, tmp+'0');
}

void debug_break()
{
#ifdef BOCHS_DEBUGGER
	asm("xchg %bx, %bx");
#else
	video_scrivi_stringa("debug_break(): nessun debugger trovato!");
#endif
}

void reg_dump_confr(reg r, reg *re)
{
	video_scrivi_stringa("\nREG DUMP");
		video_scrivi_stringa("\nDS: ");
	video_scrivi_hex(r.ds); video_scrivi_stringa(" - "); video_scrivi_hex(re->ds);
	video_scrivi_stringa("\nEDI: ");
	video_scrivi_hex(r.edi); video_scrivi_stringa(" - "); video_scrivi_hex(re->edi);
	video_scrivi_stringa("\nESI: ");
	video_scrivi_hex(r.esi); video_scrivi_stringa(" - "); video_scrivi_hex(re->esi);
	video_scrivi_stringa("\nEBP: ");
	video_scrivi_hex(r.ebp); video_scrivi_stringa(" - "); video_scrivi_hex(re->ebp);
	video_scrivi_stringa("\nESP: ");
	video_scrivi_hex(r.esp); video_scrivi_stringa(" - "); video_scrivi_hex(re->esp);
	video_scrivi_stringa("\nEBX: ");
	video_scrivi_hex(r.ebx); video_scrivi_stringa(" - "); video_scrivi_hex(re->ebx);
	video_scrivi_stringa("\nEDX: ");
	video_scrivi_hex(r.edx); video_scrivi_stringa(" - "); video_scrivi_hex(re->edx);
	video_scrivi_stringa("\nECX: ");
	video_scrivi_hex(r.ecx); video_scrivi_stringa(" - "); video_scrivi_hex(re->ecx);
	video_scrivi_stringa("\nEAX: ");
	video_scrivi_hex(r.eax); video_scrivi_stringa(" - "); video_scrivi_hex(re->eax);
	video_scrivi_stringa("\n---int_no---err_code---");
	video_scrivi_stringa("\nEIP: ");
	video_scrivi_hex(r.eip); video_scrivi_stringa(" - "); video_scrivi_hex(re->eip);
	video_scrivi_stringa("\nCS: ");
	video_scrivi_hex(r.cs); video_scrivi_stringa(" - "); video_scrivi_hex(re->cs);
	video_scrivi_stringa("\nEFLAGS: ");
	video_scrivi_hex(r.eflags); video_scrivi_stringa(" - "); video_scrivi_hex(re->eflags);
	video_scrivi_stringa("\nUSERESP: ");
	video_scrivi_hex(r.useresp); video_scrivi_stringa(" - "); video_scrivi_hex(re->useresp);
	video_scrivi_stringa("\nSS: ");
	video_scrivi_hex(r.ss); video_scrivi_stringa(" - "); video_scrivi_hex(re->ss);
}

void reg_dump(reg r)
{
	video_scrivi_stringa("\nREG DUMP");
		video_scrivi_stringa("\nDS: ");
	video_scrivi_hex(r.ds);
	video_scrivi_stringa("\nEDI: ");
	video_scrivi_hex(r.edi);
	video_scrivi_stringa("\nESI: ");
	video_scrivi_hex(r.esi);
	video_scrivi_stringa("\nEBP: ");
	video_scrivi_hex(r.ebp);
	video_scrivi_stringa("\nESP: ");
	video_scrivi_hex(r.esp);
	video_scrivi_stringa("\nEBX: ");
	video_scrivi_hex(r.ebx);
	video_scrivi_stringa("\nEDX: ");
	video_scrivi_hex(r.edx);
	video_scrivi_stringa("\nECX: ");
	video_scrivi_hex(r.ecx);
	video_scrivi_stringa("\nEAX: ");
	video_scrivi_hex(r.eax);
	video_scrivi_stringa("\n---int_no---err_code---");
	video_scrivi_stringa("\nEIP: ");
	video_scrivi_hex(r.eip);
	video_scrivi_stringa("\nCS: ");
	video_scrivi_hex(r.cs);
	video_scrivi_stringa("\nEFLAGS: ");
	video_scrivi_hex(r.eflags);
	video_scrivi_stringa("\nUSERESP: ");
	video_scrivi_hex(r.useresp);
	video_scrivi_stringa("\nSS: ");
	video_scrivi_hex(r.ss);
}

void reg_dump_pusha(struct pushareg r)
{
	video_scrivi_stringa("\nREG DUMP PUSHA");
	video_scrivi_stringa("\nEDI: ");
	video_scrivi_hex(r.edi);
	video_scrivi_stringa("\nESI: ");
	video_scrivi_hex(r.esi);
	video_scrivi_stringa("\nEBP: ");
	video_scrivi_hex(r.ebp);
	video_scrivi_stringa("\nESP: ");
	video_scrivi_hex(r.esp);
	video_scrivi_stringa("\nEBX: ");
	video_scrivi_hex(r.ebx);
	video_scrivi_stringa("\nEDX: ");
	video_scrivi_hex(r.edx);
	video_scrivi_stringa("\nECX: ");
	video_scrivi_hex(r.ecx);
	video_scrivi_stringa("\nEAX: ");
	video_scrivi_hex(r.eax);
}
