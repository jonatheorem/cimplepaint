#ifndef _CIMPLE_H_
#define _CIMPLE_H_

#include <inttypes.h>

/* max width and height of images in pixels */
#define MAX_WIDTH  9999
#define MAX_HEIGHT 9999
#define HEADER_LEN 17
#define INT_BUFFER 64 // buffer size to store strings representing integers
void setup_header(char *header, long w, long h);
long validate_number(char *nstr, long max, int base);
void fill_background(uint8_t *ppm, uint32_t color, int w, int h);
void line(uint8_t *ppm, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1);

#endif
