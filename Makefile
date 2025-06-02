all: main

main: main.c codec.o image.o quadtree.o
	gcc -o main.exe main.c codec.o image.o quadtree.o

codec.o: codec.c codec.h quadtree.h
	gcc -c codec.c -o codec.o

image.o: image.c image.h
	gcc -c image.c -o image.o

quadtree.o: quadtree.c quadtree.h
	gcc -c quadtree.c -o quadtree.o

clean:
	del main.exe *.o *.bin *_recovered.ppm

