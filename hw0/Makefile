all: sobel

sobel: sobel.c
	# gcc sobel.c -o sobel -lm
	gcc -Wall -Werror -ggdb2 sobel.c -o sobel

clean:
	rm -f sobel *.dSYM *.edge.pgm edge.* sobel.pgm .DS_Store