ppm_main : ppm_main.c cimple.o	
	gcc -Wall -Wextra -o ppm_main ppm_main.c cimple.o -lm

debug : ppm_main.c cimple.c	
	gcc -Wall -Wextra -g -o ppm_main.debug ppm_main.c cimple.c -lm

cimple.o : cimple.c ./headers/cimple.h
	gcc -Wall -Wextra -c cimple.c 

clean :
	rm ppm_main *~ *.o *.h.gch *.ppm *.debug
