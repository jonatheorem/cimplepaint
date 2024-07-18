#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../headers/cimple.h"

int main(int argc, char *argv[]) {
  char fname[] = "triangles.test.ppm";
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
  point p0 = {width/2, 1}, p1 = {width/2, height};
  
  p0.x = 200; p0.y = 10;
  p1.x = 10; p1.y = 220;
  point p2 = {.x=340, .y=340};
  triangle(&img, 0x8C9579, p0, p1, p2);  

  write_img_to_file(fname, img);
  free_ppm(&img);
  return 0;
}
 
