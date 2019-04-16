obj=main.o glad.o texture.o
cxxflag=-std=c++11

vpath %.h ../glad/include
vpath %.c ../glad/src
vpath %.cpp ../public

a.out:$(obj)
	g++ -framework OpenGL -lglfw \
		$(obj) -o a.out
	
main.o:main.cpp
	g++ $(cxxflag) -I../glad/include -c main.cpp

glad.o:glad.c
	g++ $(cxxflag) -I../glad/include -c $<

texture.o:texture.cpp
	g++ $(cxxflag) -I../glad/include -c $<

PHONY:clean
clean:
	-rm $(obj) a.out