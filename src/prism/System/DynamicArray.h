#pragma once

#include <type_traits>

#include "prism/Core/Pointers.h"

namespace Prism::System
{
	// Mainly used a buffer for OpenGl
	template<typename T>
	class DynamicArray
	{
	public:
		DynamicArray()
		{
			m_AllocatedSize = 64;
			m_Data = MakePtr<T[]>(m_AllocatedSize);
		}
		
		DynamicArray(size_t size)
		{
			m_AllocatedSize = size;
			m_Data = MakePtr<T[]>(m_AllocatedSize);
		}

		void Allocate(size_t size)
		{
			Destroy();
			m_AllocatedSize = size;
			m_Data = MakePtr<T[]>(m_AllocatedSize);
		}
		
		void Push(const T& element)
		{
			if (m_Pointer + 1> m_AllocatedSize)
			{
				Resize();
			}
			m_Data.get()[m_Pointer] = element;

			m_Pointer += 1;
		}
		
		void Push2(const T& element0, const T& element1)
		{
			if (m_Pointer + 2> m_AllocatedSize)
			{
				Resize();
			}

			T* ptr = &m_Data.get()[m_Pointer];

			ptr[0] = element0;
			ptr[1] = element1;

			m_Pointer += 2;
		}
		
		void Push3(T element0, T element1, T element2)
		{
			if (m_Pointer + 3 > m_AllocatedSize)
			{
				Resize();
			}

			T* ptr = &m_Data.get()[m_Pointer];

			m_Data.get()[m_Pointer] = element0;
			m_Data.get()[m_Pointer + 1]= element1;
			m_Data.get()[m_Pointer + 2]= element2;

			m_Pointer += 3;
		}
		
		void Push4(const T& element0, const T& element1, const T& element2, const T& element3)
		{
			if (m_Pointer + 4 > m_AllocatedSize)
			{
				Resize();
			}

			T* ptr = &m_Data.get()[m_Pointer];

			ptr[0] = element0;
			ptr[1] = element1;
			ptr[2] = element2;
			ptr[3] = element3;

			PR_INFO("Element push");
			
			m_Pointer += 4;
		}

		void Resize()
		{
			T* NewData = new T[m_AllocatedSize * 2];

			for (int i = 0; i < m_AllocatedSize; i++)
			{
				NewData[i] = m_Data.get()[i];
			}

			m_Data.release();

			m_Pointer <<= 1;
		}

		size_t MemorySize()
		{
			return m_Pointer * sizeof(T);
		}
		
		size_t Count()
		{
			return m_Pointer;
		}
		
		T* RawData()
		{
			return m_Data.get();
		}

		Ptr<T>& DataPtr()
		{
			return m_Data;
		}
		
		void Destroy()
		{
			m_Data.release();
		}
	private:
		Ptr<T[]> m_Data;
		size_t m_Pointer{ 0 };
		size_t m_AllocatedSize;
	};
}