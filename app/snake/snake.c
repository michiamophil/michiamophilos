/**
 * file 	app/snake.c
 * autore 	Filippo Bedendo Bordon
 * data 	30-01-12
 */ 

// Gioco snake :P
void main_snake()
{
	// Prima cosa, sposto la pagina 0 sulla pagina 1
	// In sostanza copio da 0xB8000 a 0xB8FFF, su 0xB9000
	memcpyb(0xB9000, 0xB8000, 0x1000);


	unsigned char *Sbuffer = get_page(1);
	tmpfs_read("pixmap.sk", Sbuffer);
	//video_scrivi_stringa(Sbuffer);
	// Ora posso scrivere tranquillamente sullo schermo:
	memcpyb(0xB8000, Sbuffer, 0x1000);
	free_page(Sbuffer); 
}
