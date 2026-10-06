#pragma once
#include "RendererComponent.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Tilemap.h"
namespace nu {
	class TilemapRendererComponent : public RendererComponent {

	public:
		TilemapRendererComponent() = default;
		TilemapRendererComponent(const TilemapRendererComponent& other);

		CLASS_PROTOTYPE(TilemapRendererComponent)


		void Start() override;
		void Draw(const Renderer& renderer) override;

		void Read(const json::value_t& value)override;
		Vector2 GetNearestTilePosition(const Vector2& worldPos) const;
	private:
		std::string m_tilemapName;
		res_t<class Tilemap> m_tilemap;

		std::vector<std::unique_ptr<class PhysicsBody>> m_physicsBodies;
		std::vector<Tilemap::GravityZone> m_gravityZones;
	};
}