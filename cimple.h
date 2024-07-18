#ifndef _CIMPLE_H_
#define _CIMPLE_H_

#include <inttypes.h>

/* max width and height of images in pixels */
#define MAX_WIDTH  9999
#define MAX_HEIGHT 9999
#define HEADER_LEN 17
#define INT_BUFFER 64 // buffer size to store strings representing integers
#define DEFAULT_IMG_SIZE 128

// define boleanos as enum
typedef enum _bool {
  false = 0,
  true  = 1
} bool;

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

typedef enum _num_validation_codes {
  ok = 0,
  bigger_than_max = -1,
  no_number_syntax = -2
} num_validation_codes;

//typedef enum _num_validation_codes num_validation_codes;

void setup_header(char *header, long w, long h);
num_validation_codes validate_number(char *nstr, uint32_t *num, long max,
                                     int base);
void paint_pixel(ppm *img, uint32_t color, point p);
void fill_background(ppm *img, uint32_t color);
void line(ppm *img, uint32_t color, point p0, point p1);
void rectangle(ppm *img, uint32_t *color, point origen, uint32_t, uint32_t);

int validate_cmds(int argc, char *argv[], uint32_t *color, uint32_t *w,
                  uint32_t *h);
int open_img(ppm *img, uint32_t w, uint32_t h);
int write_img_to_file(char *fname, ppm img);
void free_ppm(ppm *img);

void print_point(point p, bool nl);

void triangle(ppm *img, uint32_t color, point p0, point p1, point p2);
point sum_points(point a, point b);
point scalar_mult_points(point v, float a);
float norm_points(point v);
point minus_points(point p);

#endif
