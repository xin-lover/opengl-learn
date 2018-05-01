objs=main.o
appname=app.out

option= -std=c++11

app.out:$(objs)
	g++ -std=c++11 -I/home/xin/mesa3d/include \
		-I/home/xin/freeglut/include \
		-L/home/xin/mesa3d/lib \
		-L/home/xin/mesa3d/lib64 \
		-L/home/xin/freeglut/lib64 \
	 	-lGL -lGLU -lGLEW -lglut \
	-o app.out $(objs)

main.o:main.cpp
	g++ $(option) -c main.cpp

.PHONY:clean
clean:
	-rm app.out $(objs)
