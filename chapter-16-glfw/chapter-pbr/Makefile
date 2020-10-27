obj=main.o glad.o
cxxflags=-std=c++11

vpath %.c ../glad/src
vpath %.h ../glad/include

a.out:$(obj)
	g++ $(cxxflags) -framework OpenGL -lglfw \
		$(obj) -o a.out

main.o:main.cpp
	g++ $(cxxflags) -I../glad/include -c main.cpp

glad.o:glad.c
	g++ $(cxxflags) -I../glad/include -c $<

PHONY:clean
clean:
	-rm a.out $(obj)