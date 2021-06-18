#include "DynamicMesh.h"

namespace Prism::Renderer
{
	DynamicMesh::DynamicMesh()
		:
		m_VertArray(MakePtr<Gl::VertexArray>())
	{
		m_IdxBuffer = Gl::IndexBuffer::CreateRef();
		m_VertArray->SetIndexBuffer(m_IdxBuffer);
		CreateNewVertexBuffer({
			{ Gl::ShaderDataType::Float3, "position" }
		});
	}

	DynamicMesh::DynamicMesh(const std::initializer_list<Gl::BufferElement>& bufferElements)
		:
		m_VertArray(MakePtr<Gl::VertexArray>())
	{
		m_IdxBuffer = Gl::IndexBuffer::CreateRef();
		m_VertArray->SetIndexBuffer(m_IdxBuffer);
		CreateNewVertexBuffer(bufferElements);
	}

	uint32_t DynamicMesh::CreateNewVertexBuffer(const std::initializer_list<Gl::BufferElement>& bufferElements)
	{
		auto buff = Gl::VertexBuffer::CreateRef({ bufferElements });

		m_VertexData.emplace_back();
		
		m_VertArray->AddVertexBuffer(buff);
		m_VertexBuffers.push_back(buff);
		
		return m_VertexBuffers.size() - 1;
	}

	void DynamicMesh::AllocateVertexBuffer(uint32_t vertIdx, size_t size)
	{
		m_VertexData[vertIdx].reserve(size);
	}

	void DynamicMesh::AllocateIndexBuffer(size_t size)
	{
		m_IndexData.reserve(size);
	}
	
	void DynamicMesh::SetIndexBuffer(Ref<Gl::IndexBuffer> idxBuff)
	{
		//m_IdxBuffer = std::move(idxBuff);
	}

	void DynamicMesh::AddVertexData(uint32_t vertIdx, std::vector<float>& data)
	{
		m_VertexData[vertIdx].insert(m_VertexData[vertIdx].end(), data.begin(), data.end());

		m_VertCount += data.size() / m_VertexBuffers[vertIdx]->GetLayout().GetLength();
	}

	void DynamicMesh::AddVertexData(std::vector<float>& data)
	{
		AddVertexData(0, data);
	}

	void DynamicMesh::ConnectVertices(uint32_t idx1, uint32_t idx2, uint32_t idx3)
	{
		m_IndexData.push_back(idx1);
		m_IndexData.push_back(idx2);
		m_IndexData.push_back(idx3);
		m_ElementCount += 3;
	}

	void DynamicMesh::FlushVertexData(uint32_t vertIdx)
	{
		PR_ASSERT(m_VertexBuffers.size() > vertIdx, "Can't flush vertex buffer that doesn't exist!");
		PR_ASSERT(m_VertexData[vertIdx].size() > 0, "Can't flush vertex buffer that doesn't have any data!");
		m_VertexBuffers[vertIdx]->SetData(m_VertexData[vertIdx], m_VertexData[vertIdx].size());
	}

	void DynamicMesh::FlushIndexData()
	{
		m_IdxBuffer->SetData(m_IndexData, m_ElementCount);
	}

	void DynamicMesh::Flush()
	{
		FlushIndexData();

		for (auto i = 0; i < m_VertexData.size(); i++)
		{
			FlushVertexData(i);
		}
	}
	
	void DynamicMesh::DrawIndexed() const
	{
		m_VertArray->Bind();
		//m_IdxBuffer->Bind();
		glDrawElements(GL_TRIANGLES, m_ElementCount, GL_UNSIGNED_INT, 0);
	}
	
	void DynamicMesh::DrawArrays() const
	{
		m_VertArray->Bind();
		glDrawArrays(GL_TRIANGLES, 0, m_VertCount);
	}

	void DynamicMesh::NewMesh()
	{
		ClearBuffers();
		ClearGpuBuffers();
		m_ElementCount = 0;
	}
	
	void DynamicMesh::ClearBuffers()
	{
		for (auto& vec: m_VertexData)
		{
			vec.clear();
		}
		m_IndexData.clear();
	}

	void DynamicMesh::ClearGpuBuffers()
	{
		for (auto& buff : m_VertexBuffers)
		{
			buff->Clear();
		}
		m_IdxBuffer->Clear();
	}
}