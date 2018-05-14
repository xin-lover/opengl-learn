objs=texture.o mesh.o model.o main.o
appname=app.out

option= -std=c++11

vpath %.h ../public
vpath %.cpp ../public
vpath %.hpp /home/xin/assimp/include

app.out:$(objs)
	g++ -std=c++11 -I/home/xin/mesa3d/include \
		-I/home/xin/freeglut/include \
		-I/home/xin/assimp/include \
		-I/../public \
		-L/home/xin/mesa3d/lib \
		-L/home/xin/mesa3d/lib64 \
		-L/home/xin/freeglut/lib64 \
		-L/home/xin/assimp/lib \
	 	-lGL -lGLU -lGLEW -lglut -lassimp \
	-o app.out $(objs)

main.o:main.cpp
	g++ $(option) -I/home/xin/assimp/include -c $<

model.o:model.cpp model.h
	g++ $(option) -I/home/xin/assimp/include -c $<

mesh.o:mesh.cpp mesh.h
	g++ $(option) -c $<

texture.o:texture.cpp texture.h
	g++ $(option) -c $<

.PHONY:clean
clean:
	-rm app.out $(objs)
