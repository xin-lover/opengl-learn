CFLAGS=	-g \
      	 -L/home/xin/gl/glew/lib \
	  -lGL \
	  -lGLU \
	  -lglut \
	  -lGLEW \

testbuffer:useframebuffer.o
	gcc  $(CFLAGS) -o testbuffer useframebuffer.o

useframebuffer.o:ShaderHelper.h useframebuffer.cpp
	gcc -g -c ShaderHelper.h useframebuffer.cpp

.PHONY : clean

clean:
	rm -rf useframebuffer.o testbuffer

