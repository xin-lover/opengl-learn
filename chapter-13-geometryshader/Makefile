target=app.out

objs=texture.o main.o 

vpath %.h ../public
vpath %.cpp ../public

$(target):$(objs)
	g++ -I/home/xin/freeglut/include \
		-I/home/xin/mesa3d/include \
		-L/home/xin/mesa3d/lib \
		-L/home/xin/mesa3d/lib64 \
		-L/home/xin/freeglut/lib64 \
		-lGL -lGLU -lGLEW -lglut \
	-o $(target) $(objs)

main.o:main.cpp
	g++ -std=c++11 -c main.cpp

texture.o:texture.cpp
	g++ -std=c++11 -c $<

PHONY:clean
clean:
	-rm app.out $(objs)
