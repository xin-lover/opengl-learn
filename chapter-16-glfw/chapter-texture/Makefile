vpath %.cpp ../public
vpath %.c ../glad/src
vpath $.h ../glad/include
#vpath $.h ../public

objs=main.o glad.o texture.o

cxxflags=-std=c++1y

a.out:$(objs)
		g++ $(cxxflags) -framework OpenGL -lglfw \
				-o a.out ${objs}

glad.o:glad.c
		g++ -c -I../glad/include $<

main.o:main.cpp
		g++ $(cxxflags) -c -I../glad/include $<

texture.o:texture.cpp
		g++ $(cxxflags) -c -I../glad/include -I../public $<

.PHONY:clean
clean:
		rm a.out $(objs)
