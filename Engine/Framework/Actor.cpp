#include "pch.h"
#include "Actor.h"
#include "Renderer/Renderer.h"
#include "Renderer/Texture.h"
#include "Math/MathUtils.h"
#include "Engine.h"
#include "Resources/ResourceManager.h"
#include "Components/RendererComponent.h"


namespace nu {
    FACTORY_REGISTER(Actor)

    Actor::Actor(const Actor& other):
        Object{other},
        m_tag{other.m_tag},
        m_transform{other.m_transform},
        m_lifespan{other.m_lifespan}
    {
        //clone all components
        for (const auto& component : other.m_components) {
            auto clone = std::unique_ptr<Component>(dynamic_cast<Component*>(component->Clone().release()));
            if (clone) {
                AddComponent(std::move(clone)); 
            }
        }
    }

    void Actor::Start() {
        for (auto& component : m_components) {
            component->Start();
        }
    }
    void Actor::OnDestroy() {
        for (auto& component : m_components) {
            component->Destroy();
        }
    }

    void Actor::Update(float dt) {

        //physics
        //m_transform.position += (m_velocity * dt);
       // m_velocity *= 0.9997f; // Apply friction to slow down the actor over time

        m_transform.position.x = math::Wrap(0.0f, 1920.0f, m_transform.position.x);
        m_transform.position.y = math::Wrap(0.0f, 1024.0f, m_transform.position.y);

        for (auto& component : m_components) {
            component->Update(dt);
        }

    }
    void Actor::Draw(const Renderer& render) const {

        for (auto& component : m_components) {
            auto rendererComponent = dynamic_cast<RendererComponent*>(component.get());
            if (rendererComponent) {
               rendererComponent->Draw(render);
            }
            
        }
    }

    float Actor::GetRadius() const {
   
	   return 0.0f;
    }

    void Actor::Read(const json::value_t& value) {
        Object::Read(value);

        if (JSON_HAS_NAME(value, "transform")) {
            m_transform.Read(JSON_GET_NAME(value, "transform"));
        }

        JSON_READ_NAME(value, "name", m_name);
        JSON_READ_NAME(value, "tag", m_tag);
        JSON_READ_NAME(value, "persistent", m_persistent);
        
        

        //read actor components
        if (JSON_HAS_NAME(value, "components")) {
            for (auto& componentValue : JSON_GET_NAME(value, "components").GetArray()) {


                std::string typeName;
                JSON_READ_NAME(componentValue, "type", typeName);

                auto component = Factory::Instance().Create<Component>(typeName);

                if (component) {
                    component->Read(componentValue);
                    AddComponent(std::move(component));
                }
            }
        }

    }

    void Actor::AddComponent(std::unique_ptr<Component> component){
        component->SetOwner(this);
        m_components.push_back(std::move(component));
    }

}