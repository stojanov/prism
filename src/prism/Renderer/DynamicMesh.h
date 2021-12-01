#pragma once
#include <glm.hpp>

#include "prism/GL/VertexArray.h"
#include "prism/GL/VertexBuffer.h"
#include "prism/Core/Pointers.h"
#include "Vertex.h"

namespace Prism::Renderer
{
	// Will render only triangles
	// The first vertex buffer will always be the positions buffer
	// If not passed a layout, assumed layout is a vec3 float positions
	// Supports only floating point vertices
	// TODO: Test Indexed Draw
	// Only rendering triangles

	class DynamicMesh
	{
	public:
		DynamicMesh();
		DynamicMesh(const std::initializer_list<Gl::BufferElement>& layout);
		
		uint32_t CreateNewVertexBuffer(const std::initializer_list<Gl::BufferElement>& layout);
		void AllocateVertexBuffer(uint32_t vertIdx, size_t size);
		void AllocateIndexBuffer(size_t size);
		void SetIndexBuffer(Ref<Gl::IndexBuffer>);
		
		void AddVertexData(uint32_t vertIdx, std::vector<float>& data);
		void AddVertexData(std::vector<float>& data);

		template<typename T>
		const std::vector<T>& GetVertexData(uint32_t vertIdx = 0)
		{
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

		template<typename T>
		void OverwriteVertex(uint32_t vertBufferIdx, int vertIdx, const T& vert)
		{
			static_assert(IsVertexFloat<T>());
			constexpr int VertSize = GetVertexSize<T>();
			PR_ASSERT(vertBufferIdx < m_VertexData.size(), "Cannot find vertex data");

			if constexpr (VertSize == 1)
			{
				m_VertexData[vertBufferIdx][vertIdx] = vert;
			}
			else if constexpr (VertSize == 2)
			{
				m_VertexData[vertBufferIdx][vertIdx] = vert.x;
				m_VertexData[vertBufferIdx][vertIdx + 1] = vert.y;
			}
			else if constexpr (VertSize == 3)
			{
				m_VertexData[vertBufferIdx][vertIdx] = vert.x;
				m_VertexData[vertBufferIdx][vertIdx + 1] = vert.y;
				m_VertexData[vertBufferIdx][vertIdx + 2] = vert.z;
			}
			else if constexpr (VertSize == 4)
			{
				m_VertexData[vertBufferIdx][vertIdx] = vert.x;
				m_VertexData[vertBufferIdx][vertIdx + 1] = vert.y;
				m_VertexData[vertBufferIdx][vertIdx + 2] = vert.z;
				m_VertexData[vertBufferIdx][vertIdx + 3] = vert.w;
			}
		}

		template<typename T>
		const T& GetVertex(uint32_t vertBufferIdx, uint32_t vertIdx)
		{
			constexpr int VertSize = GetVertexSize<T>();
			PR_ASSERT(vertBufferIdx < m_VertexData.size(), "Cannot find vertex data");

			if constexpr (VertSize == 1)
			{
				return m_VertexData[vertBufferIdx][vertIdx];
			}
			else if constexpr (VertSize == 2)
			{
				return Vertex2f{
					m_VertexData[vertBufferIdx][vertIdx],
					m_VertexData[vertBufferIdx][vertIdx + 1]
				};
			}
			else if constexpr (VertSize == 3)
			{
				return Vertex3f{
					m_VertexData[vertBufferIdx][vertIdx],
					m_VertexData[vertBufferIdx][vertIdx + 1],
					m_VertexData[vertBufferIdx][vertIdx + 2],
				};
			}
			else if constexpr (VertSize == 4)
			{
				return Vertex4f{
					m_VertexData[vertBufferIdx][vertIdx],
					m_VertexData[vertBufferIdx][vertIdx + 1],
					m_VertexData[vertBufferIdx][vertIdx + 2],
					m_VertexData[vertBufferIdx][vertIdx + 3]
				};
			}
		}

		const glm::vec3& GetVertex3f(uint32_t vertBufferIdx, uint32_t vertIdx)
		{
			return GetVertex<glm::vec3>(vertBufferIdx, vertIdx);
		}
		void ClearGpuBuffers();
		void ConnectVertices(uint32_t idx1, uint32_t idx2, uint32_t idx3);
		void FlushVertexData(uint32_t vertIdx = 0);
		void FlushIndexData();
		void Flush();
		
		void DrawIndexed() const;
		void DrawArrays() const;
		void NewMesh();
		void ClearBuffers();
	private:
		//std::vector<std::array<std::vector<float>, m_MaxTypes>> m_VertexData;
		std::vector<std::vector<float>> m_VertexData;
		std::vector<uint32_t> m_IndexData;
		uint32_t m_VertCount{ 0 };
		uint32_t m_ElementCount{ 0 }; // Store element count in case we destroy the buffer data
		std::vector<Ref<Gl::VertexBuffer>> m_VertexBuffers;
		Ref<Gl::IndexBuffer> m_IdxBuffer;
		Ptr<Gl::VertexArray> m_VertArray;
	};
}
