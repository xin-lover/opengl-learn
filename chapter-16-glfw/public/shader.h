#ifndef __SHADER_HEAD__
#define __SHADER_HEAD__

//#include <GL/glew.h>
#include <glad/glad.h>
#include <string.h>
#include <string>
#include <cstdlib>
#include <cstdio>

#include "logger.h"

using namespace std;

namespace luwu
{
class Shader
{
	public:
		Shader():m_vShader(0),
			m_fShader(0),
			m_program(0),
			m_programValid(false)
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
			m_programValid = success;
			if(!success)
			{
				char buf[256];
				glGetProgramInfoLog(m_program,256,NULL,buf);
				Logger::Log("link program error..\n %s",buf);
			}
		}

		Shader(string vertexShaderPath, string fragmentShaderPath, string geometryShaderPath)
		{
			m_vShader = CreateShader(GL_VERTEX_SHADER,vertexShaderPath);
			m_fShader = CreateShader(GL_FRAGMENT_SHADER,fragmentShaderPath);
			m_gShader = CreateShader(GL_GEOMETRY_SHADER,geometryShaderPath);
			m_program = glCreateProgram();
			glAttachShader(m_program,m_vShader);
			glAttachShader(m_program,m_fShader);
			glAttachShader(m_program,m_gShader);
			glLinkProgram(m_program);
			int success;
			glGetProgramiv(m_program,GL_LINK_STATUS,&success);
			m_programValid=success;
			if(!success)
			{
				char buf[256];
				glGetProgramInfoLog(m_program,256,NULL,buf);
				Logger::Error("link program error..\n %s",buf);
			}

		}

		~Shader()
		{
			glDeleteShader(m_vShader);
			glDeleteShader(m_fShader);
			glDeleteShader(m_gShader);
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
				Logger::Log("can't find %s in shader.\n",name.c_str());
				return;
			}

			glUniform1i(loc,value);
		}

		void SetFloat(string name,float value)
		{
			GLint loc = glGetUniformLocation(m_program,name.c_str());
			if(loc == -1)
			{
				Logger::Log("can't find %s in shader.\n",name.c_str());
				return;
			}

			glUniform1f(loc,value);
		}

		void SetMatrix(string name, float* data)
		{
			GLint loc = glGetUniformLocation(m_program,name.c_str());
			if(loc == -1)
			{
				Logger::Log("can't find %s in shader. program:%d\n",name.c_str(),m_program);
				return;

			}

			glUniformMatrix4fv(loc,1,GL_FALSE,data);

		}

		void SetMatrix(string name, int count,float* data)
		{
			GLint loc = glGetUniformLocation(m_program,name.c_str());
			if(loc == -1)
			{
				Logger::Log("can't find %s in shader. program:%d\n",name.c_str(),m_program);
				return;

			}

			glUniformMatrix4fv(loc,count,GL_FALSE,data);

		}

		void SetVec4(string name, float v1, float v2, float v3, float v4)
		{
			GLint loc = glGetUniformLocation(m_program,name.c_str());
			if(loc == -1)
			{
				Logger::Log("can't find %s in shader. program:%d\n",name.c_str(),m_program);
				return;

			}

			glUniform4f(loc,v1,v2,v3,v4);
		}

		void SetVec3(string name, float v1, float v2, float v3)
		{
			GLint loc = glGetUniformLocation(m_program,name.c_str());
			if(loc == -1)
			{
				Logger::Log("can't find %s in shader. program:%d\n",name.c_str(),m_program);
				return;

			}

			glUniform3f(loc,v1,v2,v3);
		}

		void SetVec2(string name, float v1, float v2)
		{
			GLint loc = glGetUniformLocation(m_program,name.c_str());
			if(loc == -1)
			{
				Logger::Log("can't find %s in shader. program:%d\n",name.c_str(),m_program);
				return;

			}

			glUniform2f(loc,v1,v2);
		}

		int GetAttribLocation(string name)
		{
			return glGetAttribLocation(m_program,name.c_str());
		}

	private:
		GLuint m_program;
		GLuint m_vShader;
		GLuint m_fShader;
		GLuint m_gShader;

		bool m_programValid;

		static char* readShader(const char* file)
		{
			FILE *fp = fopen(file,"rb");
			if(fp == NULL)
			{
				Logger::Log("open %s failed.",file);
				return NULL;
			}

			if(fseek(fp,0,SEEK_END) == 0)
			{
				int len = ftell(fp);

				char *content = (char*) malloc ((len + 1) * sizeof(char));
				if(content == NULL)
				{
					Logger::Fatal("malloc fail...\n");
					exit(1);
				}
				memset(content,0,len+1);

				fseek(fp,0,SEEK_SET);
				fread(content,1,len, fp);
				fclose(fp);
				return content;
			}
			else
			{
				Logger::Error("fseek error..\n");
				return NULL;
			}
		}

		GLuint CreateShader(GLenum type, string path)
		{
			GLuint shader = glCreateShader(type);
			if(shader == 0)
			{
				Logger::Error("create shader %d fail.\n",type);
			}

			Logger::Log("Compile Shader file:%s\n",path.c_str());
			char* filebuf = readShader(path.c_str());
			if(filebuf == NULL)
			{
				Logger::Error("read shader file %s fail..\n",path.c_str());
				return 0;
			}

			glShaderSource(shader,1,(const GLchar**)&filebuf,NULL);
			glCompileShader(shader);

			int success;
			glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
			if(!success)
			{
				char info[256];
				glGetShaderInfoLog(shader,256,NULL,info);
				Logger::Error("compile shader fail. %s\n",info);
			}

			free(filebuf);

			return shader;
		}
};
}
#endif
