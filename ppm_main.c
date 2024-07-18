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

  point center = {.x = width/2, .y=height/2};
  circle(&img, 0xff00, center, 150);
  
  
  write_img_to_file(fname, img);
  free_ppm(&img);
  return 0;
}
