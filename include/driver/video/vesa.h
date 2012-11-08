/**
 * file 	include/driver/video/vesa.h
 * autore 	Filippo Bedendo Bordon
 * data 	22-02-12	
 * info		Grazie a SpiderpigOs :D (Davide Gessa)
**/

#ifndef driver_video_vesa
#define driver_video_vesa

#define PIXEL_INTERLETTER 8
#define FONT_HEIGHT 16
#define FONT_WIDTH 8

// Informazioni generali vbe
typedef struct
{
    char signature[4];
    unsigned short version;
    unsigned int oem_string_ptr;
    unsigned int capabilities;
    unsigned int video_mode_ptr;
    unsigned short total_memory;
	
	// Vesa 2.0
	unsigned int oem_software_rev;
	char *oem_vendor_name;
	char *oem_product_name;
	char *oem_product_rev;
} vesa_info_t;

 
// Informazioni sulla modalità in uso
typedef struct
{
    unsigned short mode_attributes;
    unsigned char wina_attributes;
    unsigned char winb_attributes;
    unsigned short win_granularity;
    unsigned short win_size;
    unsigned short wina_segment;
    unsigned short winb_segment;
    unsigned int win_pos_func_ptr;
    unsigned short bytes_per_scan_line;
    unsigned short width;
    unsigned short height;
    unsigned char char_width;
    unsigned char char_height;
    unsigned char num_planes;
    unsigned char bits_per_pixel;
    unsigned char num_banks;
    unsigned char memory_model_type;
    unsigned char bank_size;
    unsigned char num_image_pages;
    unsigned char reserved1;
    unsigned char red_mask_size;
    unsigned char red_field_position;
    unsigned char green_mask_size;
    unsigned char green_field_position;
    unsigned char blue_mask_size;
    unsigned char blue_field_position;
    unsigned char reserved_mask_size;
    unsigned char reserved_mask_position;
    unsigned char direct_color_mode_info;
    unsigned int phys_base_ptr;
    unsigned int offscreen_mem_ptr;
    unsigned short offscreen_mem_size;
    unsigned short lin_bytes_per_scan_line;
    unsigned char bnk_num_image_pages;
    unsigned char lin_num_image_pages;
    unsigned char lin_red_mask_size;
    unsigned char lin_red_field_position;
    unsigned char lin_green_mask_size;
    unsigned char lin_green_field_position;
    unsigned char lin_blue_mask_size;
    unsigned char lin_blue_field_position;
    unsigned char lin_rsvd_mask_size;
    unsigned char lin_rsvd_field_position;
    unsigned int max_pixel_clock;
    unsigned char reserved4[190];
} vesa_mode_info_t;


void vesa_draw_pixel(unsigned int x, unsigned int y, unsigned int RGB);
void vesa_init(void *vbe_control_info, void *vbe_mode_info);
vesa_info_t *vci;
vesa_mode_info_t *vmi;
char* vesamem;
unsigned int vesasize;
char IsvesaOn;

#endif
