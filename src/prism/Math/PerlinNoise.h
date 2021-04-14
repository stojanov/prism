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

		void offsetX(prdecimal x);
		void offsetY(prdecimal y);
		void offsetZ(prdecimal z);

		void setXOffset(prdecimal x);
		void setYOffset(prdecimal y);
		void setZOffset(prdecimal z);

		void setScale(prdecimal s);
		void offsetScale(prdecimal s);

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

		static constexpr int p[512] = {
			131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 160,
			190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33, 137,
			88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166, 91,
			77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 90,
			102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 15,
			135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123, 151,
			5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
			223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
			129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
			251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
			49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
			138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180,
			131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 160,
			190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33, 137,
			88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166, 91,
			77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 90,
			102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 15,
			135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123, 151,
			5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
			223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
			129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
			251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
			49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
			138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
		};
	};
}
