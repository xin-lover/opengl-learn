#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "../public/ShaderHelper.h"
#include "../public/threemath.h"
#include "../public/program_util.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../public/stb_image.h"

GLuint vao1;
GLuint vbo1;

GLuint prog;

Vector3 lastV;
Matrix4x4 modelMat;
GLint modelLoc;

void Display();
void MouseHandle(int x,int y);
void MouseUpHandle(int button, int state, int x, int y);
GLuint LoadImage(const char* path);


int main(int argc, char *argv[])
{
	glutInit(&argc,argv);
	glutInitContextVersion(3,3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitWindowSize(600,480);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
	glutCreateWindow("Depth");
	glutInitWindowSize(0,0);

	glewExperimental = GL_TRUE;
	GLint res = glewInit();
	if(res != GLEW_OK)
	{
		printf("init glew fail.\n");
		exit(1);
	}

	//depth and stencil apply
	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);
	glClearStencil(0x0);

	Float vertices[] = 
	{
		//front
		-0.5, 0.5, 0.5,		0.0f,0.0f,1.0, 	0, 0,
		-0.5,-0.5, 0.5,		0.0f,0.0f,1.0,	0, 1,
		0.5, -0.5, 0.5,		0.0f,0.0f,1.0,	1, 1,

		0.5, -0.5, 0.5,		0.0f,0.0f,1.0,	1, 1,
		0.5,  0.5, 0.5,		0.0f,0.0f,1.0,	1, 0,
		-0.5,  0.5, 0.5,	0.0f,0.0f,1.0,	0, 0,

		//back
		-0.5, 0.5, -0.5,	0.0f,0.0f,-1.0,	1, 0,
		0.5,  0.5, -0.5,	0.0f,0.0f,-1.0,	0, 0,
		0.5,  -0.5, -0.5, 	0.0f,0.0f,-1.0, 0, 1,

		0.5,  -0.5, -0.5,	0.0f,0.0f,-1.0,	0, 1,
		-0.5, -0.5, -0.5,	0.0f,0.0f,-1.0, 1, 1,
		-0.5,  0.5, -0.5,	0.0f,0.0f,-1.0, 1, 0,

		//left	
		-0.5,  0.5, -0.5, 	-1.0f,0.0f,0.0,		0,0,
		-0.5,  -0.5,-0.5,	-1.0f,0.0f,0.0,		0,1,
		-0.5,  -0.5, 0.5,	-1.0f,0.0f,0.0,		1,1,

		-0.5,  -0.5, 0.5,	-1.0f,0.0f,0.0,		1,1,
		-0.5,  0.5,  0.5,	-1.0f,0.0f,0.0,		1,0,
		-0.5,  0.5, -0.5,	-1.0f,0.0f,0.0,		0,0,

		//right
		0.5,  0.5, -0.5,	1.0f,0.0f,0.0,		1,0,
		0.5,  0.5,  0.5,	1.0f,0.0f,0.0,		0,0,
		0.5, -0.5,  0.5,	1.0f,0.0f,0.0,		0,1,

		0.5, -0.5,  0.5,	1.0f,0.0f,0.0,		0,1,
		0.5, -0.5, -0.5,	1.0f,0.0f,0.0,		1,1,
		0.5,  0.5, -0.5,	1.0f,0.0f,0.0,		1,0,

		//up	
		 0.5, 0.5, -0.5,	0.0f,1.0f,0.0,		0,0,
		-0.5, 0.5, -0.5,	0.0f,1.0f,0.0,		0,1,
		-0.5, 0.5,  0.5,	0.0f,1.0f,0.0,		1,1,

		-0.5, 0.5,  0.5,	0.0f,1.0f,0.0,		1,1,
		 0.5, 0.5,  0.5,	0.0f,1.0f,0.0,		1,0,
		 0.5, 0.5, -0.5,	0.0f,1.0f,0.0,		0,0,

		//bottom
		0.5,  -0.5, -0.5,	0.0f,-1.0f,0.0,		1,0,
		0.5,  -0.5,  0.5,	0.0f,-1.0f,0.0,		0,0,
		-0.5, -0.5,  0.5,	0.0f,-1.0f,0.0,		0,1,

		-0.5, -0.5,  0.5,	0.0f,-1.0f,0.0,		0,1,
		-0.5, -0.5, -0.5,	0.0f,-1.0f,0.0,		1,1,
		 0.5, -0.5, -0.5,	0.0f,-1.0f,0.0,		1,0,
	};

	glGenVertexArrays(1,&vao1);
	glBindVertexArray(vao1);

	glGenBuffers(1,&vbo1);
	glBindBuffer(GL_ARRAY_BUFFER,vbo1);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(GL_FLOAT),(GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(GL_FLOAT),(GLvoid*)(3*sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8 * sizeof(GL_FLOAT),(GLvoid*)(6 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);

	//shader
	ShaderInfo si[]={
		{GL_VERTEX_SHADER,"multiple_light.vert"},
		{GL_FRAGMENT_SHADER,"multiple_light.frag"},
		{GL_NONE,""}
	};
	prog = LoadShaders(si);
	glUseProgram(prog);
	

	//图片加载
	GLuint diffuseTex = LoadImage("diffuse.png");
	GLuint specularTex = LoadImage("specular.png");

	modelMat = Matrix4x4::Identity();
	//modelMat.Rotate(Vector3(1,0,0),30);
	modelMat.Rotate(Vector3(0,1,0),30);
	modelLoc= glGetUniformLocation(prog,"model");
	if(modelLoc == -1)
	{
		printf("can't find mvp in shader.\n");
	}
	glUniformMatrix4fv(modelLoc,1,GL_FALSE,modelMat.Get());

	Matrix4x4 viewMat = Matrix4x4::Identity();
	viewMat.Translate(Vector3(0,0,-2));
	GLint viewLoc = glGetUniformLocation(prog,"view");
	glUniformMatrix4fv(viewLoc,1,GL_FALSE,viewMat.Get());
	
	Matrix4x4 projMat = Ortho(-2,2,-2,2,0,10);
	GLint projLoc = glGetUniformLocation(prog,"proj");
	glUniformMatrix4fv(projLoc,1,GL_FALSE,projMat.Get());

	SetProgramVec3(prog,"material.ambient",1.0,1.0,1.0);
	SetProgramInt(prog,"material.diffuse",0);
	SetProgramInt(prog,"material.specular",1);
	SetProgramFloat(prog,"material.shininess",20);

	//SetProgramVec3(prog,"dirLight.lightColor",1.0,1.0,1.0);
	SetProgramVec3(prog,"dirLight.direction",1.0,0,0);
	SetProgramVec3(prog,"dirLight.ambient",1.0,1.0,1.0);
	SetProgramVec3(prog,"dirLight.diffuse",1.0,1.0,1.0);
	SetProgramVec3(prog,"dirLight.specular",1.0,1.0,1.0);


	SetProgramVec3(prog,"pointLights[0].position",-1,0.3f,5);
	SetProgramFloat(prog,"pointLights[0].constant",1);
	SetProgramFloat(prog,"pointLights[0].linear",0.09f);
	SetProgramFloat(prog,"pointLights[0].quadratic",0.032f);
	SetProgramVec3(prog,"pointLights[0].ambient",1,1,1);
	SetProgramVec3(prog,"pointLights[0].diffuse",1,0,0);
	SetProgramVec3(prog,"pointLights[0].specular",1,0,0);

	SetProgramVec3(prog,"pointLights[1].position",1,0.3f,5);
	SetProgramFloat(prog,"pointLights[1].constant",1);
	SetProgramFloat(prog,"pointLights[1].linear",0.09f);
	SetProgramFloat(prog,"pointLights[1].quadratic",0.032f);
	SetProgramVec3(prog,"pointLights[1].ambient",0,1,0);
	SetProgramVec3(prog,"pointLights[1].diffuse",0,1,0);
	SetProgramVec3(prog,"pointLights[1].specular",0,1,0);

	SetProgramVec3(prog,"pointLights[2].position",-1,0.3f,-2);
	SetProgramFloat(prog,"pointLights[2].constant",1);
	SetProgramFloat(prog,"pointLights[2].linear",0.09f);
	SetProgramFloat(prog,"pointLights[2].quadratic",0.032f);
	SetProgramVec3(prog,"pointLights[2].ambient",0,0,1);
	SetProgramVec3(prog,"pointLights[2].diffuse",0,0,1);
	SetProgramVec3(prog,"pointLights[2].specular",0,0,1);

	SetProgramVec3(prog,"pointLights[3].position",1,0.3f,-2);
	SetProgramFloat(prog,"pointLights[3].constant",1);
	SetProgramFloat(prog,"pointLights[3].linear",0.09f);
	SetProgramFloat(prog,"pointLights[3].quadratic",0.032f);
	SetProgramVec3(prog,"pointLights[3].ambient",1,0,1);
	SetProgramVec3(prog,"pointLights[3].diffuse",1,0,1);
	SetProgramVec3(prog,"pointLights[3].specular",1,0,1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,diffuseTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,specularTex);

	glutDisplayFunc(Display);
	glutMotionFunc(MouseHandle);
	glutMouseFunc(MouseUpHandle);
	glutMainLoop();

	return 0;
}
	
void Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	glBindVertexArray(vao1);
	glUniformMatrix4fv(modelLoc,1,GL_FALSE,modelMat.Get());
	glDrawArrays(GL_TRIANGLES,0,36);

	glBindVertexArray(0);
	glFlush();
}

void MouseHandle(int x, int y)
{
	Float fx = (x/640.0f - 0.5) * 2;
	Float fy = (y/480.0f - 0.5) * 2 * -1;
	Float x2addy2=fx * fx + fy * fy;
	Float z = 0;
	if(x2addy2 < 1)
	{
		z = sqrt(1 - fx * fx - fy * fy);
	}
	
	//printf("x:%f y:%f z:%f\n", fx, fy, z);

	if(lastV.x == 0 && lastV.y == 0 && lastV.z == 0)
	{
		lastV = Vector3(fx,fy,z);
		return;
	}

	Vector3 curV(fx,fy,z);
	Vector3 axis = Vector3::Cross(lastV,curV);
	axis.Normalized();

	Float c = Vector3::Dot(lastV, curV);
	if(c > 1)
	{
		c = 1;
	}

	if(c < -1)
	{
		c = -1;
	}

	Float theta = acos(c) * 180;

	//printf("theta:%f\n",theta);
	Matrix4x4 m = Matrix4x4::Identity();
	m.Rotate(axis,theta);
	modelMat *= m;
	lastV = curV;
	glutPostRedisplay();
}

void MouseUpHandle(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		if(state == GLUT_UP)
		{
			lastV = Vector3(0,0,0);
		}

		if(state == GLUT_DOWN)
		{

			Float fx = (x/640.0f - 0.5) * 2;
			Float fy = (y/480.0f - 0.5) * 2 * -1;
			Float z = sqrt(1 - fx * fx - fy * fy);

			lastV = Vector3(fx,fy,z);
		}
	}
}

GLuint LoadImage(const char* path)
{
	int width,height,nChannels;
	unsigned char* data = stbi_load(path,&width,&height,&nChannels,0);
	if(data == NULL)
	{
		printf("load %s fail.\n",path);
		return 0;
	}

	printf("load image:width:%d height %d channel:%d\n",width,height,nChannels);
	GLuint tex;
	glGenTextures(1,&tex);
	glBindTexture(GL_TEXTURE_2D,tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexStorage2D(GL_TEXTURE_2D, 1,GL_RGBA8,width,height);
	glTexSubImage2D(GL_TEXTURE_2D,
			0,
			0,0,
			width,height,
			GL_RGBA,GL_UNSIGNED_BYTE,
			data);

	return tex;
}
