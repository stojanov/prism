#pragma once

#include <cmath>

#include "General.h"
#include <corecrt_math_defines.h>

namespace Prism::Math
{
	class PerlinNoise
	{
	public:
		PerlinNoise(unsigned int octaves = 5, float persistence = 0.5);


		prdecimal Perlin2(prdecimal x, prdecimal y);
		prdecimal Perlin3(prdecimal x, prdecimal y, prdecimal z);

		prdecimal Fractal2(prdecimal x, prdecimal y);
		prdecimal Fractal3(prdecimal x, prdecimal y, prdecimal z);

		void ReseedNoise();
		void ReseedNoise(unsigned seed);

		void offsetX(prdecimal x);
		void offsetY(prdecimal y);
		void offsetZ(prdecimal z);

		void setXOffset(prdecimal x);
		void setYOffset(prdecimal y);
		void setZOffset(prdecimal z);

		void SetScale(prdecimal s);
		void OffsetScale(prdecimal s);
		void SetPersistance(prdecimal p);
		void SetOctaves(int o);

	private:
		prdecimal m_offsetX{ 0 };
		prdecimal m_offsetY{ 0 };
		prdecimal m_offsetZ{ 0 };
		prdecimal m_scale{ 0.1 };
		unsigned m_octaves{ 5 };
		prdecimal m_persistence{ 0.7 };

		// Using these ones in order to maintain maximum performance
		static prdecimal fade(prdecimal t) { return t * t * t * (t * (t * 6 - 15) + 10); }
		static prdecimal lerp(prdecimal t, prdecimal a, prdecimal b) { return a + t * (b - a); }
		static prdecimal cosinelerp(prdecimal t, prdecimal a, prdecimal b)
		{
			auto ft = t * M_PI;
			auto f = (1 - cos(ft)) * 0.5f;
			return  a * (1 - f) + b * f;
		}

		prdecimal grad(int hash, prdecimal x, prdecimal y, prdecimal z)
		{
			int h = hash & 15;
			prdecimal u = h < 8 ? x : y, v = h < 4 ? y : h == 12 || h == 14 ? x : z;
			return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
		}

		prdecimal grad(int hash, prdecimal x, prdecimal y)
		{
			int h = hash & 15;
			prdecimal u = h < 8 ? x : y, v = h < 4 ? y : h == 12 || h == 14 ? x : 0;
			return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
		}

		static int p[512];;
	};
}
