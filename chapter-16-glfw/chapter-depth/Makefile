obj=main.o glad.o texture.o
cxxFlags= -std=c++11

vpath %.h ../glad/include
vpath %.c ../glad/src
vpath %.cpp ../public

a.out:$(obj)
	g++ $(cxxFlags) -framework OpenGL -lglfw \
	 -o a.out $(obj)

main.o:main.cpp
	g++ $(cxxFlags) -I../glad/include \
	 -c main.cpp

glad.o:glad.c
	g++ -c  -I../glad/include $<

texture.o:texture.cpp
	g++ -c -I../glad/include $<


PHONY:clean
clean:
	-rm a.out $(obj)