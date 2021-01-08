#pragma once
#include <cstdint>

#include "prism/Core/Core.h"
#include "Buffer.h"

namespace Prism::Gl
{
	class VertexBuffer : public Buffer
	{
	public:
		VertexBuffer(const BufferLayout& layout);
		VertexBuffer(float* vertices, size_t size, const BufferLayout& layout, bool dynamic = true);
		VertexBuffer(std::vector<float>& vertices, const BufferLayout& layout, bool dynamic = true);
		virtual ~VertexBuffer();

		static Ref<VertexBuffer> CreateRef(const BufferLayout& layout);
		static Ptr<VertexBuffer> CreatePtr(const BufferLayout& layout);
		static Ref<VertexBuffer> CreateRef(float* vertices, size_t size, const BufferLayout& layout, bool dynamic = true);
		static Ref<VertexBuffer> CreateRef(std::vector<float>& vertices, const BufferLayout& layout, bool dynamic = true);
		static Ptr<VertexBuffer> CreatePtr(float* vertices, size_t size, const BufferLayout& layout, bool dynamic = true);
		static Ptr<VertexBuffer> CreatePtr(std::vector<float>& vertices, const BufferLayout& layout, bool dynamic = true);
		
		const BufferLayout& GetLayout() { return m_Layout; };
		void SetData(float* vertices, size_t size);
		void SetData(std::vector<float>& vertices, size_t size);
		
		void Bind() const override;
		void Unbind() const override;
	private:
		void CreateBuffer();

		bool m_Dynamic{ false };
		unsigned m_BufferID;
		BufferLayout m_Layout;
	};
}
