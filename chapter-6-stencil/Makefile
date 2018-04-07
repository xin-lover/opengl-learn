cxxflags = -I/home/xin/freeglut/include \
	   -L/home/xin/freeglut/lib64 \
	   -L/home/xin/mesa3d/lib64 \
	   -L/home/xin/mesa3d/lib \
	-lGL -lGLU -lglut -lGLEW  
	   
app.out:main.o
	g++ $(cxxflags) -o app.out main.o 

main.o:main.cpp
	g++ -c main.cpp

.PHONY:clean

clean:
	rm main.o app.out


