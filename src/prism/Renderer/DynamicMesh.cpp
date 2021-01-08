#include "DynamicMesh.h"

namespace Prism::Renderer
{
	DynamicMesh::DynamicMesh()
	{
		CreateNewVertexBuffer({
			{ Gl::ShaderDataType::Float3, "position" }
		});
	}

	DynamicMesh::DynamicMesh(const std::initializer_list<Gl::BufferElement>& bufferElements)
	{
		CreateNewVertexBuffer(bufferElements);
	}

	uint32_t DynamicMesh::CreateNewVertexBuffer(const std::initializer_list<Gl::BufferElement>& bufferElements)
	{
		auto buff = Gl::VertexBuffer::CreateRef({ bufferElements });
		
		m_VertArray.AddVertexBuffer(buff);
		m_VertexBuffers.push_back(buff);
		m_VertexData.emplace_back();

		return m_VertexBuffers.size() - 1;
	}
	
	void DynamicMesh::SetIndexBuffer(Ptr<Gl::IndexBuffer> idxBuff)
	{
		m_IdxBuffer = std::move(idxBuff);
	}

	void DynamicMesh::AddVertexData(uint32_t vertIdx, std::vector<float> data)
	{
		m_VertexData[vertIdx].insert(m_VertexData[vertIdx].end(), data.begin(), data.end());
		m_VertCount += data.size();
	}

	void DynamicMesh::AddVertexData(std::vector<float> data)
	{
		AddVertexData(0, data);
	}

	void DynamicMesh::ConnectTriangles(uint32_t idx1, uint32_t idx2, uint32_t idx3)
	{
		m_IndexData.push_back(idx1);
		m_IndexData.push_back(idx2);
		m_IndexData.push_back(idx3);
	}

	void DynamicMesh::FlushVertexData(uint32_t vertBuffIdx)
	{
		PR_ASSERT(m_VertexBuffers.size() > vertBuffIdx, "Can't flush vertex buffer that doesn't exist!");
		m_VertexBuffers[vertBuffIdx]->SetData(m_VertexData[vertBuffIdx],m_VertexData[vertBuffIdx].size() * sizeof(float));
	}

	void DynamicMesh::FlushIndexData()
	{
		
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
		m_VertArray.Bind();
		glDrawElements(GL_TRIANGLES, m_VertCount / 3, GL_UNSIGNED_INT, 0);
	}
	
	void DynamicMesh::DrawArrays() const
	{
		m_VertArray.Bind();
		glDrawArrays(GL_TRIANGLES, 0, m_VertCount);
	}

}