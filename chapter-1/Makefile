
ifeq ($(shell uname),"Darwin")
	libs:=-lGL -lGLU -lglut
else
	libs:=-framework OpenGL -framework GLUT
endif
		

teapot:teapot.cpp
	gcc ${libs} -o teapot teapot.cpp

.PHONY : clean

clean:
	rm teapot
