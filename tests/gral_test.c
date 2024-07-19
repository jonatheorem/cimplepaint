#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "headers/cimple.h"

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
  
  fill_background(&img, bg_color);
  /*   255, 69, 0, // orange */
  /*   0, 0, 128, // navy blue */
  /* 0x8C9579 nice gray */
  uint32_t navy_blue = 0x800000;

  point center = {.x = (float)width/2, .y=(float)height/2};
  circle(&img, navy_blue, center,(float)width/3);
  
  point p1 = {.x=5, .y=5};
  point p2 = {.x=width-5, .y=(float)height/3};
  point p3 = {.x=(float)width/2, .y=(float)height/2};
  triangle(&img, 0xff, p1, p2, p3);

  uint32_t orange = 0x44ff;
  point p4 = {.x=10, .y=height*.55};
  rectangle(&img, &orange, p4, width/2, height/3);
  
  write_img_to_file(fname, img);
  free_ppm(&img);
  return 0;
}
