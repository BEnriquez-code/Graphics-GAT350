#include "Player.h"
#include "Resources/ResourceManager.h"
#include "Bullet.h"
#include "Assets.h"
#include "Renderer/Renderer.h"
#include "Engine.h"
#include "Core/Factory.h"
#include "Components/PhysicsComponent.h"
#include <iostream>

FACTORY_REGISTER(Player)

void Player::Update(float dt) {
    float thrust = 0.0f;
    float rotate = 0.0f;

   
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) {
        rotate = -400.0f;
        
    }
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) { 
        rotate = +400.0f; 
    }

    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W)) { 
        thrust = +m_speed;
    }
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_S)) { 
        thrust = -m_speed; 
    }

    nu::PhysicsComponent* physicsComponent = GetComponent<nu::PhysicsComponent>();
    if (physicsComponent) {
        nu::Vector2 forward{ 1, 0 }; // ->
        nu::Vector2 force = forward.Rotate(m_transform.rotation * nu::math::DegToRad) * thrust;

        physicsComponent->ApplyForce(force);
        physicsComponent->ApplyTorque(rotate);

        nu::Vector2 position = physicsComponent->GetPosition();
        /*position.x = nu::math::Wrap(0.0f, 1200.f, position.x);
        position.y = nu::math::Wrap(0.0f, 1024.f, position.y);

        physicsComponent->SetPosition(position);*/

		nu::Engine::Get().GetRenderer().SetCamera(position);
    }

    SetRotation(m_transform.rotation + rotate * dt);


    if (nu::Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE)) {

        auto bullet = nu::Factory::Instance().Create<Bullet>("BulletPrototype");
        if (!bullet) {
            std::cerr << "Failed to Instantiate BulletPrototype" << std::endl;
            return;
        }
        bullet->SetTransform(m_transform);
        bullet->SetScale(2.0f);

        m_scene->AddActor(std::move(bullet));

    }

    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_X)) {
        nu::Engine::Get().GetTime().SetTimeScale(0.5f);
    }
    else {
        nu::Engine::Get().GetTime().SetTimeScale(1.0f);
    }
    
    nu::Particle particle;
    particle.position = m_transform.position;
    particle.texture = nu::Resources().Get<nu::Texture>("Textures/particle.png", nu::Engine::Get().GetRenderer());
    particle.lifespan = nu::RandomFloat(0.5f, 1.5f);
    particle.velocity = { nu::RandomFloat(-200.0f, 200.0f), nu::RandomFloat(-200.0f, 200.0f) };

    
    Actor::Update(dt);

}


void Player::Draw(const nu::Renderer& renderer) const {
	Actor::Draw(renderer);
}

void Player::OnCollision(Actor* other) {
   if (other->GetTag() == "Enemy") {
       std::cout << "Player destroyed" << std::endl;
       SetDestroyed();
       other->SetDestroyed();
   }
}