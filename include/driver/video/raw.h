/**
 * file 	include/driver/video/raw.h
 * autore 	Filippo Bedendo Bordon
 * data 	22-02-12	
**/

#ifndef driver_video_raw
#define driver_video_raw

typedef struct
{
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel;
  unsigned char	 *pixel_data;
} raw_image;

#endif
