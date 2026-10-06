#include "Enemy.h"
#include "Player.h"
#include "Renderer/Renderer.h"
#include "Engine.h"
#include "SpaceGame.h"
#include "Core/Factory.h"
#include "Components/PhysicsComponent.h"

#include <iostream>

FACTORY_REGISTER(Enemy)

void Enemy::Update(float dt) {
	Player* player = m_scene->FindByTag<Player>("Player");
	if (player) {
		nu::Vector2 direction = player->GetTransform().position - m_transform.position;

		float angle = direction.Angle();
		SetRotation(angle * nu::math::RadToDeg);

		nu::PhysicsComponent* physicsComponent = GetComponent<nu::PhysicsComponent>();
		if (physicsComponent) {
			nu::Vector2 forward{ 1, 0 }; // ->
			nu::Vector2 force = forward.Rotate(m_transform.rotation * nu::math::DegToRad) * m_speed;

			physicsComponent->ApplyForce(force);

			nu::Vector2 direction = player->GetTransform().position - m_transform.position;
			float rotation = direction.Angle();
			physicsComponent->SetRotation(rotation * nu::math::RadToDeg);
		}
	}

	//float thrust = 0.0f;

	//float rotate = 0.0f;

	//nu::Vector2 forward{ 1, 0 }; // ->
	//nu::Vector2 velocity = forward.Rotate(m_transform.rotation * nu::math::DegToRad) * thrust;
	//AddVelocity(velocity * dt);

	Actor::Update(dt);
}

void Enemy::OnCollision(Actor* other) {
	if (other->GetTag() == "PlayerBullet") {
		SetDestroyed();
		other->SetDestroyed();

		int enemyBaseValue = 100;		
		((SpaceGame*)m_scene->GetGame())->AddKillStreakPoints(enemyBaseValue);


		for (int i = 0; i < 100; i++)
		{
			nu::Particle particle;
			particle.position = m_transform.position;
			particle.color = { 1.0f, 1.0f, 1.0f };
			particle.lifespan = nu::RandomFloat(0.5f, 2.0f);
			particle.velocity = { nu::RandomFloat(-600.0f, 600.0f), nu::RandomFloat(-600.0f, 600.0f) };

			nu::Engine::Get().GetPS().AddParticle(particle);
		}
	}
	else if (other->GetTag() == "Player") {
		SetDestroyed();
		other->SetDestroyed();
	}
}

void Enemy::Read(const nu::json::value_t& value) {
	Actor::Read(value);

	JSON_READ_NAME(value, "speed", m_speed);

}