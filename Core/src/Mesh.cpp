#include "Mesh.h"
#include "OBJ_Loader.h"


namespace Core
{
	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) :
		IndexCount(indices.size())
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexCount * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}

	Mesh::Mesh(const MeshData& meshData):
		Mesh(meshData.Vertices, meshData.Indices)
	{}

	Mesh Mesh::CreateFromObj(const std::filesystem::path& path)
	{
		return LoadFromObj(path);
	}


	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	MeshData Mesh::LoadFromObj(const std::filesystem::path& path)
	{
		objl::Loader loader;
		bool loadout = loader.LoadFile(path.string());

		if (!loadout)
		{
			LOG_ERROR("Failed to load mesh from path: {}", path.string());
			return {};
		}

		MeshData meshData;

		for (const auto& mesh : loader.LoadedMeshes)
		{
			uint32_t indexOffset = static_cast<uint32_t>(meshData.Vertices.size());
			for (const auto& vertex : mesh.Vertices)
			{
				glm::vec3 position(vertex.Position.X, vertex.Position.Y, vertex.Position.Z);
				glm::vec3 normal(vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z);
				glm::vec2 texCoord(vertex.TextureCoordinate.X, vertex.TextureCoordinate.Y);

				meshData.Vertices.emplace_back(position, normal, texCoord);
			}

			for (const auto& index : mesh.Indices)
			{
				meshData.Indices.push_back(index + indexOffset);
			}
		}
		return meshData;
	}
}