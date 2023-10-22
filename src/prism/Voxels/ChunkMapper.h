#pragma once

namespace Prism::Voxel
{
	class Chunk;

	class ChunkMapper
	{
	public:
		enum class FaceSide
		{
			FRONT,
			BACK,
			LEFT,
			RIGHT,
			TOP
		};

		struct MappingData
		{
			std::function<void(const glm::vec3&)> ColorFn;
			std::function<void(std::array<glm::vec2, 4>)> TextureCoordFn;
			int MaxHeight;
			int MinHeight;
			bool isAttached{ false };
		};

		ChunkMapper() = default;
		virtual ~ChunkMapper() = default;

		virtual void Map(const MappingData& ctx, int x, int y, int height, FaceSide side) = 0;
	private:
	};
}