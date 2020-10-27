#include <vector>
#include <string>
#include <memory>
#include "shader.h"
#include "mesh.h"
#include <glad/glad.h>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <stdio.h>

using namespace luwu;


class Model
{
	public:
		vector<Texture> textures_loaded;
		vector<Mesh> meshes;
		string directory;

		Model(const string &path)
		{
			LoadModel(path);
		}

		Model(const Model &other) = delete;
		void operator=(const Model &other) = delete;

		~Model()
		{

		}

		void Draw(std::shared_ptr<Shader> shader);

	private:
		void LoadModel(string path);
		void ProcessNode(aiNode *node, const aiScene *scene);
		Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
		vector<Texture> LoadMaterialTextures(aiMaterial *mat,aiTextureType type,string typeName);
};
