#ifndef __MESH_HEADER__
#define __MESH_HEADER__

#include "../public/threemath.h"
#include "../public/shader.h"
#include <string>
#include <vector>
#include <memory>

using namespace std;

namespace luwu
{
	struct Vertex
	{
		Vector3 position;
		Vector3 normal;
		Vector2 texcoords;
	};

	struct Texture
	{
		unsigned int id;
		string type;
		string path;
	};

	class Mesh
	{
		public:
			/*网格数据*/
			vector<Vertex> vertices;
			vector<unsigned int> indices;
			vector<Texture> textures;

			Mesh(vector<Vertex> vertices,vector<unsigned int> indices, vector<Texture> textures);
			void Draw(std::shared_ptr<Shader> shader);
		private:
			/*渲染数据*/
			unsigned int VAO,VBO,EBO;
			void SetupMesh();
	};
}


#endif
