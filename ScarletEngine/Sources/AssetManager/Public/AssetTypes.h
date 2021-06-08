#pragma once

namespace ScarletEngine
{
	using AssetID = uint64_t;

	/** Types of asset files */
	enum class AssetType
	{
		Texture,
		Font,
		StaticMesh
	};
}