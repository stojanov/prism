#include "IndexBuffer.h"

namespace Prism::Gl
{
	IndexBuffer::IndexBuffer()
	{
		CreateBuffer();
	}

	IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count)
	{
		CreateBuffer();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_DYNAMIC_DRAW);
	}

	IndexBuffer::IndexBuffer(std::vector<uint32_t>& indices)
	{
		CreateBuffer();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_DYNAMIC_DRAW);
	}
	
	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_BufferID);
	}

	Ptr<IndexBuffer> IndexBuffer::CreatePtr(uint32_t* indices, uint32_t count)
	{
		return MakePtr<IndexBuffer>(indices, count);
	}

	Ref<IndexBuffer> IndexBuffer::CreateRef(uint32_t* indices, uint32_t count)
	{
		return MakeRef<IndexBuffer>(indices, count);
	}

	Ref<IndexBuffer> IndexBuffer::CreateRef()
	{
		return MakeRef<IndexBuffer>();
	}

	Ptr<IndexBuffer> IndexBuffer::CreatePtr()
	{
		return MakePtr<IndexBuffer>();
	}

	void IndexBuffer::SetData(uint32_t* indices, uint32_t count)
	{
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), &indices[0], GL_DYNAMIC_DRAW);
	}

	void IndexBuffer::SetData(std::vector<uint32_t>& indices)
	{
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_DYNAMIC_DRAW);
	}
	
	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}