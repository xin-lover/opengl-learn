#include "model.h"
#include <stdio.h>

//#define STB_IMAGE_IMPLEMENTATION
//#include "../public/stb_image.h"

#include "../public/texture.h"


//unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

void Model::Draw(std::shared_ptr<Shader> shader)
{
	for(unsigned int i =0;i < meshes.size(); ++i)
	{
		meshes[i].Draw(shader);
	}
}

void Model::LoadModel(string path)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		printf("error:assimp:%s\n",import.GetErrorString());
		return;
	}

	directory = path.substr(0,path.find_last_of('/'));

	ProcessNode(scene->mRootNode,scene);
}

void Model::ProcessNode(aiNode *node, const aiScene *scene)
{
	//处理节点所有网格
	for(unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh,scene));
	}

	//接下来对它的节点重复这一过程
	for(unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i],scene);
	}
}

Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene)
{
	vector<Vertex> vertices;;
	vector<unsigned int> indices;
	vector<Texture> textures;

	//顶点，法线，纹理
	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		Vector3 v;
		v.x = mesh->mVertices[i].x;
		v.y = mesh->mVertices[i].y;
		v.z = mesh->mVertices[i].z;
		vertex.position = v;

		v.x = mesh->mNormals[i].x;
		v.y = mesh->mNormals[i].y;
		v.z = mesh->mNormals[i].z;
		vertex.normal = v;

		if(mesh->mTextureCoords[0])
		{
			Vector2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texcoords = vec;
		}
		else
		{
			vertex.texcoords = Vector2(0.0f,0.0f);
		}

		/*//tangent
		v.x = mesh->mTangents[i].x;
		v.y = mesh->mTangents[i].y;
		v.z = mesh->mTangents[i].z;
		vertex.tangent = v;

		//bitangent
		v.x = mesh->mBitangents[i].x;
		v.y = mesh->mBitangents[i].y;
		v.z = mesh->mBitangents[i].z;
		vertex.bitangent=v;*/

		vertices.push_back(vertex);

	}

	//索引
	for(unsigned int i=0;i < mesh->mNumFaces;i++)
	{
		aiFace face = mesh->mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices;j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
	vector<Texture> diffuseMaps = LoadMaterialTextures(material,aiTextureType_DIFFUSE,"texture_diffuse");
	textures.insert(textures.end(),diffuseMaps.begin(), diffuseMaps.end());
	vector<Texture> specularMaps = LoadMaterialTextures(material,aiTextureType_SPECULAR,"texture_specular");
	textures.insert(textures.end(),specularMaps.begin(),specularMaps.end());

	return Mesh(vertices, indices, textures);
}

vector<Texture> Model::LoadMaterialTextures(aiMaterial *mat,aiTextureType type, string typeName)
{
	vector<Texture> textures;
	for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type,i, &str);
		bool skip = false;
		for(unsigned int j = 0;j < textures_loaded.size(); j++)
		{
			if(std::strcmp(textures_loaded[j].path.data(),str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}

		if(!skip)
		{
			//如果纹理还没有被加载，则加载它
			Texture texture;
			string filename = string(str.C_Str());
			filename = directory + '/' + filename;
			texture.id = luwu::Texture2D::LoadTexture(filename.c_str());
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);//添加到已加载纹理中
		}

	}
	return textures;
}

/*unsigned int TextureFromFile(const char* path, const string &directory, bool gamma)
{
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1,&textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height,&nrComponents,0);
	if(data == NULL)
	{
		printf("load %s fail..\n",path);
		stbi_image_free(data);
		return 0;
	}

	GLenum format;
	if(nrComponents ==  1)
	{
		format = GL_RED;
	}
	else if(nrComponents == 3)
	{
		format = GL_RGB;
	}
	else if(nrComponents == 4)
	{
		format = GL_RGBA;
	}
	else
	{
		std::cout<<"error texture format"<<std::endl;
	}

	glBindTexture(GL_TEXTURE_2D,textureID);
	glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,format,GL_UNSIGNED_BYTE,data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	stbi_image_free(data);
	return textureID;
}*/
