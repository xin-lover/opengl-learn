#include <GL/glew.h>
#include <GL/freeglut.h>

#include "../public/shader.h"
#include "../public/threemath.h"

#include <stdio.h>
#include <memory>

bool isInit = false;
void Display();


GLuint vao;
GLuint vbo;
GLuint cvbo;

std::shared_ptr<Shader> dShader;

int main(int argc, char* argv[])
{
	glutInit(&argc,argv);
	glutInitContextVersion(3,3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640,480);
	glutCreateWindow("instanced rendering");

	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
	{
		printf("glew init error.\n");
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

		glEnable(GL_DEPTH_TEST);
		glGenVertexArrays(1,&vao);
		glBindVertexArray(vao);

		glGenBuffers(1,&vbo);
		glBindBuffer(GL_ARRAY_BUFFER,vbo);
		
		float vertices[] = 
		{
			-1,1,0,
			-1,-1,0,
			1,-1,0,

			1,-1,0,
			1,1,0,
			-1,1,0
		};

		glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(GL_FLOAT),(GLvoid*)0);
		glEnableVertexAttribArray(0);

		float colors[]=
		{
			0.4,0,0,
			1.0,0,0,
			0,0.5,0,
			0,1,0,
			0,0,0.5,
			0,0,1,
			0.5,0.5,0,
			1,1,0,
			0,0.5,0.5,
			0,1,1
		};

		glGenBuffers(1,&cvbo);
		glBindBuffer(GL_ARRAY_BUFFER,cvbo);
		glBufferData(GL_ARRAY_BUFFER,sizeof(colors),colors,GL_STATIC_DRAW);

		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3 * sizeof(GL_FLOAT),(GLvoid*)0);
		glEnableVertexAttribArray(1);

		glVertexAttribDivisor(1,1);


		Matrix4x4 modelMat = Matrix4x4::Identity();
		modelMat.Translate(Vector3(0,0,-2));
		Matrix4x4 viewMat = Matrix4x4::Identity();
		//Matrix4x4 projMat = Ortho(-2,2,-2,2,0,10);
		Matrix4x4 projMat = Frustum(-2,2,-2,2,0.5,10);
		
		dShader = std::make_shared<Shader>("./sample.vert","./sample.frag");
		dShader->Use();

		dShader->SetMatrix("model",modelMat.Get());
		dShader->SetMatrix("view",viewMat.Get());
		dShader->SetMatrix("proj",projMat.Get());
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao);
	dShader->Use();
	glDrawArraysInstanced(GL_TRIANGLES,0,6,10);

	glFlush();
}
