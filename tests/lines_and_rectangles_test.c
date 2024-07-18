#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../headers/cimple.h"

int main(int argc, char *argv[]) {
  char fname[] = "lines_and_rectangles.ppm";
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
  point p0 = {width/2, 1}, p1 = {width/2, height};
  line(&img, 0x800000, p0, p1);
  p1.x = width; p1.y = height;
  line(&img, 0x800000, p0, p1);
  p0.x = 1; p0.y = 1;
  p1.x = width; p1.y = height;
  line(&img, 0xff00, p0, p1);

  /* linea de tres pixeles */
  p0.x = 1;
  p0.y = 5;
  p1.x = 5;
  p1.y = img.height;
  line(&img, 0xffffff, p0, p1);

  uint32_t rect_color = 0x00;
  point or = {.x=30, .y=25};
  rectangle(&img,
	    &rect_color,
	    or, 40, 30);  

  write_img_to_file(fname, img);
  free_ppm(&img);
  return 0;
}
  
