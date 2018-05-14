cxxflags = -I/home/xin/freeglut/include \
	   -I/home/xin/assimp/include \
	   -I../pulbic \
	   -L/home/xin/freeglut/lib64 \
	   -L/home/xin/mesa3d/lib64 \
	   -L/home/xin/mesa3d/lib \
	   -L/home/xin/assimp/lib \
	-lGL -lGLU -lglut -lGLEW -lassimp

objs = main.o mesh.o model.o texture.o
ops = -g -std=c++11

vpath %.h ../public
vpath %.cpp ../public
#VPATH=../pulbic/
	   
app.out:$(objs)
	g++ $(ops) $(cxxflags) -o app.out $(objs)

main.o:main.cpp
	g++ $(ops) $(cxxflags) -c main.cpp

mesh.o:mesh.cpp mesh.h
	g++ $(ops) $(cxxflags) -c $< 

model.o:model.cpp model.h
	g++ $(ops) $(cxxflags) -c $<

texture.o:texture.cpp texture.h
	g++ $(ops) $(cxxflags) -c $<

.PHONY:clean

clean:
	-rm $(objs) app.out


