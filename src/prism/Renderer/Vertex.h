#pragma once

#include <glm.hpp>

namespace Prism::Renderer
{
	using Vertex1f = float;
	using Vertex2f = glm::vec2;
	using Vertex3f = glm::vec3;
	using Vertex4f = glm::vec4;
	using Vertex1i = int;
	using Vertex2i = struct { int x; int y; };
	using Vertex3i = struct { int x; int y; int z; };
	using Vertex4i = struct { int x; int y; int z; int w; };
	using VertexM4 = glm::mat4;
	using VertexM3 = glm::mat3;
	using Vertex1b = bool;

	static constexpr int VertexTypeCount = 11;

	// matrices not supported as of now
	template<typename T>
	constexpr int GetVertexSize()
	{
		if constexpr (
			std::is_same<T, Vertex2f>::value ||
			std::is_same<T, Vertex2i>::value
			)
		{
			return 2;
		}
		else if constexpr (
			std::is_same<T, Vertex3f>::value ||
			std::is_same<T, Vertex3i>::value
			)
		{
			return 3;
		}
		else if constexpr (
			std::is_same<T, Vertex4f>::value ||
			std::is_same<T, Vertex4i>::value
			)
		{
			return 4;
		}

		return 1; // matrices not supported as of now
	}

	template<typename T>
	constexpr bool IsVertexFloat()
	{
		return
			std::is_same<T, Vertex1f>::value ||
			std::is_same<T, Vertex2f>::value ||
			std::is_same<T, Vertex3f>::value ||
			std::is_same<T, Vertex4f>::value;
	}

	template<typename T>
	constexpr bool IsVertexInt()
	{
		return
			std::is_same<T, Vertex1i>::value ||
			std::is_same<T, Vertex2i>::value ||
			std::is_same<T, Vertex3i>::value ||
			std::is_same<T, Vertex4i>::value;
	}
}
