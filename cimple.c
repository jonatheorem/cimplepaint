#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

#include "cimple.h"

// define boleanos as enum
typedef enum _bool { false, true } bool;

void setup_header(char *header, long w, long h) {
  sprintf(header, "P6\n%04ld %04ld\n255\n", w, h);
}

long validate_number(char *nstr, long max, int base) {
  char *buff_ptr = (char *)malloc(sizeof(char)*INT_BUFFER);
  char *endstr = buff_ptr;
  bool valid = true;
  long num;

  /* If nstr exceeds max lenght then is not valid */
  if ( strlen(nstr) > ceil(log10(max+1)) ) {
    valid = false;
  } else {
    /* If does exceed max. try to convert */
    num = strtol(nstr, &endstr, base);
    if (*endstr != '\0')
      valid = false;
  }
  /* If the number is longer than `max or not valid string explode */
  if ( !valid ) {
    fprintf(stderr,
	    "Not valid argument. "
	    "Enter integers less than %ld.\n", max);
    /* ... bit first we free up memory used :3 */
    free(buff_ptr);
    exit(1);
  }

  free(buff_ptr);
  return num;
}
  
void fill_background(uint8_t *ppm_, uint32_t color, int w, int h) {
  for (int i = 0; i < 3*w*h; i+=3) {
    ppm_[i+0] = (color>>(8*0))&0xFF;
    ppm_[i+1] = (color>>(8*1))&0xFF;
    ppm_[i+2] = (color>>(8*2))&0xFF;
  }
}

void line(ppm *img, uint32_t color, point p0, point p1) {
  int range, canonical = 0;
  float m;
  /* Interchange to make sure x0, y0 < x1, y1 */
  if (p1.x < p0.x) { uint32_t t = p1.x; p1.x = p0.x; p0.x = t; }
  if (p1.y < p0.y) { uint32_t t = p1.y; p1.y = p0.y; p0.y = t; }
  /* not draw anything if out of bounds points */
  //if (x0 < 0 || y0 < 0) return;
  if (p1.x > img->width || p1.y > img->height) {
    return;
  }
  /* use as parameter the one with longest range */
  if ( p1.x - p0.x >= p1.y - p0.y ) {
    range = p1.x - p0.x;
    m = (float)(p1.y-p0.y)/(p1.x-p0.x);
    canonical = 1;
  } else {
    range = p1.y-p0.y;
    m = (float)(p1.x-p0.x)/(p1.y-p0.y);
  }
  uint32_t lcoord, x, y;
  for(int i = 0; i < range; i++) {
    /* draw line depending on canonical case y=mx+b or x = my+b */
    if (canonical) {
      x = i+p0.x;
      y = round(m*x+p0.y);
    } else {
      y = i+p0.y;
      x = round(m*y + p0.x);
    }
    lcoord = y*img->width + x; // lineal coordinate
    img->pixels[3*lcoord + 0] = (color>>(8*0))&0xFF;
    img->pixels[3*lcoord + 1] = (color>>(8*1))&0xFF;
    img->pixels[3*lcoord + 2] = (color>>(8*2))&0xFF;
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
