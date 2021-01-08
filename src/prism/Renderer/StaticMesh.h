#pragma once
#include "prism/GL/VertexArray.h"

namespace Prism::Renderer
{
	class StaticMesh
	{
	public:
		StaticMesh();
		
		void SetIndexBuffer();
		
	private:
		Gl::VertexArray m_VertArray;
	};
}
