box:main.o
	g++ -I/home/xin/mesa3d/include -I/home/xin/freeglut/include -L/home/xin/mesa3d/lib -L/home/xin/mesa3d/lib64 -L/home/xin/freeglut/lib64 -lGL -lGLU -lglut -lGLEW -o box main.o

main.o:main.cpp
	g++ -c main.cpp

.PHONY:clean

clean:
	rm -rf main.o box
