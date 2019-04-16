obj=main.o glad.o
cxxflags=-std=c++11

vpath %.h ../glad/include
vpath %.c ../glad/src

a.out:$(obj)
	g++ -framework OpenGL -lglfw -o a.out $(obj)

main.o:main.cpp
	g++ $(cxxflags) -c -I../glad/include main.cpp

glad.o:glad.c
	g++ $(cxxflags) -c -I../glad/include $<

PHONY:clean
clean:
	-rm a.out $(obj)