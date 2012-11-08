/**
 * file		fs/tmpfs.c
 * autore	Filippo Bedendo Bordon
 * data		14-01-12
 * info		Gestione del mio fs (temporaneo, senza vfs)
**/ 

#include <fs/tmpfs.h>


void init_tmpfs()
{
	video_scrivi_stringa("\n-> Inizializzo il TempFs...\n");
	tmpfs_nFile = *(unsigned char *) tmpfs_start;
	tmpfs_Header = (struct file_header *) (tmpfs_start + 1);
	video_scrivi_stringa("Numero di file trovati: ");
	video_scrivi_dec(tmpfs_nFile);
}

// Molto molto sbrigativa...
unsigned int tmpfs_read(char *nome, unsigned char *buffer)
{
	unsigned int i = 0;

	unsigned int sorgente, lunghezza;

	while ( i < tmpfs_nFile )
	{
		if ( strcmp(nome, tmpfs_Header[i].nome) )
		{
			sorgente = tmpfs_start + tmpfs_Header[i].posizione;
			lunghezza = tmpfs_Header[i].dimensione;
			memcpyb(buffer, sorgente, lunghezza);
			return lunghezza;
		}
		i++;
	}
	return 0;
}


unsigned int tmpfs_size(char *nome)
{
	unsigned int i = 0;
	
	while ( i < tmpfs_nFile )
	{
		if ( strcmp(nome, tmpfs_Header[i].nome) )
			return tmpfs_Header[i].dimensione;
		i++;
	}
	return 0;
}
