/**
 * file		shell/shell.c
 * autore	Filippo Bedendo Bordon
 * data		24-12-11
**/

#include <shell/shell.h>
#include <driver/video/vga.h>
#include <driver/video/vesa.h>
#include <tasks/tasks.h>
#include <fs/tmpfs.h>
#include <fs/fat.h>
#include <tasks/schedule.h>

extern struct shell_vesa_info shV;

// Funzione ricorsiva
void manage_cmd()
{
	memsetb(Cbuffer, 0, 0x100);
	output_str("[MiniShell tmpfs]# ");
	unsigned int Clenght = (unsigned int) keyboard_read_with_output(Cbuffer);
	if ( Clenght == 0 )
	{
		output_str("\n");
		manage_cmd();
		return;
	}
	// Controlla che l'ultimo carattere immesso non sia uno spazio
	if (Cbuffer[(Clenght-1)] == ' ')
		Cbuffer[--Clenght] = '\0';

	// **TODO: Controlla che il primo carattere non sia uno spazio
	output_str("\n");

	make_args();
	// Ora abbiamo Argc e Argv settati!!


	// Eseguo il comando
	exec_cmd();


	manage_cmd();
}


void init_shell()
{
	asm("sti");
	if (IsvesaOn)
	{
		shV.x = 0;
		shV.y = 5;
		shV.forecolor = 0x00c500;
		shV.backcolor = 0x000000;
		shV.max_x = vmi->width;
		shV.max_y = vmi->height;
	}
	
	Cbuffer = get_page(1);	// La paginazione deve essere abilitata
	
	output_str("\nMiniShell -> Benvenuto! (digita aiuto per la lista dei comandi)\n");

	manage_cmd();
}

unsigned int keyboard_read_with_output(char *destBuffer)
{
	unsigned int ctr = 0;
	unsigned char c;
	while ( (c != '\n') && (ctr < 256) )	// <- Dimensione di un unsigned char
	{
		c = get_char(); 
		if ( (c != 0) && (c != '\n') )
		{
			if ( c == '\b' )
				destBuffer[--ctr] = '\0';
			else
				destBuffer[ctr++] = c;
			output_char(c);	// Scrive il carattere sullo schermo
		}
	}
	return ctr;	// Numero di caratteri letti
}

void make_args()
{
	// Sostituisco gli spazi(20) con i caratteri nulli (0) nella stringa digitata(Cbuffer)
	unsigned int strLen = str_len(Cbuffer);
	unsigned int Nargs = 0;
	unsigned int indice = 0;

	// Il primo argomento
	argv[Nargs++] = &Cbuffer[0];

	// Una spece di strReplace()
	while(indice < strLen)
	{
		if (Cbuffer[indice] == ' ')
		{
			Cbuffer[indice] = '\0';
			// Setto argv[1, 2,...] con i puntatori alle stringhe individuate dagli spazi
			argv[Nargs++] = &Cbuffer[indice+1];
		}
		indice++;
	}
	argc = Nargs;
}

void task_color()
{
	unsigned char *RamVideo = (unsigned char*) 0xB8000;
	unsigned char RandVal = 0x0;
	while(1)
	{
		RamVideo[0] = RandVal++;
		RamVideo[1] = RandVal++;
		RamVideo[2] = RandVal++;
		RamVideo[3] = RandVal++;
		RamVideo[4] = RandVal++;
		RamVideo[5] = RandVal++;
		RamVideo[6] = RandVal++;
		RamVideo[7] = RandVal++;
	}
}
extern unsigned int make_task(unsigned int eip);
void task_ls()
{
	unsigned int i;
	
	for ( i = 0; i < tmpfs_nFile; i++ )
	{
		output_str(tmpfs_Header[i].nome);
		output_str("\n");
	}
	return;
}


void exec_cmd()
{
	// Clear
	if ( strcmp(argv[0], "clear") )
	if (IsvesaOn)
	{
		for ( shV.y = 0; shV.y < shV.max_y; shV.y++ )
		{
			for ( shV.x = 0; shV.x < shV.max_x; shV.x++ )
			{
				vesa_draw_pixel(shV.x, shV.y, shV.backcolor);
			}
		}
		shV.x = 0;
		shV.y = 5;
	}
	else
		video_cancella();

	// Echo
	else if ( strcmp(argv[0], "echo") )
	{
		unsigned int i = 1;
		if (argc == 1) output_str("Uso: echo Something");
		else
		{
			while (i < argc)
			{
				output_str(argv[i++]);
				output_str(" ");
			}
		}
		output_str("\n");
	}

	// "Ls" (FAT)
	else if ( strcmp(argv[0], "ls") )
	{
		fat_list();
		output_str("\n");
	}
	
	// "Cat" (FAT)
	else if ( strcmp(argv[0], "cat") )
	{
		if (argc == 1) output_str("Uso: cat file (FAT fs)");
		else fat_fileout(argv[1]);
		output_str("\n");
	}
	
	// Colore
	else if ( strcmp (argv[0], "colore") )
	{
		if (IsvesaOn)
		{
			return;
			
			
		}
		else
		{
			if ( argc == 4 )
			{
				unsigned char backColor, foreColor, lamp;
				// BackColor
				if ( strcmp(argv[1], "nero") )			backColor = NERO;
				else if ( strcmp(argv[1], "blu") )		backColor = BLU;
				else if ( strcmp(argv[1], "verde") )	backColor = VERDE;
				else if ( strcmp(argv[1], "celeste") )	backColor = CELESTE;
				else if ( strcmp(argv[1], "rosso") )	backColor = ROSSO;
				else if ( strcmp(argv[1], "viola") )	backColor = VIOLA;
				else if ( strcmp(argv[1], "marrone") )	backColor = MARRONE;
				else if ( strcmp(argv[1], "fumo") )	backColor = BIANCO_SPORCO;
				else
					goto usage;
				// ForeColor
				if ( strcmp(argv[2], "nero") )			foreColor = NERO;
				else if ( strcmp(argv[2], "blu") )		foreColor = BLU;
				else if ( strcmp(argv[2], "verde") )	foreColor = VERDE;
				else if ( strcmp(argv[2], "celeste") )	foreColor = CELESTE;
				else if ( strcmp(argv[2], "rosso") )	foreColor = ROSSO;
				else if ( strcmp(argv[2], "viola") )	foreColor = VIOLA;
				else if ( strcmp(argv[2], "marrone") )	foreColor = MARRONE;
				else if ( strcmp(argv[2], "fumo") )		foreColor = BIANCO_SPORCO;
				else if ( strcmp(argv[2], "grigio") )	foreColor = GRIGIO;
				else if ( strcmp(argv[2], "blu_chiaro") )		foreColor = BLU_CHIARO;
				else if ( strcmp(argv[2], "verde_chiaro") )		foreColor = VERDE_CHIARO;
				else if ( strcmp(argv[2], "celestino") )		foreColor = CELESTINO;
				else if ( strcmp(argv[2], "rosso_chiaro") )		foreColor = ARANCIONE;
				else if ( strcmp(argv[2], "magenta") )	foreColor = ROSA;
				else if ( strcmp(argv[2], "giallo") )	foreColor = GIALLO;
				else if ( strcmp(argv[2], "bianco") )	foreColor = BIANCO;
				else
					goto usage;
				// Lamp
				if ( strcmp(argv[3], "on") )		lamp = 1;
				else if ( strcmp(argv[3], "off") )	lamp = 0;
				else
					goto usage;
				// Funzione che colora lo schermo (video.c)
				video_colora(foreColor, backColor, lamp);
				return;
			}
			else
				goto usage;
			usage:
			{
				output_str("Uso: colore background foreground lampeggio\n");
				output_str(" Backcolor: nero blu verde celeste rosso viola marrone fumo\n");
				output_str(" Forecolor: nero blu verde celeste rosso viola marrone fumo\n");
				output_str("  grigio blu_chiaro verde_chiaro celestino rosso_chiaro magenta giallo bianco\n");
				output_str(" Lampeggio: on off\n");
				output_str(" Esempio: colore rosso verde off\n");
				return;
			}
		}
	}

	else if ( strcmp (argv[0], "tasks") )
	{
		if ( strcmp (argv[1], "new") )
		{
			output_str("Nuovo task 'shell_idle()', pid: ");
			video_scrivi_dec(make_task(task_color));
			output_str("\n");
		}
		else if ( strcmp (argv[1], "kill") )
		{
			kill_task(2);
		}
		else if ( strcmp (argv[1], "high") )
		{
			setPriority(2, 0x100);
		}
		output_str("Numero di tasks in esecuzione: ");
		video_scrivi_dec(TasksT->totTasks);	// Non va per ora in vesa...
		output_str("\n");
	}
	
	else if ( strcmp (argv[0], "ls") )
	{
		make_task(task_ls);
		cur_task->state = BUSY;
		cur_task->Qattuale = 0;
		asm("int $32");
		
	}
	
	else if ( strcmp (argv[0], "loadimg") )
	{
		unsigned char *buffer = malloc(tmpfs_size(argv[1]));
		tmpfs_read(argv[1], buffer);
		render_raw_image(buffer, 0, 0);
		free(buffer);
	}
	
	// Forse un po' stupida $_$
	else if ( strcmp (argv[0], "cli") )
	{
		asm("cli");
	}
	
	else if ( strcmp (argv[0], "sti") )
	{
		asm("sti");
	}

	// Aiuto
	else if ( strcmp (argv[0], "aiuto") )
		output_str("I possibili comandi sono: aiuto, clear, colore, echo, tasks\n");
	else
	{
		output_str("Comansdo sconosciuto\n");
	}
}

void output_str(char *text)
{
	if (IsvesaOn)
	{
		unsigned int i = 0;
		while(text[i] != '\0')
			output_char(text[i++]);
	}
	else
		video_scrivi_stringa(text);
}

void output_char (char letter)
{
	if (IsvesaOn)
	{
		if (letter == '\n')
		{
			shV.y += FONT_HEIGHT;
			shV.x = 0;
			return;
		}
		else if ( letter == '\b' )
		{
			shV.x -= FONT_WIDTH;
			vesa_put_char('\b', shV.backcolor, shV.x, shV.y);
			return;
		}
		vesa_put_char(letter, shV.forecolor, shV.x, shV.y);
		if ( shV.x >= shV.max_x )
		{	
			shV.y += FONT_HEIGHT;
			shV.x = 0;
		} else {
		shV.x += FONT_WIDTH;
		}
		
	}
	else
	{
		put_char(letter);
		muovi_cursore();
	}
	
}


