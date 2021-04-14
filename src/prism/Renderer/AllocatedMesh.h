#pragma once
#include "prism/Components/IMesh.h"
#include "prism/GL/Buffer.h"
#include "prism/System/Debug.h"
#include "prism/System/DynamicArray.h"
#include "Vertex.h"
#include "prism/GL/VertexArray.h"

namespace Prism::Renderer
{
	class AllocatedMesh: public IMesh
	{
	public:
		AllocatedMesh();
		AllocatedMesh(const std::initializer_list<Gl::BufferElement>& layout);
		
		void AllocateVertexBuffer(uint32_t bIdx, size_t size);
		void AllocateIndexBuffer(size_t size);
		uint8_t CreateNewVertexBuffer(const std::initializer_list<Gl::BufferElement>& layout);
		
		template<typename T>
		uint32_t AddVertex(uint32_t idx, const T& vert)
		{
			static_assert(IsVertexFloat<T>());
			constexpr int VertSize = GetVertexSize<T>();
			PR_ASSERT(idx < m_VertexBuffers.size(), "Cannot find vertex data");

			if constexpr (VertSize == 1)
			{
				m_VertexBuffers[idx].Push(vert);
			}
			else if constexpr (VertSize == 2)
			{
				m_VertexBuffers[idx].Push2(vert.x, vert.y);
			}
			else if constexpr (VertSize == 3)
			{
				m_VertexBuffers[idx].Push3(vert.x, vert.y, vert.z);
			}
			else if constexpr (VertSize == 4)
			{
				m_VertexBuffers[idx].Push4(vert.x, vert.y, vert.z, vert.w);
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

		void ConnectVertices(uint32_t idx1, uint32_t idx2, uint32_t idx3);

		void FlushVertexData(uint32_t bIdx);
		void FlushIndexData();
		void Flush();
		
		void ClearBuffers();
		void ClearGPUBuffers();
		
		void DrawArrays() const override;
		void DrawIndexed() const override;
	private:
		void _IncAlloc()
		{
			m_AllocCount++;

			if (m_VertexBuffers.size() + 1 == m_AllocCount)
			{
				m_IsAllocated = true;
			}
		}
		
		std::vector<System::DynamicArray<float>> m_VertexBuffers;
		System::DynamicArray<uint32_t> m_IndexBuffer;
		size_t m_VertCount{ 0 };
		size_t m_ElementCount{ 0 };
		bool m_IsAllocated{ false };
		int m_AllocCount = 0;
		
		Ptr<Gl::VertexArray> m_VertexArray;
		Ref<Gl::IndexBuffer> m_GlIndexBuffer;
		std::vector<Ref<Gl::VertexBuffer>> m_GlVertexBuffers;
	};
}
