/**
 * file 	driver/video/vesadd.c
 * autore 	Filippo Bedendo Bordon
 * data 	29-02-12	( Il prossimo sarà tra 4 anni!!!)
 * info		Funzioni in aggiunta (che non servono a niente) e possono contenere molti bug...
**/

#include <driver/video/vesa.h>
#include <driver/video/raw.h>

// Disegna una mappa di bit sullo schermo
void vesadd_raw_image(unsigned char *src, int x, int y)
{	
	raw_image raw;
	unsigned int i, j;
	
	unsigned int *info = src;
		
	raw.width = info[0];
	raw.height = info[1];
	raw.bytes_per_pixel = info[2];
	raw.pixel_data = (char *) (src+12);
	
	// Allinea a sinistra
	if ( x < 0 )
		x = vmi->width - raw.width;
	if ( y < 0 )
		y = vmi->height - raw.height;
	
	for(i = 0; i < raw.height; i++)
		for(j = 0; j < raw.width; j++)
		{
			unsigned int RGB = 
				(raw.pixel_data[((i*4*raw.width)+j*4)+3] << 24) +
				(raw.pixel_data[((i*4*raw.width)+j*4)+1] << 8) +
				(raw.pixel_data[((i*4*raw.width)+j*4)+2] << 0)  + 
				(raw.pixel_data[(i*4*raw.width)+j*4] << 16);
			
			if(RGB != 0) vesa_draw_pixel(x + j, y + i, RGB); 	
		}	
}
