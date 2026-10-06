#include "pch.h"
#include "RigidBodyPhysicsComponent.h"
#include "Core/Factory.h"
#include "Framework/Actor.h"
namespace nu {
	FACTORY_REGISTER(RigidBodyPhysicsComponent)

	void RigidBodyPhysicsComponent::Update(float dt) {
		m_velocity += m_acceleration * dt;
		m_velocity *= 1.0f / ((1.0f) + m_damping * dt);

		m_angularVelocity += m_angularAcceleration * dt;
		m_angularVelocity *= 1.0f / ((1.0f) + m_angularDamping * dt);

		Vector2 position = GetOwner()->GetTransform().position;
		position += m_angularVelocity * dt;
		GetOwner()->SetPosition(position);

		float rotation = GetOwner()->GetTransform().rotation;
		rotation += m_angularVelocity * dt;
		GetOwner()->SetRotation(rotation);

		m_acceleration = Vector2{ 0.0f };
		m_angularAcceleration = 0.0f;

	}

	void RigidBodyPhysicsComponent::Read(const json::value_t& value) {
		PhysicsComponent::Read(value);

		JSON_READ_OPTIONAL(value, "velocity", m_velocity);
		JSON_READ_OPTIONAL(value, "angular_velocity", m_angularVelocity);
	}

	void nu::RigidBodyPhysicsComponent::ApplyForce(const Vector2& force) {
		m_acceleration += force / m_mass;
	}

	void nu::RigidBodyPhysicsComponent::SetVelocity(const Vector2& velocity) {
		m_velocity = velocity;
	}

	Vector2 nu::RigidBodyPhysicsComponent::GetVelocity() {
		return m_velocity;
	}

	void nu::RigidBodyPhysicsComponent::ApplyTorque(float torque) {
		m_angularAcceleration += torque / m_mass;
	}

	void nu::RigidBodyPhysicsComponent::SetAngularVelocity(float angularVelocity) {
		m_angularVelocity = angularVelocity;
	}

	float nu::RigidBodyPhysicsComponent::GetAngularVelocity() const {

		return m_angularVelocity;
	}

	void nu::RigidBodyPhysicsComponent::SetPosition(const Vector2& position) {
		GetOwner()->SetPosition(position);
	}

	Vector2 nu::RigidBodyPhysicsComponent::GetPosition() const{
		return GetOwner()->GetTransform().position;
	}

	void RigidBodyPhysicsComponent::SetRotation(float rotation){
		GetOwner()->SetRotation(rotation);
	}
	float RigidBodyPhysicsComponent::GetRotation() const{
		return GetOwner()->GetTransform().rotation;
	}

	
	
}