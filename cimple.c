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
  uint32_t i = p.y*img->width + p.x;
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
  int range, canonical = 0;
  float m;
  bool vert_hor = false;
  /* Interchange to make sure x0, y0 < x1, y1 */
  if (p1.x < p0.x) { uint32_t t = p1.x; p1.x = p0.x; p0.x = t; }
  if (p1.y < p0.y) { uint32_t t = p1.y; p1.y = p0.y; p0.y = t; }
  /* not draw anything if out of bounds points */
  if (p1.x > img->width || p1.y > img->height) {
    return;
  }
  /* use as parameter the one with longest range */
  if ( p1.x - p0.x >= p1.y - p0.y ) {
    range = p1.x - p0.x;
    m = (float)(p1.y - p0.y)/(p1.x - p0.x);
    if (p1.y == p0.y)
      vert_hor = true;
    canonical = 1;
  } else {
    range = p1.y - p0.y;
    m = (float)(p1.x - p0.x)/(p1.y - p0.y);
    if (p1.x == p0.x)
      vert_hor = true;
  }
  point p;
  for(int i = 0; i < range; i++) {
    /* draw line depending on canonical case y=mx+b or x = my+b */
    if (canonical) { /* canonical case */
      p.x = i+p0.x;
      /* check if line is vertical or horizontal */
      if (!vert_hor) { 
	p.y = (i == 0 ? p0.y : round(m*p.x + p0.y - 1));
      } else {
	p.y = (i == 0 ? p0.y : round(m*p.x + p0.y));
      }
    } else { /* non-canonical case */
      p.y = i+p0.y;
      /* check if line is vertical or horizontal */
      if (!vert_hor) {
	p.x = (i == 0 ? p0.x : round(m*p.y + p0.x-1));
      } else {
	p.x = (i == 0 ? p0.x : round(m*p.y + p0.x));
      }
    }
    paint_pixel(img, color, p);
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
  *w = 32;
  *h = 32;
  *color = 0xffff;
  if (ac < 4) {
    puts("Image default: 32x32. Background color yellow.");
    return 0;
  } else { /* Parse width of image */
    if (validate_number(av[1], w, MAX_WIDTH, 10) < 0 ||
	validate_number(av[2], h, MAX_HEIGHT, 10) < 0) {
      *w = 32;
      *h = 32;
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
