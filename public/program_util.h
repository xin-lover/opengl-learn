#include <GL/gl.h>
#include <stdio.h>

void SetProgramInt(GLuint prog, const char* name, int value)
{
	GLint loc = glGetUniformLocation(prog,name);
	if(loc == -1)
	{
		printf("get location fail. name: %s\n",name);
		return;
	}

	glUniform1i(loc,value);
}

void SetProgramFloat(GLuint prog,const char* name, float value)
{
	GLint loc = glGetUniformLocation(prog,name);
	if(loc == -1)
	{
		printf("get location error. name:%s\n",name);
		return;
	}

	glUniform1f(loc,value);
}

void SetProgramVec3(GLuint prog, const char* name, float v1,float v2, float v3)
{
	GLint loc = glGetUniformLocation(prog,name);
	if(loc == -1)
	{
		printf("get location fail. name: %s\n",name);
		return;
	}

	glUniform3f(loc,v1,v2,v3);
}
