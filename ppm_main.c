#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* max width and height of images in pixels */
#define MAX_WIDTH  9999
#define MAX_HEIGHT 9999
#define HEADER_LEN 17   // Len of header of ppm file
#define INT_BUFFER 64   // buffer size to store strings representing integers

/* Using the fact that to convert digits from number to char
 * in ascii it is necessary to add 48. Check of this also
 * happens in other encondings.
 */
#define first_digit(n)   (((int)(n) / 1000) + 48)
#define second_digit(n)  (((int)((n) % 1000) / 100) + 48)
#define third_digit(n)   (((int)((n) % 100) / 10) + 48)
#define fourth_digit(n)  (((int)((n) % 10)) + 48)


// define los boleanos como enum
enum _bool { false, true };
typedef enum _bool bool;

void setup_header(char **header, long w, long h);
long validate_number(char *nstr, long max);
void fill_background(char **ppm, int r, int g, int b, int w, int h);

int main(int argc, char *argv[]) {
  FILE *img;
  char fname[] = "imagen.ppm";

  long width, height;
  if (argc < 3) {
    /* Default image size of 32x32 pixels */
    width  = 32;
    height = 32;
    puts("Image default: 32x32.");
  } else { /* Parse width of image */
    width  = validate_number(argv[1], MAX_WIDTH);
    height = validate_number(argv[2], MAX_HEIGHT);
    printf("Image size: %ldx%ld.\n", width, height);
  }

  if ((img = fopen(fname, "wb")) == NULL) {
    fprintf(stderr, "Unable to open %s.", fname);
    exit(1);
  }
  
  char *header = (char *)malloc(sizeof(char)*HEADER_LEN);
  
  setup_header(&header, width, height);

  char *data_buffer = (char *)malloc(sizeof(char)*3*width*height);

  fill_background(&data_buffer, 255, 255, 0, width, height);
  /*   255, 69, 0, // orange */
  /*   0, 0, 128, // navy blue */

  fwrite(header, 1, HEADER_LEN, img);
  fwrite(data_buffer, sizeof(char), 3*width*height, img);

  free(header);
  free(data_buffer);
  fclose(img);
  return 0;
}

void setup_header(char **header, long w, long h) {
  char wstr[6] = {
    first_digit(w),
    second_digit(w),
    third_digit(w),
    fourth_digit(w),
    ' ', // added space from here
  };
  char hstr[5] = {
    first_digit(h),
    second_digit(h),
    third_digit(h),
    fourth_digit(h),
    '\n' // added new line from here
  };
  strncpy(*header, "P6\n", 4); 
  strncpy(*header + 3, wstr, 5);
  strncpy(*header + 8, hstr, 5);
  strncpy(*header + 13, "255\n", 5);
}

long validate_number(char *nstr, long max) {
  char *buff_ptr = (char *)malloc(sizeof(char)*INT_BUFFER);
  char *endstr = buff_ptr;
  bool valid = true;
  long num;

  /* If nstr exceeds max lenght then is not valid */
  if ( strlen(nstr) > ceil(log10(max+1)) ) {
    valid = false;
  } else {
    /* If does exceed max. try to convert */
    num = strtol(nstr, &endstr, 10);
    if (*endstr != '\0')
      valid = false;
  }
  /* If the number is longer than `max or not valid string explode */
  if ( !valid ) {
    fprintf(stderr,
	    "Not valid image dimensions. "
	    "Enter integers less than %ld.\n", max);
    /* ... bit first we free up memory used :3 */
    free(buff_ptr);
    exit(1);
  }

  free(buff_ptr);
  return num;
}
  
void fill_background(char **ppm, int r, int g, int b, int w, int h) {
  for (int i = 0; i < 3*w*h; i+=3) {
    (*ppm)[i]   = r;
    (*ppm)[i+1] = g;
    (*ppm)[i+2] = b;
  }
}
