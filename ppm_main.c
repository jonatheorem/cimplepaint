#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
  
  fill_background(&img, bg_color);
  /*   255, 69, 0, // orange */
  /*   0, 0, 128, // navy blue */
  point p0 = {width/2, 1}, p1 = {width/2, height};
  /* line(&img, 0x800000, p0, p1); */
  /* p1.x = width; p1.y = height; */
  /* line(&img, 0x800000, p0, p1); */
  /* p0.x = 1; p0.y = 1; */
  /* p1.x = width; p1.y = height; */
  /* line(&img, 0xff00, p0, p1); */

  // linea de tres pixeles
  /* p0.x = 1; */
  /* p0.y = 5; */
  /* p1.x = 5; */
  /* p1.y = img.height; */
  /* line(&img, 0xffffff, p0, p1); */

  /* uint32_t rect_color = 0x00; */
  /* point or = {.x=30, .y=25}; */
  /* rectangle(&img, */
  /* 	    &rect_color, */
  /* 	    or, 40, 30); */

  /* for (int i = 0; i < 10; i++) { */
  /*   p0.x = 1+i; p0.y = height-1; */
  /*   p1.x = 1+i; p1.y = 1+i; */
  /*   /\* point p2 = {.x=10, .y=10}; *\/ */
  /*   line(&img, 0xffffff, p0, p1); */
  /*   /\* line(&img, 0xffffff, p0, p2); *\/ */
  /*   /\* line(&img, 0xffffff, p1, p2); *\/ */
  /* } */

  /* point a = {.x=5, .y=0}; */
  /* point b = {.x=2, .y=5}; */
  /* for (int i = 0; i < 5; i++) { */
  /*   point u, v; */
  /*   u.x = a.x + i; u.y = a.y + i; */
  /*   v.x = b.x + i; v.y = b.y + i; */

  /*   //if (i == 2) { */
  /*     line(&img, 0xff00, u, v); */
  /*     paint_pixel(&img, 0xff, u); */
  /*     paint_pixel(&img, 0xff0000, v); */
  /*     //} */
  /*     printf("linea de "); */
  /*     print_point(u, false); */
  /*     printf(" a "); */
  /*     print_point(v,true); */
  /* } */
  /* a.x = 9; a.y = 4; */
  /* b.x = 6; b.y = 9; */
  /* /\* a.x = 9; a.y = 0; *\/ */
  /* /\* b.x = 6; b.y = 5; *\/ */
  /* line(&img, 0x00, a, b); */
  
  p0.x = 200; p0.y = 1;
  p1.x = 1; p1.y = 350;
  point p2 = {.x=598, .y=550};
  triangle(&img, 0x00, p0, p1, p2);  

  write_img_to_file(fname, img);
  free_ppm(&img);
  return 0;
}
  
