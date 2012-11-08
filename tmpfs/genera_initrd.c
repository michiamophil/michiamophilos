/**
 * file 	fs/genera_initrd.c
 * autore 	Filippo Bedendo Bordon
 * data 	26-12-11
**/

#include <stdio.h>

#define MAX_CHAR 35	// Per non lasciare spazio sprecato nell'header :)

// Intestazione di ogni file
struct file_header {
	unsigned int posizione;
	unsigned int dimensione;
	unsigned char nome[MAX_CHAR];
	unsigned char magic;	// Attenzione, se si scrive su header[i].nome[35] in poi, si perdono dati!!!
};

int main(char argc, char **argv)
{
	char n_file = argc - 1;	// L'argomento 1 è il nome del programma e a noi non ci interessa xD
	struct file_header header[n_file];	// Definisco un'array di header (uno per ogni file)
	unsigned int inizio_dati = sizeof(char) + sizeof(struct file_header) * n_file;	// sizeof(char) è la dimensione di n_file
	//printf("Header: %dbyte\n", sizeof(struct file_header));	// PRINT
	memset(&header, 0x00, sizeof(struct file_header) * n_file);	// Pulisce l'area dell'header

	char i;
	// Setta l'header di ogni file
	for (i = 0; i < n_file; i++) {
		strcpy(header[i].nome, argv[i+1]);
		header[i].posizione = inizio_dati;
		// Provo ad aprire il file in lettura
		FILE *stream = fopen(argv[i+1], "r");
		if (!stream) {
			printf("Errore: non è stato possibile aprire il file: %s\nProgramma terminato\n", argv[i+1]);	// PRINT
			return 1;
		}
		fseek(stream, 0, SEEK_END);	// Fine dello stream
		header[i].dimensione = ftell(stream);	// Restituisce la posizione
		printf("_(File \"%s\") posizione: 0x%x, dimensione: %dbyte\n", header[i].nome, header[i].posizione, header[i].dimensione);	// PRINT
		inizio_dati += header[i].dimensione;	// Preparo la posizione dei dati per il file successivo
		fclose(stream);
		header[i].magic = 0xBF;	// E' una entry di un FS
	}

	// Scrive l'initrd
	FILE *initrd = fopen("./initrd", "w");
	unsigned char *data = (unsigned char *) malloc(inizio_dati);
	fwrite(&n_file, sizeof(char), 1, initrd);	// Scrivo la var n_file
	fwrite(header, sizeof(struct file_header), n_file, initrd);	// Scrivo l'header
	// Copio i file (passati come parametri) nell'initrd
	for (i = 0; i < n_file; i++) {
		FILE *temp = fopen(argv[i+1], "r");	// Apro la sorgente
		unsigned char *buffer = (unsigned char *) malloc(header[i].dimensione);	// Allocco lo spazio per il buffer
		fread(buffer, 1, header[i].dimensione, temp);	// Copia il file nel buffer temporaneo
		fwrite(buffer, 1, header[i].dimensione, initrd);	// Preleva i dati dal buffer e li scrive nell'initrd
		fclose(temp);
		free(buffer);
	}

	fclose(initrd);
	free(data);
}


/*// Restituisce la somma delle dimensioni dei vari file
	unsigned int dimensione_dati()
	{
		unsigned int temp = 0;
		for (i; i > 0; i--)
			temp += header[i-1].dimensione;
		return temp;
	}

	unsigned int n, h, d;	// n_file, header, dati
	n = sizeof(char);
	h = sizeof(struct file_header) * n_file;
	d = dimensione_dati();
	printf("\nStruttura FileSystem:\n0x0 N_file\n");	// PRINT 
	printf("0x%x -> 0x%x\tIntestazioni\n", n, n+h-1);					// PRINT
	printf("0x%x -> 0x%x\tDati\n", n+h, n+h+d-1);						// PRINT
*/
