#include "pch.h"
#include "PlatformGravityComponent.h"
#include "PhysicsComponent.h"
#include "TilemapRendererComponent.h"
#include "Core/Factory.h"
#include "Framework/Actor.h"
#include "Math/MathUtils.h"
#include "Framework/Scene.h"

#include <iostream>

namespace nu {
	FACTORY_REGISTER(PlatformGravityComponent)

		void PlatformGravityComponent::Start() {
		m_physicsComponent = GetOwner()->GetComponent<PhysicsComponent>();
		assert(m_physicsComponent);

		auto levelActor = GetOwner()->GetScene()->FindByTag<Actor>(m_levelTag);
		assert(levelActor);

		m_gravitySource = levelActor->GetComponent<TilemapRendererComponent>();
		assert(m_gravitySource);
	}

	void PlatformGravityComponent::Update(float dt) {
		Vector2 pos = m_physicsComponent->GetPosition();
		Vector2 dir = m_gravitySource->GetNearestTilePosition(pos);


		m_physicsComponent->ApplyForce(dir * m_gravityStrength);
		float targetAngle = std::atan2(dir.y, dir.x) - (math::PI / 2.0f);
		m_physicsComponent->SetRotation(targetAngle);
	}

	void PlatformGravityComponent::Read(const json::value_t& value) {
		Component::Read(value);
		JSON_READ_NAME(value, "gravity_strength", m_gravityStrength);
		JSON_READ_NAME(value, "level_tag", m_levelTag);
	}
}