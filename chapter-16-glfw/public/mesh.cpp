#include "mesh.h"
#include <stddef.h>

using namespace luwu;

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	SetupMesh();
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1,&VBO);
	glGenBuffers(1,&EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);

	glBufferData(GL_ARRAY_BUFFER,this->vertices.size() * sizeof(Vertex), &vertices[0],GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(unsigned int), &indices[0],GL_STATIC_DRAW);

	//顶点位置
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	//顶点法线
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex,normal)));
	//顶点纹理坐标
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,2,GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex,texcoords)));

	glBindVertexArray(0);
}

void Mesh::Draw(std::shared_ptr<Shader> shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;

	for(unsigned int i = 0; i < textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i); //在绑定之前激活相应纹理单元
		//获取纹理号
		string number;
		string name = textures[i].type;
		if(name == "texture_diffuse")
		{
			number = std::to_string(diffuseNr++);
		}
		else if(name == "texture_specular")
		{
			number = std::to_string(specularNr++);
		}

		shader->SetInt((name + number).c_str(),i);
		glBindTexture(GL_TEXTURE_2D,textures[i].id);
	}


	//绘制网格
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

