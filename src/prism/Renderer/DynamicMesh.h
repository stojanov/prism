#pragma once
#include <glm/glm.hpp>

#include "prism/GL/VertexArray.h"
#include "prism/GL/VertexBuffer.h"
#include "prism/Core/Pointers.h"
#include "Vertex.h"
#include "prism/Components/IMesh.h"

namespace Prism::Renderer
{
	// Will render only triangles
	// The first vertex buffer will always be the positions buffer
	// If not passed a layout, assumed layout is a vec3 float positions
	// Supports only floating point vertices
	// TODO: Test Indexed Draw
	// Only rendering triangles
	class DynamicMesh: public IMesh
	{
	public:
		DynamicMesh();
		DynamicMesh(const std::initializer_list<Gl::BufferElement>& layout);
		
		uint32_t CreateNewVertexBuffer(const std::initializer_list<Gl::BufferElement>& layout);
		void SetIndexBuffer(Ptr<Gl::IndexBuffer>&& idxBuff);
		
		void AddVertexData(uint32_t vertIdx, std::vector<float>& data);
		void AddVertexData(std::vector<float>& data);
		
		template<typename T>
		uint32_t AddVertex(uint32_t idx,  const T& vert)
		{
			static_assert(IsVertexFloat<T>());
			constexpr int VertSize = GetVertexSize<T>();
			
			if constexpr (VertSize == 1)
			{
				m_VertexData[0].push_back(vert);
			}
			else if constexpr (VertSize == 2)
			{
				m_VertexData[0].push_back(vert.x);
				m_VertexData[0].push_back(vert.y);

			}
			else if constexpr (VertSize == 2)
			{
				m_VertexData[idx].push_back(vert.x);
				m_VertexData[idx].push_back(vert.y);
			}
			else if constexpr (VertSize == 3)
			{
				m_VertexData[idx].push_back(vert.x);
				m_VertexData[idx].push_back(vert.y);
				m_VertexData[idx].push_back(vert.z);
			}
			else if constexpr (VertSize == 4)
			{
				m_VertexData[idx].push_back(vert.x);
				m_VertexData[idx].push_back(vert.y);
				m_VertexData[idx].push_back(vert.z);
				m_VertexData[idx].push_back(vert.w);
			}
			return m_VertCount++;
		}
		
		template<typename T>
		uint32_t AddVertex(const T& vert)
		{
			return AddVertex<T>(0, vert);
		}
	
		void ConnectVertices(uint32_t idx1, uint32_t idx2, uint32_t idx3);
		void FlushVertexData(uint32_t vertIdx = 0);
		void FlushIndexData();
		void Flush();
		
		void DrawIndexed() const override;
		void DrawArrays() const override;
	private:
		std::vector<Gl::BufferLayout> m_Layouts;
		std::vector<std::vector<float>> m_VertexData;
		std::vector<uint32_t> m_IndexData;
		uint32_t m_VertCount{ 0 };
		
		std::vector<Ref<Gl::VertexBuffer>> m_VertexBuffers;
		Ref<Gl::IndexBuffer> m_IdxBuffer;
		Gl::VertexArray m_VertArray;
	};
}
