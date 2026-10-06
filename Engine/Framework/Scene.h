#pragma once
#include "Actor.h"
#include <string>
#include <vector>
#include <memory>

namespace nu{
	class Scene {

	public:
		void AddActor(std::unique_ptr<Actor> actor);
		const std::vector<std::unique_ptr<Actor>>& GetActors() const { return m_actors; }
		void RemoveAllActors(bool force = false);

		bool Load(const std::string& sceneName);

		void Update(float dt);
		void Draw(const class Renderer& renderer);

		template<typename T>
		T* GetActorByName(const std::string& name);

		template<typename T>
		T* FindByTag(const std::string& tag);

		void SetGame(class Game* game) { m_game = game; };
		class Game* GetGame() { return m_game; };

	private:
		void UpdateCollisions();

		std::vector<std::unique_ptr<Actor>> m_actors;
		std::vector<std::unique_ptr<Actor>> m_pendingActors;

		class Game* m_game{ nullptr };
	};

	template<typename T>
	inline T* Scene::GetActorByName(const std::string& name) {
		for (auto& actor : m_actors) {
			T* actorT = dynamic_cast<T*>(actor.get());
			if (actorT && actorT->m_name == name) {
				return actorT;
			}
		}

		return nullptr;
	}

	template<typename T>
	inline T* Scene::FindByTag(const std::string& tag) {
		for (auto& actor : m_actors) {
			if (actor->GetTag() == tag) {
				if (T* result = dynamic_cast<T*>(actor.get())) {
					return result;
				}
			}
		}
		return nullptr;
	}
}	