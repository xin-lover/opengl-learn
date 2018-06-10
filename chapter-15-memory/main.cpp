#include <GL/glew.h>
#include <GL/freeglut.h>
#include <memory>

#include "../public/shader.h"
#include "../public/threemath.h"

bool isInit = false;
void Display();

GLuint vao;
GLuint vbo;

GLuint tex;
GLuint otex;
GLuint obuf;

std::shared_ptr<Shader> dShader;
std::shared_ptr<Shader> tShader;

Matrix4x4 modelMat;

int main(int argc, char* argv[])
{
	glutInit(&argc,argv);
	glutInitContextVersion(4,3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(640,480);
	glutCreateWindow("memory");

	printf("%s\n",gluErrorString(glGetError()));

	glewExperimental = true;
	if(glewInit() != GLEW_OK)
	{
		printf("glew init fail.\n");
		exit(1);
	}

	glutDisplayFunc(Display);

	glutMainLoop();

	return 0;
}

void Display()
{
	if(!isInit)
	{
		isInit = true;

		glClearColor(0,0,0,1);

		glGenVertexArrays(1,&vao);
		glBindVertexArray(vao);

		glGenBuffers(1,&vbo);
		glBindBuffer(GL_ARRAY_BUFFER,vbo);

		float vertices[] = { 
			-1,1,0,	  0,0,
			-1,-1,0,  0,1,
			1,-1,0,	  1,1,

			1,-1,0,	  1,1,
			1,1,0,	  1,0,
			-1,1,0,	  0,0
		};

		glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(GL_FLOAT),(GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(GL_FLOAT),(GLvoid*)(3* sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(1);

		modelMat = Matrix4x4::Identity();
		Matrix4x4 viewMat =Matrix4x4::Identity();
		Matrix4x4 projMat = Ortho(-2,2,-2,2,0,10);

		dShader = std::make_shared<Shader>("./sample.vert","./sample.frag");
		dShader->Use();
		dShader->SetMatrix("model",modelMat.Get());
		dShader->SetMatrix("view",viewMat.Get());
		dShader->SetMatrix("proj",projMat.Get());

		tShader = std::make_shared<Shader>("./image.vert","./image.frag");
		tShader->Use();
		modelMat.Scale(0.5);
		tShader->SetMatrix("model",modelMat.Get());
		tShader->SetMatrix("view",viewMat.Get());
		tShader->SetMatrix("proj",projMat.Get());

		//输入图像
		static const GLfloat cData[] = {
			1.0,0.0,0.0,1.0,	0.0,1.0,0.0,1.0,
			0.0,0.0,1.0,1.0,	1.0,1.0,0.0,1.0
		};
		glGenTextures(1,&tex);
		glBindTexture(GL_TEXTURE_2D,tex);
		glTexStorage2D(GL_TEXTURE_2D,1,GL_RGBA32F,2,2);
		glTexSubImage2D(GL_TEXTURE_2D,
				0,
				0,0,
				2,2,
				GL_RGBA,GL_FLOAT,
				cData);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

		//输出图像
		glGenTextures(1,&otex);
		glBindTexture(GL_TEXTURE_2D,otex);
		glTexStorage2D(GL_TEXTURE_2D,1,GL_RGBA32F,4,4);
		glBindTexture(GL_TEXTURE_2D,0);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		
		glBindTexture(GL_TEXTURE_2D,0);
	}

	glClear(GL_COLOR_BUFFER_BIT);

	dShader->Use();
	glBindImageTexture(0,tex,0,GL_FALSE,0,GL_READ_ONLY,GL_RGBA32F);
	glBindImageTexture(1,otex,0,GL_FALSE,0,GL_WRITE_ONLY,GL_RGBA32F);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES,0,6);
	//glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	tShader->Use();
	glBindTexture(GL_TEXTURE_2D,otex);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES,0,6);

	printf("%s\n",gluErrorString(glGetError()));

	glFlush();
}
