#pragma once
#include "Resources/ResourceManager.h"
#include "Math/Rect.h"
#include "Math/Vector2.h"
#include <vector>
namespace nu {

	class Tilemap : public Resource {

	public:
		struct Layer {
			int width = 0;
			int height = 0;
			bool hasCollision = false;
			std::vector<int> data;
			res_t<class Texture> texture;
		};

		struct GravityZone {
			Rect bounds;
			Vector2 direction;
		};

		bool Load(const std::string& filename, class Renderer& renderer);

		const std::vector<Layer>& GetLayers() const { return m_layers; }
		Rect GetTileRect(const Layer& layer, int tlieId);
		Vector2 GetTilePosition(const Layer& layer, int tileIndex);

		const std::vector<GravityZone>& GetGravityZones() const { return m_gravityZones; }

	private:
		int m_tileWidth = 0;
		int m_tileHeight = 0;

		std::vector<Layer> m_layers;
		std::vector<GravityZone> m_gravityZones;
	};
}