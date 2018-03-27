#ifndef HEADER_SHADERHELPER
#define HEADER_SHADERHELPER

#include <GL/glew.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 64
#define BUFFER_OFFSET(a) ((GLvoid*)a)

int readShader(const char* file,char** content)
{
	FILE *fp = fopen(file,"rb");
	if(fp == NULL)
	{
		printf("open %s failed.",file);
		return 1;
	}

	fseek(fp,0,SEEK_END);
	int len = ftell(fp);

	*content = (char*) malloc (len * sizeof(char));
	if(content == NULL)
	{
		printf("malloc fail...\n");
		return 2;
	}

	fseek(fp,0,SEEK_SET);
	fread(*content,len,1,fp);
	fclose(fp);
	return 0;
}


typedef struct
{
	GLuint type;
	char file[64];
} ShaderInfo;


GLuint LoadShaders(ShaderInfo shaders[])
{
	GLuint sArrays[3]= {0,0,0};
	int j;
	for(j = 0;j < 3; ++j)
	{
		if(shaders[j].type == GL_NONE)
		{
			continue;
		}

		sArrays[j] = glCreateShader(shaders[j].type);
		if(sArrays[j]== 0)
		{
			printf("create shader %d fail.\n",shaders[j].type);
		}

		char* filebuf = NULL;
		printf("file:%s\n",shaders[j].file);
		if(readShader(shaders[j].file,&filebuf) != 0)
		{
			continue;
		}

		printf("%s\n",filebuf);
		glShaderSource(sArrays[j],1,(const GLchar**)&filebuf,NULL);
		glCompileShader(sArrays[j]);

		int success;
		glGetShaderiv(sArrays[j],GL_COMPILE_STATUS,&success);
		if(!success)
		{
			printf("compile shader fail.\n");
			char info[256];
			glGetShaderInfoLog(sArrays[j],256,NULL,info);
			printf("%s\n",info);
		}
	}
	
	GLuint prog = glCreateProgram();
	int i;
	for(i=0;i < 3; ++i)
	{
		if(sArrays[i] > 0)
		{
			glAttachShader(prog,sArrays[i]);

		}
	}
	glLinkProgram(prog);
	int success;
	glGetProgramiv(prog,GL_LINK_STATUS,&success);
	if(!success)
	{
		char buf[256];
		glGetProgramInfoLog(prog,256,NULL,buf);
		printf("link program error..\n %s",buf);
		return 0;
	}

	return prog;
	
		
} 

#endif
