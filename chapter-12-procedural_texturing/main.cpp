#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <memory>
#include <math.h>
#include <vector>

#include "../public/shader.h"
#include "../public/threemath.h"
#include "../public/perlinnoise.h"

bool isInit = false;
void Display();

GLuint vao;
GLuint vbo;
Matrix4x4 modelMat;

GLuint sphereVao;
GLuint sphereVbo;
GLuint sphereEbo;

std::shared_ptr<Shader> dShader;
std::shared_ptr<Shader> brickShader;
std::shared_ptr<Shader> latticeShader;
std::shared_ptr<Shader> cloudShader;

std::shared_ptr<Shader> sampleShader;

GLuint noise3dTex;

GLuint CreateNoise3dTexture(int size);

int main(int argc, char *argv[])
{
	glutInit(&argc,argv);
	glutInitContextVersion(3,3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(512,512);
	glutCreateWindow("procedural_texturing");

	glewExperimental = GL_TRUE;
	GLint res = glewInit();
	if(res != GLEW_OK)
	{
		printf("glew init failed.");
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

		//glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);

		glGenVertexArrays(1,&vao);
		glBindVertexArray(vao);

		glGenBuffers(1,&vbo);
		glBindBuffer(GL_ARRAY_BUFFER,vbo);

		float vertices[] = {
			-1,-1,0, 0,0, 0,0,1,
			1,-1,0,	 1,0, 0,0,1,
			1,1,0,	 1,1, 0,0,1,

			1,1,0,	 1,1, 0,0,1,
			-1,1,0,	 0,1, 0,0,1,
			-1,-1,0, 0,0, 0,0,1,
		};

		glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8 * sizeof(GL_FLOAT),(GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,8 * sizeof(GL_FLOAT),(GLvoid*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,8 * sizeof(GL_FLOAT),(GLvoid*)(5 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(2);

		glGenVertexArrays(1,&sphereVao);
		glBindVertexArray(sphereVao);

		glGenBuffers(1,&sphereVbo);
		glBindBuffer(GL_ARRAY_BUFFER,sphereVbo);


		const float pi = 3.141592653;
		float r = 1;
		int s = 0;
		std::vector<Vector3> svertices;
		std::vector<int> indices;
		
		int count = 50 ;
		float theta = 2 * pi / count;
		Matrix4x4 urot = Matrix4x4::Identity();
		urot.Rotate(Vector3(0,1,0),360.0 / count);
		int seq = 0;
		printf("%f\n",cos(pi));
		for(int i =0;i < 50; ++i)
		{
			Matrix4x4 rot = Matrix4x4::Identity();
			rot.Rotate(Vector3(0,1,0),i * 360.0 / count);
			for(int j=0; j < count; ++j)
			{
				Vector3 v(0, r * sin(pi * 0.5 - theta * j), r * cos(pi * 0.5 - theta * j));
				v = rot * v;
				printf("%f %f %f \n", v.x,v.y,v.z);

				svertices.push_back(v);
				indices.push_back(seq++);

				if(j != 0 && j != count / 2)
				{
					v = urot * v;
					svertices.push_back(v);
					indices.push_back(seq++);
				}

			}

			//svertices[i * 3 + 0] = r * cos(theta * i);
			//svertices[i * 3 + 1] = 0;
			//svertices[i * 3 + 2] = r * sin(theta * i);
		}

		/*float svertices[] = 
		{
			-1,-1,0,
			0,1,0,
			1,-1,0
		};*/

		glBufferData(GL_ARRAY_BUFFER,svertices.size() * sizeof(Vector3),&svertices[0],GL_STATIC_DRAW);


		glGenBuffers(1,&sphereEbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sphereEbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(int),&indices[0],GL_STATIC_DRAW);


		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(GL_FLOAT), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		modelMat = Matrix4x4::Identity();
		modelMat.Translate(Vector3(0,-0,-1.5));
		Matrix4x4 viewMat= Matrix4x4::Identity();
		//viewMat.Rotate(Vector3(0,1,0),-30);
		//viewMat.Translate(Vector3(0,-0.5,0));
		Matrix4x4 projMat= Ortho(-2,2,-2,2,0,10);
		//Matrix4x4 projMat= Frustum(-2,2,-2,2,0.5,10);

		dShader = std::make_shared<Shader>("./stripe.vert","./stripe.frag");
		dShader->Use();
		dShader->SetMatrix("model",modelMat.Get());
		dShader->SetMatrix("view",viewMat.Get());
		dShader->SetMatrix("proj",projMat.Get());
		dShader->SetFloat("fuzz",0.1);
		dShader->SetFloat("width",0.5);
		dShader->SetFloat("scale",10.0);

		dShader->SetVec3("stripeColor",1,0,0);
		dShader->SetVec3("backColor",1,1,1);

		brickShader = std::make_shared<Shader>("./brick.vert","./brick.frag");
		brickShader->Use();
		brickShader->SetMatrix("model",modelMat.Get());
		brickShader->SetMatrix("view",viewMat.Get());
		brickShader->SetMatrix("proj",projMat.Get());
		brickShader->SetVec3("brickColor",1,0,0);
		brickShader->SetVec3("mortarColor",0,1,0);
		brickShader->SetVec2("brickSize",0.2,0.1);
		brickShader->SetVec2("brickPct",0.9,0.8);
		
		latticeShader = std::make_shared<Shader>("./lattice.vert","./lattice.frag");
		latticeShader->Use();
		latticeShader->SetMatrix("model",modelMat.Get());
		latticeShader->SetMatrix("view",viewMat.Get());
		latticeShader->SetMatrix("proj",projMat.Get());
		latticeShader->SetVec2("scale",5,5);
		latticeShader->SetVec2("threshold",0.13,0.13);

		Matrix4x4 mvp = projMat * viewMat * modelMat;
		Matrix4x4 mv = viewMat * modelMat;
		cloudShader = std::make_shared<Shader>("./cloud.vert","./cloud.frag");
		
		cloudShader->Use();
		cloudShader->SetMatrix("model",modelMat.Get());
		cloudShader->SetMatrix("view",viewMat.Get());
		cloudShader->SetMatrix("proj",projMat.Get());
		cloudShader->SetMatrix("normalMat",Matrix4x4::Identity().Get());
		cloudShader->SetVec3("lightPos",0,0,1);
		cloudShader->SetFloat("scale",1.5);
		cloudShader->SetVec3("skyColor",0,0,0.8);
		cloudShader->SetVec3("cloudColor",0.8,0.8,0.8);

		noise3dTex = CreateNoise3dTexture(128);

		sampleShader = std::make_shared<Shader>("./sample.vert","./sample.frag");
		sampleShader->Use();
		sampleShader->SetMatrix("model",modelMat.Get());
		sampleShader->SetMatrix("view",viewMat.Get());
		sampleShader->SetMatrix("proj",projMat.Get());
	}

	glClear(GL_COLOR_BUFFER_BIT);

	Matrix4x4 tmat = modelMat;
	tmat.Scale(0.3);
	tmat.Translate(Vector3(-0.5,0.5,0));

	/*dShader->Use();
	dShader->SetMatrix("model",tmat.Get());
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES,0,6);*/

	tmat.Translate(Vector3(-1,0,0));
	latticeShader->Use();
	latticeShader->SetMatrix("model",tmat.Get());
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES,0,6);

	tmat.Translate(Vector3(1,0,0));
	tmat.Scale(1.8);
	brickShader->Use();
	brickShader->SetMatrix("model",tmat.Get());
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES,0,6);

	tmat.Translate(Vector3(0,-1,0));
	cloudShader->Use();
	cloudShader->SetMatrix("model",tmat.Get());
	glBindTexture(GL_TEXTURE_3D,noise3dTex);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES,0,6);

	/*sampleShader->Use();
	glBindVertexArray(sphereVao);
	glDrawElements(GL_TRIANGLE_STRIP,98 * 50,GL_UNSIGNED_INT,0);*/

	glFlush();
}

GLuint CreateNoise3dTexture(int size)
{
	int f,i,j,k,inc;
	int startFrequency = 4;
	int numOctaves = 4;
	float ni[3];
	float inci,incj,inck;
	int frequency = startFrequency;
	GLubyte *ptr;
	float amp = 0.5;
	GLubyte *tmp;

	if((ptr = (GLubyte *)malloc(size * size * size * 4)) == NULL)
	{
		fprintf(stderr,"Error:out of memory\n");
		exit(1);
	}

	for(f = 0,inc = 0;f< 1;++f,frequency *=2,++inc,amp*=0.5)
	{
		//SetNoiseFrequency(frequency);
		ni[0] = ni[1] = ni[2] = 0;
		tmp = ptr;

		inci = 1.0/(size / frequency);
		for(i=0;i<size;++i,ni[0]+=inci)
		{
			incj = 1.0 / (size / frequency);
			for(j=0;j<size; ++j,ni[1]+=incj)
			{
				inck = 1.0/(size / frequency);
				for(k = 0; k<size;++k,ni[2]+=inck,tmp+=4)
				{
					//printf("%d\n",(GLubyte)(((noise3(ni) + 1.0) + amp) * 128.0));
					*(tmp+inc) = (GLubyte)(((noise3(ni) + 1.0) + amp) * 128.0);
				}
			}
		}
	}

	GLuint tex;
	glGenTextures(1,&tex);
	//glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_3D,tex);
	glTexParameterf(GL_TEXTURE_3D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_3D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_3D,GL_TEXTURE_WRAP_R,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_3D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_3D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	glTexImage3D(GL_TEXTURE_3D,0,GL_RGBA,size,size,size,0,GL_RGBA,GL_UNSIGNED_BYTE,ptr);

	free(ptr);

	return tex;
}
