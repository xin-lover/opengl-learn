cxxflags = -I/home/xin/freeglut/include \
	   -I/home/xin/assimp/include \
	   -I../pulbic \
	   -L/home/xin/freeglut/lib64 \
	   -L/home/xin/mesa3d/lib64 \
	   -L/home/xin/mesa3d/lib \
	   -L/home/xin/assimp/lib \
	-lGL -lGLU -lglut -lGLEW -lassimp

vpath %.h ../public
vpath %.cpp ../public
#VPATH=../pulbic/
	   
app.out:mesh.o model.o main.o
	g++ -g -std=c++11 $(cxxflags) -o app.out mesh.o model.o main.o

main.o:main.cpp
	g++ -g -std=c++11 $(cxxflags) -c main.cpp

mesh.o:mesh.cpp mesh.h
	g++ -g -std=c++11 $(cxxflags)  -c $< 

model.o:model.cpp model.h
	g++ -g -std=c++11 $(cxxflags) -c $<

.PHONY:clean

clean:
	-rm main.o mesh.o model.o app.out


