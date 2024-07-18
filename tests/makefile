triangle_test : triangle_test.c cimple.o
	gcc -Wall -Wextra -o triangle_test triangle_test.c cimple.o -lm

lines_and_rectangles_test : lines_and_rectangles_test.c cimple.o
	gcc -Wall -Wextra -o lines_and_rectangles_test lines_and_rectangles_test.c cimple.o -lm

cimple.o : ../cimple.c ../headers/cimple.h
	gcc -Wall -Wextra -c ../cimple.c

clean :
	rm *_test *~ *.o *.h.gch *.ppm
