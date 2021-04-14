#include "PerlinNoise.h"

namespace Prism::Math
{
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

	void PerlinNoise::setScale(prdecimal s)
	{
		m_scale = s;
	}

	void PerlinNoise::offsetScale(prdecimal s)
	{
		m_scale *= s;
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
		prdecimal freq = 0;
		prdecimal amplitude = 0;

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
		prdecimal freq = 0;
		prdecimal amplitude = 0;

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