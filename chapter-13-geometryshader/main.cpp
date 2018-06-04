#include <GL/glew.h>
#include <GL/freeglut.h>

#include <stdio.h>
#include <memory>

#include "../public/shader.h"
#include "../public/threemath.h"
#include "../public/texture.h"

using namespace luwu;

bool isInit = false;
void Display();
void Reshape(int width,int height);

GLuint vao;
GLuint vbo;

std::shared_ptr<Shader> dShader;
std::shared_ptr<Shader> layerShader;
std::shared_ptr<Shader> texShader;
std::shared_ptr<Shader> vShader;

std::shared_ptr<Texture2D> abTex;

GLuint fbo;
GLuint tex;


int main(int argc, char* argv[])
{
	glutInit(&argc,argv);
	glutInitContextVersion(4,1);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(640,480);
	glutCreateWindow("geometry");

	glewExperimental = GL_TRUE;
	GLint res = glewInit();
	if(res != GLEW_OK)
	{
		printf("glew init error\n");
	}
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
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
			-1,1,0, 0,0,
			-1,-1,0,0,1,
			1,-1,0,	1,1,

			1,-1,0, 1,1,
			1,1,0, 	1,0,
			-1,1,0, 0,0
		};

		glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(GL_FLOAT),(GLvoid*)(0));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(GL_FLOAT),(GLvoid*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(1);

		//着色器
		Matrix4x4 modelMat = Matrix4x4::Identity();
		Matrix4x4 viewMat = Matrix4x4::Identity();
		Matrix4x4 projMat = Ortho(-2,2,-2,2,0,10);

		dShader = std::make_shared<Shader>("./sample.vert","./sample.frag", "./sample.geom");
		dShader->Use();
		dShader->SetMatrix("model",modelMat.Get());
		dShader->SetMatrix("view",viewMat.Get());
		dShader->SetMatrix("proj",projMat.Get());

		layerShader = std::make_shared<Shader>("./layer.vert","./layer.frag","./layer.geom");
		layerShader->Use();
		layerShader->SetMatrix("proj",projMat.Get());
		layerShader->SetInt("output_slices",2);

		texShader = std::make_shared<Shader>("./tex.vert","./tex.frag");
		texShader->Use();
		texShader->SetMatrix("model",modelMat.Get());
		texShader->SetMatrix("view",viewMat.Get());
		texShader->SetMatrix("proj",projMat.Get());

		vShader = std::make_shared<Shader>("./mview.vert","./mview.frag","./mview.geom");
		vShader->Use();

		Matrix4x4 m[4];
		for(int i=0;i < 4; ++i)
		{
			m[i] = Matrix4x4::Identity();
			m[i].Translate(Vector3((i/2) * 0.25,(i%2) * 0.25,0));
		}

		float mdata[4][16];
		for(int i=0;i < 4; ++i)
		{
			for(int j =0; j < 16; ++j)
			{
				mdata[i][j] = (m[i].Get())[j];
			}
		}
		vShader->SetMatrix("model_matrix",4,&(mdata[0][0]));
		vShader->SetMatrix("proj",projMat.Get());
	printf("%s\n",gluErrorString(glGetError()));


		abTex = std::make_shared<Texture2D>("./left.jpg");

		//创建帧缓存
		glGenFramebuffers(1,&fbo);
		glBindFramebuffer(GL_FRAMEBUFFER,fbo);

		glGenTextures(1,&tex);
		glBindTexture(GL_TEXTURE_2D_ARRAY,tex);
		glTexImage3D(GL_TEXTURE_2D_ARRAY,
				0,
				GL_RGB,
				640,
				480,
				32,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				nullptr);
		/*glTexStorage3D(GL_TEXTURE_2D_ARRAY,
				1,
				GL_RGB8,
				1,1,
				100
			      );
		for( unsigned int i(0); i!=100;++i)
		{
			//Choose a random color for the i-essim image
			GLubyte color[3] = {rand()%255,rand()%255,rand()%255};

			//Specify i-essim image
			glTexSubImage3D( GL_TEXTURE_2D_ARRAY,
					0,                     //Mipmap number
					0,0,i,                 //xoffset, yoffset, zoffset
					1,1,1,                 //width, height, depth
					GL_RGB,                //format
					GL_UNSIGNED_BYTE,      //type
					color);                //pointer to data
		}*/
		glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

		/*glBindTexture(GL_TEXTURE_2D, tex);
		glTexImage2D(GL_TEXTURE_2D,
				0,
				GL_RGB,
				640,480,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				nullptr);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);*/

		glFramebufferTexture(GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0,
				tex,
				0);


		/*static const GLenum draw_buffers[] = {
			GL_COLOR_ATTACHMENT0
		};
		glDrawBuffers(1,draw_buffers);*/

		/*glFramebufferTexture2D(GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D,
				tex,
				0);*/

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(status != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("frambuffer status error.\n");
		}

		glBindTexture(GL_TEXTURE_2D_ARRAY,0);
	}


	glBindFramebuffer(GL_FRAMEBUFFER,fbo);
	//glViewport(0,0,640,480);
	glClear(GL_COLOR_BUFFER_BIT);
	layerShader->Use();
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES,0,6);

	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glViewport(0,0,640,480);
	glClear(GL_COLOR_BUFFER_BIT);
	texShader->Use();
	glBindTexture(GL_TEXTURE_2D_ARRAY,tex);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES,0,6);


	glFlush();
}

void Reshape(int width, int height)
{
	const float wot = float(width) * 0.5f;
	const float hot = float(height) * 0.5f;

	glViewportIndexedf(0,0.0f,0.0f,wot,hot);
	glViewportIndexedf(1,wot,0.0f,wot,hot);
	glViewportIndexedf(2,0.0f,hot,wot,hot);
	glViewportIndexedf(3,wot,hot,wot,hot);
}
