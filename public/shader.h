#ifndef __SHADER_HEAD__
#define __SHADER_HEAD__

#include <GL/glew.h>
#include <GL/gl.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>

using namespace std;

#define Error(...) printf(__VA_ARGS__)

class Shader
{
	public:
		Shader():m_vShader(0),
			m_fShader(0),
			m_program(0)
		{}

		Shader(string vertexShaderPath, string fragmentShaderPath)
		{

			m_vShader = CreateShader(GL_VERTEX_SHADER,vertexShaderPath);
			m_fShader = CreateShader(GL_FRAGMENT_SHADER,fragmentShaderPath);
			m_program = glCreateProgram();
			glAttachShader(m_program,m_vShader);
			glAttachShader(m_program,m_fShader);
			glLinkProgram(m_program);
			int success;
			glGetProgramiv(m_program,GL_LINK_STATUS,&success);
			if(!success)
			{
				char buf[256];
				glGetProgramInfoLog(m_program,256,NULL,buf);
				Error("link program error..\n %s",buf);
			}
		}

		~Shader()
		{
			glDeleteShader(m_vShader);
			glDeleteShader(m_fShader);
			glDeleteProgram(m_program);
		}

		Shader(const Shader &other) = delete;
		void operator=(const Shader &other) = delete;

		void Use()
		{
			glUseProgram(m_program);
		}

		void SetInt(string name, int value)
		{
			GLint loc = glGetUniformLocation(m_program,name.c_str());
			if(loc == -1)
			{
				Error("can't find %s in shader.\n",name.c_str());
				return;
			}

			glUniform1i(loc,value);
		}

		void SetFloat(string name,float value)
		{
			GLint loc = glGetUniformLocation(m_program,name.c_str());
			if(loc == -1)
			{
				Error("can't find %s in shader.\n",name.c_str());
				return;
			}

			glUniform1f(loc,value);
		}

		void SetMatrix(string name, float* data)
		{
			GLint loc = glGetUniformLocation(m_program,name.c_str());
			if(loc == -1)
			{
				Error("can't find %s in shader.\n",name.c_str());
				return;

			}

			glUniformMatrix4fv(loc,1,GL_FALSE,data);

		}

	private:
		GLuint m_program;
		GLuint m_vShader;
		GLuint m_fShader;

		char* readShader(const char* file)
		{
			FILE *fp = fopen(file,"rb");
			if(fp == NULL)
			{
				Error("open %s failed.",file);
				return NULL;
			}

			fseek(fp,0,SEEK_END);
			int len = ftell(fp);

			char *content = (char*) malloc ((len + 1) * sizeof(char));
			if(content == NULL)
			{
				Error("malloc fail...\n");
				exit(1);
			}
			memset(content,0,len+1);

			fseek(fp,0,SEEK_SET);
			fread(content,1,len, fp);
			fclose(fp);
			return content;
		}

		GLuint CreateShader(GLenum type, string path)
		{
			GLuint shader = glCreateShader(type);
			if(shader == 0)
			{
				Error("create shader %d fail.\n",type);
			}

			Error("file:%s\n",path.c_str());
			char* filebuf = readShader(path.c_str());

			Error("%s\n",filebuf);
			glShaderSource(shader,1,(const GLchar**)&filebuf,NULL);
			glCompileShader(shader);

			int success;
			glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
			if(!success)
			{
				Error("compile shader fail.\n");
				char info[256];
				glGetShaderInfoLog(shader,256,NULL,info);
				Error("%s\n",info);
			}

			free(filebuf);

			return shader;
		}
};

#endif
