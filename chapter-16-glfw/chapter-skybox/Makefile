obj=main.o texture.o glad.o
cxxflags= -std=c++11

vpath %.h ../public ../glad/include
vpath %.c ../glad/src
vpath %.cpp ../public

a.out:$(obj)
	g++ $(cxxflags) -framework OpenGL -lglfw \
		-o a.out $(obj)

main.o:main.cpp
	g++ $(cxxflags) -I../public -I../glad/include \
		-c main.cpp

glad.o:glad.c
	g++ $(cxxflags) -I../glad/include -c $<

texture.o:texture.cpp
	g++ $(cxxflags) -I../glad/include -c $<

.PHONY:clean
clean:
	-rm a.out $(obj)