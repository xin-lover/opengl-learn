#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <string>
#include "../public/ShaderHelper.h"
#include "../public/threemath.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../public/stb_image.h"

using namespace std;

GLuint cubeVAO;
GLuint cubeBuf;
GLuint prog;
GLint mv; 
GLint pMat;
Matrix4x4 mat;
Matrix4x4 projMat;

Vector3 lastV;

GLuint skyboxVAO;
GLuint skyboxBuf;
GLuint skyboxProg;

GLuint skyboxTex;
GLuint cubeTex;

void PrintMat(float *vv)
{

	for(int i=0;i < 16; ++i)
	{
		printf("%f ",vv[i]);
		if((i + 1) % 4 == 0)
		{
			printf("\n");
		}
	}
}

bool LoadCubemap(GLuint tex,vector<std::string> faces)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP,tex);
	int width,height;
	glTexStorage2D(GL_TEXTURE_CUBE_MAP,2,GL_RGB8,2048,2048);
	for(int i=0; i < faces.size(); ++i)
	{

		int nchannels;
		unsigned char *data = stbi_load(faces[i].c_str(),&width,&height,&nchannels,0);
		if(!data)
		{
			printf("load texture %s fail.\n",faces[i].c_str());
		}
		printf("path:%s width:%d height:%d channel:%d\n",faces[i].c_str(),width,height, nchannels);

		

		GLenum target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;

		/*glenum err = glgeterror();
		  const glubyte* content = gluerrorstring(err);
		  printf("error:%s\n",content);*/
		glTexSubImage2D(target,
				0,
				0,0,
				width,height,
				GL_RGB,GL_UNSIGNED_BYTE,
				data);


		//glTexImage2D(target,0,GL_RGB,tex_width,tex_height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
		stbi_image_free(data);
	}
}

GLuint CreateCubemap()
{
	GLuint tex;
	glGenTextures(1,&tex);
	if(tex == 0)
	{
		printf("gen texture fail.\n");
		exit(1);
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP,tex);

	
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	vector<std::string> faces{
		"./skybox/right.jpg",
		"./skybox/left.jpg",
		"./skybox/top.jpg",
		"./skybox/bottom.jpg",
		"./skybox/front.jpg",
		"./skybox/back.jpg"
	};
	
	LoadCubemap(tex,faces);
	return tex;
}

GLuint LoadImage(const char* path)
{
	glGenTextures(1,&cubeTex);
	glBindTexture(GL_TEXTURE_2D,cubeTex);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	int width,height,nChannels;
	unsigned char *data = stbi_load(path,&width,&height,&nChannels,0);
	if(!data )
	{
		printf("load %s fail...\n",path);
	}
	printf("load %s width:%d height:%d channels:%d\n", path, width, height,nChannels);

	/*glTexStorage2D(GL_TEXTURE_2D,1,GL_RGB8,width,height);
	glTexSubImage2D(GL_TEXTURE_2D,
			0,
			0,0,
			width,height,
			GL_RGB,GL_UNSIGNED_BYTE,
			data);*/

	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
	stbi_image_free(data);
	return cubeTex;
}

int init()
{
	glClearColor(0.0,0.0,0.0,1.0);
	//glMatrixMode(GL_PROJECTION);
	//glOrtho(-5,5,-5,5,5,15);
	//glFrustum(-5,5,-5,5,0.3,15);
	//glMatrixMode(GL_MODELVIEW);
	//gluLookAt(0,0,10,0,2,0,0,1,0);

	//glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	glGenVertexArrays(1,&cubeVAO);
	glBindVertexArray(cubeVAO);

	glGenBuffers(1,&cubeBuf);
	glBindBuffer(GL_ARRAY_BUFFER,cubeBuf);

	Float vertices[] = 
	{
		//front
		-0.5, 0.5, 0.5,		0.0f,0.0f,
		-0.5,-0.5, 0.5,		0.0f,1.0f,
		0.5, -0.5, 0.5,		1.0f,1.0f,

		0.5, -0.5, 0.5,		1.0f,1.0f,
		0.5,  0.5, 0.5,		1.0f,0.0f,
		-0.5,  0.5, 0.5,	0.0f,0.0f,

		//back
		-0.5, 0.5, -0.5,	0.0f,0.0f,
		0.5,  0.5, -0.5,	1.0f,0.0f,
		0.5,  -0.5, -0.5, 	1.0f,1.0f,

		0.5,  -0.5, -0.5,	1.0f,1.0f,
		-0.5, -0.5, -0.5,	0.0f,1.0f,
		-0.5,  0.5, -0.5,	0.0f,0.0f,

		//left	
		-0.5,  0.5, -0.5, 	0.0f,0.0f,	
		-0.5,  -0.5,-0.5,	1.0f,0.0f,
		-0.5,  -0.5, 0.5,	1.0f,1.0f,

		-0.5,  -0.5, 0.5,	1.0f,1.0f,
		-0.5,  0.5,  0.5,	0.0f,1.0f,
		-0.5,  0.5, -0.5,	0.0f,0.0f,

		//right
		0.5,  0.5, -0.5,	0.0f,0.0f,
		0.5,  0.5,  0.5,	1.0f,0.0f,
		0.5, -0.5,  0.5,	1.0f,1.0f,

		0.5, -0.5,  0.5,	1.0f,1.0f,
		0.5, -0.5, -0.5,	0.0f,1.0f,
		0.5,  0.5, -0.5,	0.0f,0.0f,

		//up	
		 0.5, 0.5, -0.5,	0.0f,0.0f,
		-0.5, 0.5, -0.5,	0.0f,1.0f,
		-0.5, 0.5,  0.5,	1.0f,1.0f,

		-0.5, 0.5,  0.5,	1.0f,1.0f,
		 0.5, 0.5,  0.5,	1.0f,0.0f,
		 0.5, 0.5, -0.5,	0.0f,0.0f,

		//bottom
		0.5,  -0.5, -0.5,	0.0f,0.0f,
		0.5,  -0.5,  0.5,	0.0f,1.0f,
		-0.5, -0.5,  0.5,	1.0f,1.0f,

		-0.5, -0.5,  0.5,	1.0f,1.0f,
		-0.5, -0.5, -0.5,	1.0f,0.0f,
		 0.5, -0.5, -0.5,	0.0f,0.0f,
	};
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(GL_FLOAT),BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(GL_FLOAT),BUFFER_OFFSET((3 * sizeof(GL_FLOAT))));
	glEnableVertexAttribArray(1);

	Float skyboxVertices[] = 
	{
		//front
		-1, 1, 1,		
		-1,-1, 1,	
		1, -1, 1,

		1, -1, 1,
		1,  1, 1,
		-1,  1, 1,

		//back
		-1, 1, -1,
		1,  1, -1,
		1,  -1, -1,

		1,  -1, -1,
		-1, -1, -1,
		-1,  1, -1,

		//left	
		-1,  1, -1,
		-1,  -1,-1,
		-1,  -1, 1,

		-1,  -1, 1,
		-1,  1,  1,
		-1,  1, -1,

		//right
		1,  1, -1,
		1,  1,  1,
		1, -1,  1,

		1, -1,  1,
		1, -1, -1,
		1,  1, -1,

		//up	
		 1, 1, -1,
		-1, 1, -1,
		-1, 1,  1,

		-1, 1,  1,
		 1, 1,  1,
		 1, 1, -1,

		//bottom
		1,  -1, -1,
		1,  -1,  1,
		-1, -1,  1,

		-1, -1,  1,
		-1, -1, -1,
		 1, -1, -1,
	};
	


	glGenVertexArrays(1,&skyboxVAO);
	glBindVertexArray(skyboxVAO);

	glGenBuffers(1,&skyboxBuf);
	glBindBuffer(GL_ARRAY_BUFFER,skyboxBuf);
	glBufferData(GL_ARRAY_BUFFER,sizeof(skyboxVertices),skyboxVertices,GL_STATIC_DRAW);

	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(GL_FLOAT),BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	
	//projMat = Frustum(-0.1f,0.1f,-0.1,0.1,0.3f,10);
	projMat = Ortho(-1,1,-1,1,1,50);
	mat=Matrix4x4::Identity();
	mat.Translate(Vector3(0,0,-5));

	//load shader
	ShaderInfo shaders[] = {
		{GL_VERTEX_SHADER,"cube.vert"},
		{GL_FRAGMENT_SHADER,"cube.frag"},
		{GL_NONE,""}
	};

	prog = LoadShaders(shaders);
	printf("cube prog:%d\n",prog);
	glUseProgram(prog);

	//cube
	mv = glGetUniformLocation(prog,"ViewMatrix");
       	if(mv == -1)
	{
		printf("not find uniform ViewMatrix\n");
	}
	glUniformMatrix4fv(mv,1,GL_FALSE,mat.Get());	

	pMat = glGetUniformLocation(prog, "ProjectionMatrix");
	if(pMat == -1)
	{
		printf("not find ProjectionMatrix\n");
	}
	glUniformMatrix4fv(pMat,1,GL_FALSE,projMat.Get());

	LoadImage("container.jpg");
	printf("cubeTex:%d\n",cubeTex);
	GLint texLocation = glGetUniformLocation(prog,"tex1");
	if(texLocation == -1)
	{
		printf("Can't find tex in program\n");
	}
	else
	{
		glUniform1i(texLocation,0);
	}
	
	ShaderInfo skyboxShaders[] = {
		{GL_VERTEX_SHADER,"skybox.vert"},
		{GL_FRAGMENT_SHADER,"skybox.frag"},
		{GL_NONE,""}
	};

	
	//skybox
	skyboxProg = LoadShaders(skyboxShaders);
	glUseProgram(skyboxProg);
	GLint skMatLoc = glGetUniformLocation(skyboxProg,"ViewMatrix");
	if(skMatLoc == -1)
	{
		printf("skybox program can't find ViewMatrix.\n");
	}
	glUniformMatrix4fv(skMatLoc,1,GL_FALSE,mat.Get());

	GLint skProjMat = glGetUniformLocation(skyboxProg, "ProjectionMatrix");
	if(skProjMat == -1)
	{
		printf("not find ProjectionMatrix\n");
	}
	glUniformMatrix4fv(skProjMat,1,GL_FALSE,projMat.Get());

	skyboxTex = CreateCubemap();
	GLint mapLoc = glGetUniformLocation(skyboxProg,"tex");
	glUniform1i(mapLoc,0);

	return 0;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	printf("%d\n",glIsEnabled(GL_DEPTH_TEST));
	glBindVertexArray(cubeVAO);
	glUseProgram(prog);
	glUniformMatrix4fv(mv,1,GL_FALSE,mat.Get());
	glBindTexture(GL_TEXTURE_2D,cubeTex);
	glDrawArrays(GL_TRIANGLES,0,36);
	glBindVertexArray(0);
	

	glDepthFunc(GL_LEQUAL);
	glBindVertexArray(skyboxVAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP,skyboxTex);

	glUseProgram(skyboxProg);
	glDrawArrays(GL_TRIANGLES,0,36);

	glDepthFunc(GL_LESS);
	glFlush();
}

void Idle()
{
	//mat.Rotate(Vector3(0,1,0),0.001);
	//glutPostRedisplay();
}

void MouseHandle(int x, int y)
{
	return;
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
	mat.Translate(Vector3(0,0,4));
	mat *= m;
	mat.Translate(Vector3(0,0,-4));
	lastV = curV;
	glutPostRedisplay();
}

void MouseUpHandle(int button, int state, int x, int y)
{
	/*if(button == GLUT_LEFT_BUTTON)
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
	};*/
}

void KeyPressHandle(unsigned char key, int x, int y)
{
	if(key == 'p')
	{
		mat.Rotate(Vector3(0,1,0),1);
		glutPostRedisplay();
	}

	if(key == 'o')
	{
		mat.Rotate(Vector3(0,1,0),-1);
		glutPostRedisplay();
	}
	
	if(key == 'k')
	{
		mat.Rotate(Vector3(1,0,0),1);
		glutPostRedisplay();
	}

	if(key == 'l')
	{
		mat.Rotate(Vector3(1,0,0),-1);
		glutPostRedisplay();
	}

	if(key == 'a')
	{
		mat.Translate(Vector3(0.01,0,0));
		glutPostRedisplay();
	}

	if(key == 'd')
	{
		mat.Translate(Vector3(-0.01,0,0));
		glutPostRedisplay();
	}

	if(key == 'w')
	{
		mat.Translate(Vector3(0,0,-0.01));
		glutPostRedisplay();
	}

	if(key == 's')
	{
		mat.Translate(Vector3(0,0,0.01));
		glutPostRedisplay();
	}

	
}

int main(int argc, char* argv[])
{
	glutInit(&argc,argv);
	glutInitContextVersion(4,3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(600,480);
	glutCreateWindow("Cube");

	glewExperimental = GL_TRUE;
	GLint res = glewInit();
	if(res != GLEW_OK)
	{
		printf("glew init fail..\n");
		exit(1);
	}


	if(init() != 0)
	{
		printf("init fail....\n");
		exit(1);
	}

	glutDisplayFunc(display);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyPressHandle);
	glutMouseFunc(MouseUpHandle);
	glutMotionFunc(MouseHandle);
	glutMainLoop();
	
	glDeleteVertexArrays(1,&cubeVAO);
	glDeleteBuffers(1,&cubeBuf);
	glDeleteVertexArrays(1,&skyboxVAO);
	glDeleteBuffers(1,&skyboxBuf);
	return 0;
}


