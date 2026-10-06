#include "Bullet.h"
#include "Math/MathUtils.h"
#include "Core/Factory.h"
#include "Components/PhysicsComponent.h"

#include <iostream>

FACTORY_REGISTER(Bullet);

void Bullet::Update(float dt) {

	nu::PhysicsComponent* physicsComponent = GetComponent<nu::PhysicsComponent>();
	if (physicsComponent) {
		nu::Vector2 forward{ 1.0f, 0.0f };
		nu::Vector2 velocity = forward.Rotate(m_transform.rotation * nu::math::DegToRad) * m_speed;
		physicsComponent->SetVelocity(velocity);

		nu::Vector2 position = physicsComponent->GetPosition();
		position.x = nu::math::Wrap(0.0f, 1200.f, position.x);
		position.y = nu::math::Wrap(0.0f, 1024.f, position.y);

		physicsComponent->SetPosition(position);
	}

	m_lifespan -= dt;	

	if (m_lifespan <= 0.0f) {
		SetDestroyed();
	}

	Actor::Update(dt);
}

void Bullet::Read(const nu::json::value_t& value) {
	Actor::Read(value);

	JSON_READ_NAME(value, "speed", m_speed);
	JSON_READ_NAME(value, "lifespan", m_lifespan);
}