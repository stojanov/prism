#include "AllocatedMesh.h"

namespace Prism::Renderer
{
	AllocatedMesh::AllocatedMesh()
	{
		m_VertexArray = MakePtr<Gl::VertexArray>();
		m_GlIndexBuffer = Gl::IndexBuffer::CreateRef();
		m_VertexArray->SetIndexBuffer(m_GlIndexBuffer);
	
		CreateNewVertexBuffer({
			{ Gl::ShaderDataType::Float3, "position" }
		});
	}

	AllocatedMesh::AllocatedMesh(const std::initializer_list<Gl::BufferElement>& layout)
	{
		m_VertexArray = MakePtr<Gl::VertexArray>();
		m_GlIndexBuffer = Gl::IndexBuffer::CreateRef();
		m_VertexArray->SetIndexBuffer(m_GlIndexBuffer);

		CreateNewVertexBuffer(layout);
	}

	void AllocatedMesh::AllocateVertexBuffer(uint32_t bIdx, size_t size)
	{
		PR_ASSERT(bIdx < m_VertexBuffers.size(), "Cannot find vertex data");

		m_VertexBuffers[bIdx].Allocate(size);
		_IncAlloc();
	}

	void AllocatedMesh::AllocateIndexBuffer(size_t size)
	{
		m_IndexBuffer.Allocate(size);
		_IncAlloc();
	}

	uint8_t AllocatedMesh::CreateNewVertexBuffer(const std::initializer_list<Gl::BufferElement>& layout)
	{
		auto buff = Gl::VertexBuffer::CreateRef({ layout });

		m_VertexBuffers.emplace_back();

		m_VertexArray->AddVertexBuffer(buff);
		m_GlVertexBuffers.push_back(buff);

		return m_VertexBuffers.size() - 1;
	}

	void AllocatedMesh::ConnectVertices(uint32_t idx1, uint32_t idx2, uint32_t idx3)
	{
		m_IndexBuffer.Push3(idx1, idx2, idx3);
	}

	void AllocatedMesh::FlushVertexData(uint32_t bIdx)
	{
		PR_ASSERT(m_VertexBuffers.size() > bIdx, "Can't flush vertex buffer that doesn't exist!");
		PR_ASSERT(m_VertexBuffers[bIdx].Count() > 0, "Can't flush vertex buffer that doesn't have any data!");
		for (auto& buffer : m_VertexBuffers)
		{
			m_GlVertexBuffers[bIdx]->SetData(buffer.RawData(), buffer.MemorySize());
		}
	}

	void AllocatedMesh::FlushIndexData()
	{
		m_GlIndexBuffer->SetData(m_IndexBuffer.RawData(), m_IndexBuffer.MemorySize());
	}

	void AllocatedMesh::Flush()
	{
		PR_ASSERT(m_IsAllocated, "(AllocatedMesh) Mesh is not allocated, kind of defeats the purpose of an allocated mesh!?");
		FlushIndexData();

		for (auto i = 0; i < m_VertexBuffers.size(); i++)
		{
			FlushVertexData(i);
		}
	}

	void AllocatedMesh::ClearBuffers()
	{
		for (auto& vertBuff  : m_VertexBuffers)
		{
			vertBuff.Destroy();
		}
		m_IndexBuffer.Destroy();
	}

	void AllocatedMesh::ClearGPUBuffers()
	{
		// If gl buffer is cleared it has to be recreated
		for (auto& glBuff : m_GlVertexBuffers)
		{
			glBuff->Clear();
		}
		
		m_GlIndexBuffer->Clear();
	}

	void AllocatedMesh::DrawArrays() const
	{
		m_VertexArray->Bind();
		glDrawArrays(GL_TRIANGLES, 0, m_VertCount);
	}

	void AllocatedMesh::DrawIndexed() const
	{
		m_VertexArray->Bind();
		glDrawElements(GL_TRIANGLES, m_ElementCount, GL_UNSIGNED_INT, 0);
	}

	void AllocatedMesh::ClearGpuBuffers()
	{
		for (auto& buff : m_GlVertexBuffers)
		{
			buff->Clear();
		}
		m_GlIndexBuffer->Clear();
	}
}
