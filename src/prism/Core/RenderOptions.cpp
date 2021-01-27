#include "RenderOptions.h"

namespace Prism::Core
{
	RenderOptions::RenderOptions()
	{
		
	}
	
	void RenderOptions::DepthTest(bool enabled)
	{
		if (enabled)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

	void RenderOptions::DrawWireframe(bool enabled)
	{
		if (enabled)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	void RenderOptions::ShouldCullFaces(bool enabled)
	{
		if (enabled)
		{
			glEnable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
	}

	void RenderOptions::CullFaceOrder(GLenum order)
	{
		glCullFace(order);
	}

}