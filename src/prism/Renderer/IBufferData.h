#pragma once

#include <functional>

namespace Prism::Renderer
{
	enum class BufferDataType
	{
		Int,
		Float,
		Bool
	};
	
	template<BufferDataType DataType>
	class IBufferData
	{
	public:
		virtual ~IBufferData() = default;
		
		virtual void* GetData() = 0;
		
		static constexpr BufferDataType GetDataType()
		{
			return DataType;
		}
	private:
	};
}