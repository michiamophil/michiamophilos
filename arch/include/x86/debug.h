/**
 * file 	arch/include/x86/debug.h
 * autore 	Filippo Bedendo Bordon
 * data 	22-11-11	
 */
 
#ifndef x86_debug
#define x86_debug

// Definito solo se si vuole il debug attivo
#define DEBUG

// Si stamperà il debug su bochs
#define BOCHS_DEBUGGER 

void debug_stringa(const char *s);
#endif
