#include "PerlinNoise.h"

#include <random>

namespace Prism::Math
{
	int PerlinNoise::p[512] = {
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

	PerlinNoise::PerlinNoise(unsigned octaves, float persistence)
		:
		m_octaves(octaves),
		m_persistence(persistence)
	{}

	void PerlinNoise::offsetX(prdecimal x)
	{
		m_offsetX += x;
	}

	void PerlinNoise::offsetY(prdecimal y)
	{
		m_offsetY += y;
	}

	void PerlinNoise::offsetZ(prdecimal z)
	{
		m_offsetY += z;
	}

	void PerlinNoise::setXOffset(prdecimal x)
	{
		m_offsetX = x;
	}

	void PerlinNoise::setYOffset(prdecimal y)
	{
		m_offsetY = y;
	}

	void PerlinNoise::setZOffset(prdecimal z)
	{
		m_offsetY = z;
	}

	void PerlinNoise::SetScale(prdecimal s)
	{
		m_scale = s;
	}

	void PerlinNoise::OffsetScale(prdecimal s)
	{
		m_scale *= s;
	}

	void PerlinNoise::SetPersistance(prdecimal p)
	{
		m_persistence = p;
	}

	void PerlinNoise::SetOctaves(int o)
	{
		m_octaves = o;
	}

	void PerlinNoise::ReseedNoise()
	{
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_int_distribution dist(0, 255);

		for (int i = 0; i < 512; i++)
		{
			p[i] = dist(mt);
		}
	}

	void PerlinNoise::ReseedNoise(unsigned seed)
	{
		std::default_random_engine generator(seed);
		std::uniform_int_distribution dist(0, 255);

		for (int i = 0; i < 512; i++)
		{
			p[i] = dist(generator);
		}
	}

	prdecimal PerlinNoise::Perlin2(prdecimal x, prdecimal y)
	{
		int X = static_cast<int>(floor(x)) & 255,
			Y = static_cast<int>(floor(y)) & 255;

		x -= floor(x);
		y -= floor(y);

		prdecimal u = fade(x),
				v = fade(y);
		
		int A = p[X] + Y, AA = p[A], AB = p[A + 1],
			B = p[X + 1] + Y, BA = p[B], BB = p[B + 1];

		return lerp(v, lerp(u, grad(p[AA], x, y, 0),
			grad(p[BA], x - 1, y, 0)),
			lerp(u, grad(p[AB], x, y - 1, 0),
				grad(p[BB], x - 1, y - 1, 0)));
	}

	prdecimal PerlinNoise::Perlin3(prdecimal x, prdecimal y, prdecimal z)
	{
		int X = static_cast<int>(floor(x)) & 255,
			Y = static_cast<int>(floor(y)) & 255,
			Z = static_cast<int>(floor(z)) & 255;

		x -= floor(x);
		y -= floor(y);
		z -= floor(z);

		prdecimal u = fade(x),
			v = fade(y),
			w = fade(z);

		int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z,
			B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

		return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),
			grad(p[BA], x - 1, y, z)),
			lerp(u, grad(p[AB], x, y - 1, z),
				grad(p[BB], x - 1, y - 1, z))),
			lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),
				grad(p[BA + 1], x - 1, y, z - 1)),
				lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
					grad(p[BB + 1], x - 1, y - 1, z - 1))));
	}

	prdecimal PerlinNoise::Fractal2(prdecimal x, prdecimal y)
	{
		prdecimal total = 0;
		prdecimal freq;
		prdecimal amplitude;

		for (unsigned int i = 0; i < m_octaves - 1; i++)
		{
			freq = pow(2, i);
			amplitude = pow(m_persistence, i);

			total += Perlin2(
				(m_scale * (x + static_cast<double>(m_offsetX))) * freq,
				(m_scale * (y + static_cast<double>(m_offsetY))) * freq
			) * amplitude;
		}

		return total;
	}

	prdecimal PerlinNoise::Fractal3(prdecimal x, prdecimal y, prdecimal z)
	{
		prdecimal total = 0;
		prdecimal freq;
		prdecimal amplitude;

		for (unsigned int i = 0; i < m_octaves - 1; i++)
		{
			freq = pow(2, i);
			amplitude = pow(m_persistence, i);

			total += Perlin3(
				(m_scale * (x + static_cast<double>(m_offsetX))) * freq,
				(m_scale * (y + static_cast<double>(m_offsetY))) * freq,
				(m_scale * (z + static_cast<double>(m_offsetZ))) * freq
			) * amplitude;
		}

		return total;
	}
}
