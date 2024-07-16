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
  
void fill_background(uint8_t *ppm, uint32_t color, int w, int h) {
  for (int i = 0; i < 3*w*h; i+=3) {
    ppm[i+0] = (color>>(8*0))&0xFF;
    ppm[i+1] = (color>>(8*1))&0xFF;
    ppm[i+2] = (color>>(8*2))&0xFF;
  }
}
