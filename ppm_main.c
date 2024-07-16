#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cimple.h"


int main(int argc, char *argv[]) {
  FILE *img;
  char fname[] = "imagen.ppm";

  long width, height;
  uint32_t bg_color = 0x00FFFF; //yellow
  if (argc < 4) {
    /* Default image size of 32x32 pixels */
    width  = 32;
    height = 32;
    puts("Image default: 32x32. Color bg yellow.");
  } else { /* Parse width of image */
    width  = validate_number(argv[1], MAX_WIDTH, 10);
    height = validate_number(argv[2], MAX_HEIGHT, 10);
    bg_color  = validate_number(argv[3], 0xFFFFFFFF, 16);
    printf("Image size: %ldx%ld.\n", width, height);
  }

  if ((img = fopen(fname, "wb")) == NULL) {
    fprintf(stderr, "Unable to open %s.", fname);
    exit(1);
  }
  
  char *header = (char *)malloc(sizeof(char)*HEADER_LEN);
  uint8_t *data_buffer = (uint8_t *)malloc(sizeof(uint8_t)*3*width*height);
  
  setup_header(header, width, height);

  fill_background(data_buffer, bg_color, width, height);
  /*   255, 69, 0, // orange */
  /*   0, 0, 128, // navy blue */
  line(data_buffer, 0x800000, 0, 0, width, height/2, width, height);

  //g, r, b
  fwrite(header, sizeof(char), HEADER_LEN, img);
  fwrite(data_buffer, sizeof(uint8_t), 3*width*height, img);

  free(header);
  free(data_buffer);
  fclose(img);
  return 0;
}

void line(uint8_t *ppm, uint32_t color,
	  uint32_t x0, uint32_t y0,
	  uint32_t x1, uint32_t y1,
	  uint32_t w, uint32_t h) {
  int range, canonical = 0;
  float m;
  if ( x1 - x0 >= y1 - y0 ) {
    range = x1 - x0;
    m = (float)(y1-y0)/(x1-x0);
    canonical = 1;
  } else {
    range = y1-y0;
    m = (float)(x1-x0)/(y1-y0);
  }
  uint32_t lcoord, x, y;
  for(int i = 0; i < range; i++) {
    if (canonical) {
      x = i;
      y = round(m*x+y0);
    } else {
      y = i;
      x = round(m*y + x0);
    }
    lcoord = y*w + x; // coordenada lineal
    ppm[3*lcoord + 0] = (color>>(8*0))&0xFF;
    ppm[3*lcoord + 1] = (color>>(8*1))&0xFF;
    ppm[3*lcoord + 2] = (color>>(8*2))&0xFF;
  }
}
