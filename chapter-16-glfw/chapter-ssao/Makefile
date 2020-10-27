obj=main.o glad.o texture.o model.o mesh.o
cxxflags=-std=c++11

vpath %.h ../glad/include ~/OpenSource/assimp
vpath %.c ../glad/src
vpath %.cpp ../public

a.out:$(obj)
	g++ $(cxxflags) -framework OpenGL -lglfw -L/Users/yichen/OpenSource/assimp/lib -lassimp \
		-o a.out $(obj)

main.o:main.cpp
	g++ $(cxxflags) -I../glad/include -I/Users/yichen/OpenSource/assimp/include -c main.cpp

glad.o:glad.c
	g++ $(cxxflags) -I../glad/include -c $<

texture.o:texture.cpp
	g++ $(cxxflags) -I../glad/include -c $<

mesh.o:mesh.cpp
	g++ $(cxxflags) -I../glad/include -c $<

model.o:model.cpp
	g++ $(cxxflags) -I../glad/include -I/Users/yichen/OpenSource/assimp/include -L/Users/yichen/OpenSource/assimp/lib -lassimp -c $<

PHONY:clean
clean:
	-rm a.out $(obj)