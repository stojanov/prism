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
		void AllocateBufferData(uint32_t vertIdx, size_t size);
		void AllocateIndexData(size_t size);
		void SetIndexBuffer(Ref<Gl::IndexBuffer>);
		
		void AddVertexData(uint32_t vertIdx, std::vector<float>& data);
		void AddVertexData(std::vector<float>& data);
		
		const std::vector<float>& GetVertexData(uint32_t vertIdx = 0)
		{
			//PR_CORE_ERROR("(DynamicMesh) Cannot get VertexData at {0}", vertIdx);
			PR_ASSERT(vertIdx < m_VertexData.size(), "Can't find vertex data");
			return m_VertexData[vertIdx];
		}

		const std::vector<uint32_t>& GetIndexData()
		{
			return m_IndexData;
		}
		
		template<typename T>
		uint32_t AddVertex(uint32_t idx, const T& vert)
		{
			static_assert(IsVertexFloat<T>());
			constexpr int VertSize = GetVertexSize<T>();
			PR_ASSERT(idx < m_VertexData.size(), "Cannot find vertex data");
			
			if constexpr (VertSize == 1)
			{
				m_VertexData[idx].push_back(vert);
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
			
			if (idx == 0)
			{
				return m_VertCount++;
			}

			return m_VertCount;
		}
		
		template<typename T>
		uint32_t AddVertex(const T& vert)
		{
			return AddVertex<T>(0, vert);
		}

		void ClearGpuBuffers();
		void ConnectVertices(uint32_t idx1, uint32_t idx2, uint32_t idx3);
		void FlushVertexData(uint32_t vertIdx = 0);
		void FlushIndexData();
		void Flush();
		
		void DrawIndexed() const override;
		void DrawArrays() const override;
		void NewMesh();
		void ClearBuffers();
	private:
		std::vector<std::vector<float>> m_VertexData;
		std::vector<uint32_t> m_IndexData;
		uint32_t m_VertCount{ 0 };
		uint32_t m_ElementCount{ 0 }; // Store element count in case we destroy the buffer data
		
		std::vector<Ref<Gl::VertexBuffer>> m_VertexBuffers;
		Ref<Gl::IndexBuffer> m_IdxBuffer;
		Ptr<Gl::VertexArray> m_VertArray;
	};
}
