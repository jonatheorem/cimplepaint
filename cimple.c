#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

#include <stdbool.h>

#include "headers/cimple.h"

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
  uint32_t c = 0;
  uint8_t ab, rb, gb, bb; /* channels background */
  uint8_t af, rf, gf, bf; /* channels foreground */
  uint8_t ap, rp, gp, bp; /* channels pixels     */
  if (x < img->width && y < img->height) {
    /* Before painting i need to see exiting color and alpha */
    ab = ((img->pixels[i])>>(8*3))&0xFF;
    bb = ((img->pixels[i])>>(8*2))&0xFF;
    gb = ((img->pixels[i])>>(8*1))&0xFF;
    rb = ((img->pixels[i])>>(8*0))&0xFF;

    af = (color>>(8*3))&0xFF;
    bf = (color>>(8*2))&0xFF;
    gf = (color>>(8*1))&0xFF;
    rf = (color>>(8*0))&0xFF;

    ap = af + ab - (ab*af)/255;

    float factor = (ab - (float)ab*af/255);
    /* WHAT IF ap = 0 ???? */
    bp = (bf*af + bb*factor)/ap;
    gp = (gf*af + gb*factor)/ap;
    rp = (rf*af + rb*factor)/ap;
    
    c = c | ap;
    c = (c<<8) | bp;
    c = (c<<8) | gp;
    c = (c<<8) | rp;

    /* c = af; */
    /* c = (c<<8) | bf; */
    /* c = (c<<8) | gf; */
    /* c = (c<<8) | rf; */

    img->pixels[i] = c;
  }
}
  
void fill_background(ppm *img, uint32_t color) {
  uint32_t w = img->width;
  uint32_t h = img->height;
  for (uint32_t i = 0; i < w*h; i++) {
    img->pixels[i] = color;
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
	       uint32_t color,
	       point origen,
	       uint32_t w,
	       uint32_t h) {
  for (uint32_t i = 0; i < w; i++){
    point p0 = {origen.x+i, origen.y};
    point p1 = {origen.x+i, origen.y+h};
    line(img, color, p0, p1);
  }
}

int validate_cmds(int ac,
		  char *av[],
		  uint32_t *color,
		  uint32_t *w,
		  uint32_t *h) {
  int r = 0;
  *w	 = DEFAULT_IMG_SIZE;
  *h	 = DEFAULT_IMG_SIZE;
  *color = DEFAULT_BG_COLOR;
  if (ac < 4) {
    printf("Image default: %dx%d. Using default background color.\n",
	   DEFAULT_IMG_SIZE, DEFAULT_IMG_SIZE);
    return 0;
  } else { /* Parse width of image */
    if (validate_number(av[1], w, MAX_WIDTH, 10) < 0 ||
	validate_number(av[2], h, MAX_HEIGHT, 10) < 0) {
      *w = DEFAULT_IMG_SIZE;
      *h = DEFAULT_IMG_SIZE;
      r	 = -1;
    }
    if (validate_number(av[3], color, 0xFFFFFFFF, 16) < 0) {
      *color = DEFAULT_BG_COLOR;
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
  /* Instead of creating 3*w*h of uint8_t use w*h of
     uint32_t in order to be able to use alpha channel. */
  /* uint8_t *pixels = (uint8_t *)malloc(sizeof(uint8_t)*3*w*h); */
  uint32_t *pixels = (uint32_t *)malloc(sizeof(uint32_t)*w*h);

  if ( !header || !pixels ) {
    return -1;
  }

  setup_header(header, w, h);

  (*img).header	= header;
  (*img).pixels	= pixels;
  (*img).width	= w;
  (*img).height	= h;

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

  uint8_t *pixels_write = (uint8_t *)malloc(sizeof(uint8_t)*3*w*h);
  uint8_t r, g, b;
  for (uint32_t i = 0; i < w*h; i++){
    uint32_t pixel = img.pixels[i];
    b = pixel>>(8*2)&0xFF;
    g = pixel>>(8*1)&0xFF;
    r = pixel>>(8*0)&0xFF;
    
    pixels_write[3*i+0] = r;
    pixels_write[3*i+1] = g;
    pixels_write[3*i+2] = b;
  }

  fwrite(img.header, sizeof(char), HEADER_LEN, img_file);
  fwrite(pixels_write, sizeof(uint8_t), 3*w*h, img_file);
  fclose(img_file);
  return 0;
}

void print_point(point p, bool nl) {
  printf("(%.2f, %.2f)", p.x, p.y);
  if (nl)
    putchar('\n');
}

void triangle(ppm *img, uint32_t color, point p0, point p1, point p2) {
  /* TODO: make sure no two points are equal. If so draw the
     corresponding line (degenerated triangle). */
  /* Area of triangle, if zero draw line */
  float A = area(p0, p1, p2);
  /* look for smallest x and y coordinates */
  float minx = min(p0.x, min(p1.x, p2.x)) - 1;
  float miny = min(p0.y, min(p1.y, p2.y)) - 1;
  float maxx = max(p0.x, max(p1.x, p2.x)) + 1;
  float maxy = max(p0.y, max(p1.y, p2.y)) + 1;
  if (fabs(A) < 1) {
    /* draw vertival or horixontal line */
  }

  point p = {.x=minx, .y=maxy};
  for (p.y = miny; p.y <= maxy; p.y++) {
    for (p.x = minx; p.x <= maxx; p.x++){
      if (p.x >= 0 && p.y >= 0) {
	float gamma = area(p0, p1, p)/A;
	float beta  = area(p0, p, p2)/A;
	float alpha = area(p, p1, p2)/A;
	if (alpha >= 0 && beta >= 0 && gamma >= 0) {
	  if (alpha < 1 && beta < 1 && gamma < 1)
	    paint_pixel(img, color, p);
	}
      }
    }
  }
}

float area(point a, point b, point c) {
  /* return determinant of vectors v1 = b-a and v2 = c-a */
  return (b.x - a.x)*(c.y - a.y) - (c.x - a.x)*(b.y - a.y);
}

float min(float x, float y) {
  float t = -fabs(x-y) + x + y;
  return t/2;
}

float max(float x, float y) {
  float t = fabs(x-y) + x + y;
  return t/2;
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
 
void circle(ppm *img,
	    uint32_t color,
	    point center,
	    float radius) {
  point p;
  for (p.y = center.y - radius-1;   p.y < radius+center.y+1; p.y++) {
    for (p.x = center.x - radius-1; p.x < radius+center.x+1; p.x++) {
      if (p.x >= 0 && p.y >= 0) {
	float nx = p.x - center.x;
	float ny = p.y - center.y;
	if ( nx*nx + ny*ny < radius*radius )
	  paint_pixel(img, color, p);
      }
    }
  }
}

