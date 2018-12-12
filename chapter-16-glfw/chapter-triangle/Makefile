vpath %.c ../glad/src
vpath $.h ../glad/include

a.out:main.o glad.o
		g++  -framework OpenGL -lglfw \
				-o a.out main.o glad.o

glad.o:glad.c
		g++ -c -I../glad/include $<

main.o:main.cpp
		g++ -c -I../glad/include \
				$<

.PHONY:clean
clean:
		rm a.out main.o glad.o
