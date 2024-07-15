#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* max width and height of images in pixels */
#define MAX_WIDTH  9999
#define MAX_HEIGHT 9999
#define HEADER_LEN 17
#define INT_BUFFER 64   // buffer size to store strings representing integers

// define los boleanos como enum
enum _bool { false, true };
typedef enum _bool bool;

void setup_header(char *header, long w, long h);
long validate_number(char *nstr, long max, int base);
void fill_background(uint8_t *ppm, uint32_t color, int w, int h);

int main(int argc, char *argv[]) {
  FILE *img;
  char fname[] = "imagen.ppm";

  long width, height;
  uint32_t color = 0xFF00FF; //yellow
  if (argc < 3) {
    /* Default image size of 32x32 pixels */
    width  = 32;
    height = 32;
    puts("Image default: 32x32. Color bg yellow.");
  } else { /* Parse width of image */
    width  = validate_number(argv[1], MAX_WIDTH, 10);
    height = validate_number(argv[2], MAX_HEIGHT, 10);
    //color  = validate_number(argv[3], 0xFFFFFFFF, 16);
    printf("Image size: %ldx%ld.\n", width, height);
  }

  if ((img = fopen(fname, "wb")) == NULL) {
    fprintf(stderr, "Unable to open %s.", fname);
    exit(1);
  }
  
  char *header = (char *)malloc(sizeof(char)*HEADER_LEN);
  uint8_t *data_buffer = (uint8_t *)malloc(sizeof(uint8_t)*3*width*height);
  
  setup_header(header, width, height);

  //r, b, g
  fill_background(data_buffer, color, width, height);
  /*   255, 69, 0, // orange */
  /*   0, 0, 128, // navy blue */

  //g, r, b

  fwrite(header, 1, HEADER_LEN, img);
  fwrite(data_buffer, sizeof(char), 3*width*height, img);

  free(header);
  free(data_buffer);
  fclose(img);
  return 0;
}

void setup_header(char *header, long w, long h) {
  sprintf(header, "P6\n%ld %ld\n255\n", w, h);
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
    ppm[i]   = (color>>8*0)&0xFF;
    ppm[i+1] = (color>>8*1)&0xFF;
    ppm[i+2] = (color>>8*2)&0xFF;
  }
}
