#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cimple.h"

int main(int argc, char *argv[]) {
  FILE *img_file;
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

  if ((img_file = fopen(fname, "wb")) == NULL) {
    fprintf(stderr, "Unable to open %s.", fname);
    exit(1);
  }
  
  char *header = (char *)malloc(sizeof(char)*HEADER_LEN);
  uint8_t *data_buffer = (uint8_t *)malloc(sizeof(uint8_t)*3*width*height);
  
  setup_header(header, width, height);

  ppm img = {.header=header,
	     .pixels=data_buffer,
	     .width=width,
	     .height=height};
  point or = {.x=1, .y=1};
  
  fill_background(data_buffer, bg_color, width, height);
  /*   255, 69, 0, // orange */
  /*   0, 0, 128, // navy blue */
  point p0 = {(uint32_t)width/2, 1}, p1 = {(uint32_t)width/2, height};
  line(&img, 0x800000, p0, p1);
  p1.x = width; p1.y = height;
  line(&img, 0x800000, p0, p1);
  p0.x = 1; p0.y = 1;
  line(&img, 0xff00, p0, p1);

  uint32_t red = 0xff;
  rectangle(&img, &red, or, 5, 5);

  //g, r, b
  fwrite(header, sizeof(char), HEADER_LEN, img_file);
  fwrite(data_buffer, sizeof(uint8_t), 3*width*height, img_file);

  free(header);
  free(data_buffer);
  fclose(img_file);
  return 0;
}
