#pragma once


#include <glad/glad.h>
#include <vector>
#include <filesystem>
#include <glm.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

#include "Component.h"
#include "Log.h"

namespace Core
{

	struct MeshData
	{
		std::vector<glm::vec3> Vertices;
		std::vector<glm::vec2> Uvs;
		std::vector<glm::vec3> Normals;

		std::vector<uint32_t> VertexIndices;
		std::vector<uint32_t> UvIndices;
		std::vector<uint32_t> NormalIndices;

		void ToInterleaved(std::vector<float>& outVertices, std::vector<uint32_t>& outIndices) const;
	};

	struct Mesh : Component
	{

		Mesh(const std::vector<float>& vertices, const std::vector<uint32_t>& indices);
		Mesh(const MeshData& meshData);
		~Mesh();

		static MeshData LoadFromObj(const std::filesystem::path& path);

		unsigned int VAO, VBO, EBO;
		size_t IndexCount;
	};
}

