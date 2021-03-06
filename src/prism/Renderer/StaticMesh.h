#pragma once

#include "prism/Components/IMesh.h"
#include "prism/GL/VertexArray.h"

namespace Prism::Renderer
{
	class StaticMesh: public IMesh
	{
	public:
		StaticMesh(const std::initializer_list<Gl::BufferElement>& layout);
	private:
		Gl::BufferLayout m_Layout;
		Gl::VertexArray m_VertArray;
	};
}
