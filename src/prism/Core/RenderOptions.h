#pragma once

#include <glad/glad.h>

namespace Prism::Core
{
	class RenderOptions
	{
	public:
		RenderOptions();
		
		static void DepthTest(bool enabled);
		static void DrawWireframe(bool enabled);
		static void ShouldCullFaces(bool enabled = true);
		static void CullFaceOrder(GLenum order);
	};
}
