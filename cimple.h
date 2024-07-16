#ifndef _CIMPLE_H_
#define _CIMPLE_H_

#include <inttypes.h>

/* max width and height of images in pixels */
#define MAX_WIDTH  9999
#define MAX_HEIGHT 9999
#define HEADER_LEN 17
#define INT_BUFFER 64 // buffer size to store strings representing integers

typedef struct _point {
  float x;
  float y;
} point;

typedef struct _ppm {
  char *header;
  uint8_t *pixels;
  uint32_t width;
  uint32_t height;
} ppm;

void setup_header(char *header, long w, long h);
long validate_number(char *nstr, long max, int base);
void fill_background(uint8_t *ppm, uint32_t color, int w, int h);
void line(ppm *img, uint32_t color, point p0, point p1);
void rectangle(ppm *img, uint32_t *color, point origen, uint32_t, uint32_t);

#endif
