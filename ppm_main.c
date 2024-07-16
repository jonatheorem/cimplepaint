#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <math.h>
#include "cimple.h"

int main(int argc, char *argv[]) {
  char fname[] = "imagen.ppm";
  uint32_t width, height, bg_color;
  ppm img;

  int ok_cmds = validate_cmds(argc, argv, &bg_color, &width, &height);
  if (ok_cmds < 0) {
    fprintf(stderr, "Invalid arguments.\n");
    exit(1);
  }

  if ( open_img(&img, width, height) < 0 ) {
    fprintf(stderr, "Unable to open imagen.\n");
    exit(2);
  }
  
  point or = {.x=1, .y=1};
  
  fill_background(&img, bg_color);
  /*   255, 69, 0, // orange */
  /*   0, 0, 128, // navy blue */
  point p0 = {width/2, 1}, p1 = {width/2, height};
  line(&img, 0x800000, p0, p1);
  p1.x = width; p1.y = height;
  line(&img, 0x800000, p0, p1);
  p0.x = 1; p0.y = 1;
  line(&img, 0xff00, p0, p1);

  uint32_t rect_color = 0x00;
  rectangle(&img, &rect_color, or, 5, 9);

  write_img_to_file(fname, img);
  free_ppm(&img);
  return 0;
}
