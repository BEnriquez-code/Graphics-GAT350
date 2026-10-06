#pragma once
#include "Component.h"
#include "Math/Vector2.h"
namespace nu {
    class PhysicsComponent;
    class TilemapRendererComponent;
    class PlatformGravityComponent : public Component {
    public:
        void Start() override;
        void Update(float dt) override;
        void Read(const json::value_t& value) override;
    private:
        PhysicsComponent* m_physicsComponent{ nullptr };
        TilemapRendererComponent* m_gravitySource{ nullptr };
        float m_gravityStrength{ 600.0f };
        std::string m_levelTag{ "Level" };
    };
}