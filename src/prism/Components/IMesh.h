#pragma once

namespace Prism
{
	class IMesh
	{
	public:
		virtual void DrawIndexed() const = 0;
		virtual void DrawArrays() const = 0;
	};
}