light:light.c
	gcc -Wall -o light light.c -framework GLUT -framework OpenGL
run:
	./light
clean:
	rm light

