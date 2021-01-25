#pragma once

namespace Prism
{
	class Mesh
	{
	public:
		virtual void DrawIndexed() const = 0;
		virtual void DrawArrays() const = 0;
	};
}