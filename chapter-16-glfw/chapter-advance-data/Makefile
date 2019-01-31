obj=main.o glad.o
cxxflags= -std=c++11
includepath= -I../glad/include

vpath %.h ../glad/include ../public
vpath %.c ../glad/src
vpath %.cpp ../public

a.out:$(obj)
	g++ -framework OpenGL -lglfw \
		-o a.out $(obj)

main.o:main.cpp
	g++ $(cxxflags) $(includepath) -c main.cpp

glad.o:glad.c
	g++ $(cxxflags) $(includepath) -c $<

PHONY:clean
clean:
	-rm a.out $(obj)