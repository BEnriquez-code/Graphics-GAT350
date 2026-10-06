#pragma once
#include "Framework/Actor.h"

struct PlayerDesc : public nu::ActorDesc {
    float speed = 800.0f;
};

class Player : public nu::Actor {
public:
    Player() = default;

    Player(const PlayerDesc& playerDesc) :
        Actor(playerDesc),
        m_speed(playerDesc.speed) {
    }

    CLASS_PROTOTYPE(Player)

    void Update(float dt) override;
    void Draw(const class nu::Renderer& render) const override;

    void OnCollision(Actor* other);

private:
	int m_ammo = 0;
    float m_speed = 800.0f;
};