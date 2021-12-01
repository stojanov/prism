#include "VertexBuffer.h"
#include <glad/glad.h>

#include "prism/System/Log.h"


namespace Prism::Gl
{
	void VertexBuffer::CreateBuffer()
	{
		glCreateBuffers(1, &m_BufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
	}

	VertexBuffer::VertexBuffer(void* vertices, size_t size, const BufferLayout& layout, bool dynamic)
		:
			m_Dynamic(dynamic),
			m_GlBufferType(dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW),
			m_Layout(layout)
	{
		CreateBuffer();
		glBufferData(GL_ARRAY_BUFFER, size, vertices, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	}

	VertexBuffer::VertexBuffer(const BufferLayout& layout)
		:
			m_Dynamic(true),
			m_Layout(layout)
	{
		CreateBuffer();
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_BufferID);
	}

	Ref<VertexBuffer> VertexBuffer::Create(const BufferLayout& layout)
	{
		return MakeRef<VertexBuffer>(layout);
	}

	Ref<VertexBuffer> VertexBuffer::Create(void* data, size_t size, const BufferLayout& layout, const bool dynamic)
	{
		return MakeRef<VertexBuffer>(layout);
	}
	
	void VertexBuffer::SetData(void* data, size_t size) const
	{
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, data, m_GlBufferType);
	}

	void VertexBuffer::UpdateSubData(void* data, size_t offset, size_t size) const
	{
		Bind();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}
	
	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::Clear()
	{
		glDeleteBuffers(1, &m_BufferID);
	}
}
