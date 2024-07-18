#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

#include "cimple.h"

void setup_header(char *header, long w, long h) {
  sprintf(header, "P6\n%04ld %04ld\n255\n", w, h);
}

num_validation_codes validate_number(char *nstr, uint32_t *num, long max, int base) {
  char *buff_ptr = (char *)malloc(sizeof(char)*INT_BUFFER);
  char *endstr = buff_ptr;
  num_validation_codes valid_code = ok;

  /* If nstr exceeds max lenght then is not valid */
  if ( strlen(nstr) > ceil(log10(max+1)) ) {
    valid_code = bigger_than_max;
  } else {
    /* If does exceed max. try to convert */
    *num = strtol(nstr, &endstr, base);
    if (*endstr != '\0')
      valid_code = no_number_syntax;
  }

  free(buff_ptr);
  return valid_code;
}

void paint_pixel(ppm *img, uint32_t color, point p) {
  uint32_t x = round(p.x);
  uint32_t y = round(p.y);
  uint32_t i = y*img->width + x;
  //printf(" --> pinté en: x: %d, y: %d, i: %d.\n",x, y, i);
  if (p.x < img->width && p.y < img->height) {
    img->pixels[3*i+0] = (color>>(8*0))&0xFF;
    img->pixels[3*i+1] = (color>>(8*1))&0xFF;
    img->pixels[3*i+2] = (color>>(8*2))&0xFF;
  }
}
  
void fill_background(ppm *img, uint32_t color) {
  uint32_t w = img->width;
  uint32_t h = img->height;
  point p;
  for (uint32_t i = 0; i < w*h; i++) {
    p.x = i%img->width;
    p.y = (uint32_t)(i/img->height);
    paint_pixel(img, color, p);
  }
}

void line(ppm *img, uint32_t color, point p0, point p1) {
  int  canonical = 0;
  float m, range;
  /* use as parameter the one with longest range */
  if ( fabs(p1.x - p0.x) >= fabs(p1.y - p0.y) ) {
    range = fabs(p1.x - p0.x) + 1;
    canonical = 1;
    /* make sure p0.x < p1.x */
    if (p1.x < p0.x) { point temp_p = p1; p1 = p0; p0 = temp_p; }
    m = (p1.y - p0.y)/(p1.x - p0.x);
  } else {
    range = fabs(p1.y - p0.y) + 1;
    if (p1.y < p0.y) { point temp_p = p1; p1 = p0; p0 = temp_p; }
    m = (p1.x - p0.x)/(p1.y - p0.y);
  }
  
  point p;
  for(int i = 0; i < range; i++) {
    /* draw line depending on canonical case y=mx+b or x = my+b */
    if (canonical) { /* canonical case */
      p.x = i+p0.x;
      p.y = (i == 0 ? p0.y : m*p.x + p0.y - m*p0.x);
      
    } else { /* non-canonical case */
      p.y = i+p0.y;
      p.x = (i == 0 ? p0.x : m*p.y + p0.x - m*p0.y);
    }
    paint_pixel(img, color, p);
  }
  if ( !range ) {
    paint_pixel(img, color, p0);
  }
}

void rectangle(ppm *img,
	       uint32_t *color,
	       point origen,
	       uint32_t w,
	       uint32_t h) {
  for (uint32_t i = 0; i < w; i++){
    point p0 = {origen.x+i, origen.y};
    point p1 = {origen.x+i, origen.y+h};
    line(img, *color, p0, p1);
  }
}

int validate_cmds(int ac,
		  char *av[],
		  uint32_t *color,
		  uint32_t *w,
		  uint32_t *h) {
  int r = 0;
  *w = DEFAULT_IMG_SIZE;
  *h = DEFAULT_IMG_SIZE;
  *color = 0xffff;
  if (ac < 4) {
    printf("Image default: %dx%d. Background color yellow.",
	   DEFAULT_IMG_SIZE, DEFAULT_IMG_SIZE);
    return 0;
  } else { /* Parse width of image */
    if (validate_number(av[1], w, MAX_WIDTH, 10) < 0 ||
	validate_number(av[2], h, MAX_HEIGHT, 10) < 0) {
      *w = DEFAULT_IMG_SIZE;
      *h = DEFAULT_IMG_SIZE;
      r = -1;
    }
    if (validate_number(av[3], color, 0xFFFFFFFF, 16) < 0) {
      *color = 0xffff;
      r = -2;
    }
    printf("Image size: %dx%d.\n", *w, *h);
  }
  return r;
}

void free_ppm(ppm *img) {
  free(img->header);
  free(img->pixels);
}

int open_img(ppm *img, uint32_t w, uint32_t h) {
  char *header = (char *)malloc(sizeof(char)*HEADER_LEN);
  uint8_t *pixels = (uint8_t *)malloc(sizeof(uint8_t)*3*w*h);

  if ( !header || !pixels ) {
    return -1;
  }

  setup_header(header, w, h);

  (*img).header=header;
  (*img).pixels=pixels;
  (*img).width=w;
  (*img).height=h;

  return 0;
}

int write_img_to_file(char *fname, ppm img) {
  FILE *img_file;
  uint32_t w = img.width;
  uint32_t h = img.height;
  if ((img_file = fopen(fname, "wb")) == NULL) {
    fprintf(stderr, "Unable to open %s.", fname);
    return -1;
  }
  fwrite(img.header, sizeof(char), HEADER_LEN, img_file);
  fwrite(img.pixels, sizeof(uint8_t), 3*w*h, img_file);
  fclose(img_file);
  return 0;;
}

void print_point(point p, bool nl) {
  printf("(%.2f, %.2f)", p.x, p.y);
  if (nl)
    putchar('\n');
}

void triangle(ppm *img, uint32_t color, point p0, point p1, point p2) {
  /* TODO: make sure no two points are equal. If so draw the
     corresponding line (degenerated triangle). */
  /* calculo direcciones hacia dentro */
  /* central points of sides */
  point m0 = scalar_mult_points(sum_points(p1, p2), 0.5);
  point m1 = scalar_mult_points(sum_points(p2, p0), 0.5);
  point m2 = scalar_mult_points(sum_points(p0, p1), 0.5);
  /* directions towards center */
  point d0 = sum_points(m0, minus_points(p0));
  point d1 = sum_points(m1, minus_points(p1));
  point d2 = sum_points(m2, minus_points(p2));
  /* normalized directions */
  float l0 = norm_points(d0);
  float l1 = norm_points(d1);
  float l2 = norm_points(d2);
  point n0 = scalar_mult_points(d0, 1/l0);
  point n1 = scalar_mult_points(d1, 1/l1);
  point n2 = scalar_mult_points(d2, 1/l2);

  float max_l = l0;
  if (l1 >= l2 && l1 >= l0) max_l = l1;
  if (l2 >= l1 && l2 >= l0) max_l = l2;
  
  for (int i = 0; i < max_l/2; i++) {
    line(img, color, p0, p1);
    line(img, color, p1, p2);
    line(img, color, p2, p0);

    p0 = sum_points(p0, n0);
    p1 = sum_points(p1, n1);
    p2 = sum_points(p2, n2);
  }
}

point sum_points(point a, point b) {
  point t = {.x = a.x + b.x, .y = a.y + b.y};
  return t;
}

point scalar_mult_points(point v, float a) {
  point t = {.x = a*v.x, .y = a*v.y};
  return t;
}

float norm_points(point v) {
  float r = v.x*v.x + v.y*v.y;
  return sqrt(r);
}

point minus_points(point p) {
  point t = {.x = -p.x, .y = -p.y};
  return t;
}
