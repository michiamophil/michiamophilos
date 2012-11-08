/**
 * file 	arch/include/x86/interrupts.h
 * autore 	Filippo Bedendo Bordon
 * data 	25-11-11	
 */
 
#ifndef x86_interrupts
#define x86_interrupts

// Pila
struct registers
{
    unsigned int ds;                  // "Push eax" di gestione _*
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; // "Pusha" di gestione _*
    unsigned int int_no, err_code;    // "Push byte *" di isr_* o irq_*
    unsigned int eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
}__attribute__((packed));
typedef struct registers reg;
typedef void (*hdler)(reg);

void installa_handler(char i, int eip);
void isr_handler(reg r);
void irq_handler(reg r);

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();

extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr_reserved();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();
#endif
