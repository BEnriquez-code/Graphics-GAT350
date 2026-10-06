#include "pch.h"
#include "Box2DPhysicsComponent.h"
#include "Core/Factory.h"
#include "Framework/Actor.h"
#include "Math/MathUtils.h"
#include "Engine.h"

namespace nu {
	FACTORY_REGISTER(Box2DPhysicsComponent)

	Box2DPhysicsComponent::Box2DPhysicsComponent(const Box2DPhysicsComponent& other){
		m_bodydef = other.m_bodydef;
		m_size = other.m_size;
		m_scale = other.m_scale;
	}

	void Box2DPhysicsComponent::Start(){
		m_bodydef.actor = GetOwner();
		m_physicsBody = std::make_unique<PhysicsBody>(GetOwner()->GetTransform(), m_size, m_bodydef, Engine::Get().GetPhysics());
		m_physicsBody = std::make_unique<PhysicsBody>(GetOwner()->GetTransform(), m_size, m_bodydef, Engine::Get().GetPhysics());
	}

	void Box2DPhysicsComponent::Update(float dt){
		GetOwner()->SetPosition(GetPosition());
		GetOwner()->SetRotation(GetRotation() * nu::math::RadToDeg);
	}

    void Box2DPhysicsComponent::Read(const json::value_t& value)
    {
        PhysicsComponent::Read(value);

        JSON_READ_NAME(value, "size", m_size);
        JSON_READ_NAME(value, "scale", m_scale);

        JSON_READ_NAME(value, "gravity_scale", m_bodydef.gravityScale);
        JSON_READ_NAME(value, "linear_damping", m_bodydef.linearDamping);
        JSON_READ_NAME(value, "angular_damping", m_bodydef.angularDamping);
        JSON_READ_NAME(value, "constrain_angle", m_bodydef.constrainAngle);
        JSON_READ_NAME(value, "is_dynamic", m_bodydef.isDynamic);
        JSON_READ_NAME(value, "friction", m_bodydef.friction);
        JSON_READ_NAME(value, "restitution", m_bodydef.restitution);
        JSON_READ_NAME(value, "density", m_bodydef.density);
        JSON_READ_NAME(value, "is_sensor", m_bodydef.isSensor);

        std::string shapeName;
        JSON_READ_NAME(value, "shape", shapeName);

        if (!shapeName.empty())
        {
            if (EqualsIgnoreCase(shapeName, "box")) m_bodydef.shape = PhysicsBody::Shape::Box;
            else if (EqualsIgnoreCase(shapeName, "capsule")) m_bodydef.shape = PhysicsBody::Shape::Capsule;
            else if (EqualsIgnoreCase(shapeName, "circle")) m_bodydef.shape = PhysicsBody::Shape::Circle;
        }
    }

	void Box2DPhysicsComponent::ApplyForce(const Vector2& force){
		m_physicsBody->ApplyForce(force);
	}

	void Box2DPhysicsComponent::SetVelocity(const Vector2& velocity){
		m_physicsBody->SetVelocity(velocity);
	}

	Vector2 Box2DPhysicsComponent::GetVelocity(){
		return m_physicsBody->GetVelocity();
	}

	void Box2DPhysicsComponent::ApplyTorque(float torque){
		m_physicsBody->ApplyTorque(torque);
	}

	void Box2DPhysicsComponent::SetAngularVelocity(float angularVelocity){
		m_physicsBody->SetAngularVelocity(angularVelocity);
	}

	float Box2DPhysicsComponent::GetAngularVelocity() const{
		return m_physicsBody->GetAngularVelocity();
	}

	void Box2DPhysicsComponent::SetPosition(const Vector2& position){
		m_physicsBody->SetPosition(position);
	}
	Vector2 Box2DPhysicsComponent::GetPosition() const{
		return m_physicsBody->GetPosition();
	}

	void Box2DPhysicsComponent::SetRotation(float rotation){
		m_physicsBody->SetRotation(rotation);
	}

	float Box2DPhysicsComponent::GetRotation() const{
		return m_physicsBody->GetRotation();
	}
}