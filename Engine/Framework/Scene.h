#pragma once
#include "Actor.h"
#include "Editor/Editor.h"
#include <string>
#include <vector>
#include <memory>


namespace nu{
	class Scene : public IGUI
	{

	public:
		void AddActor(std::unique_ptr<Actor> actor);
		//const std::vector<std::unique_ptr<Actor>>& GetActors() const { return m_actors; }
		void RemoveAllActors(bool force = false);

		bool Load(const std::string& sceneName);
		void UpdateGui() override;

		void Update(float dt);
		void Draw(const class Renderer& renderer);

		template<typename T>
		T* GetActorByName(const std::string& name);

		template<typename T>
		std::vector<T*> GetActorByTag(const std::string& tag);

		template<typename T>
		T* FindByTag(const std::string& tag);

		const auto& GetActors() const { return m_actors; }

		void SetGame(class Game* game) { m_game = game; };
		class Game* GetGame() { return m_game; };

	private:

		std::vector<std::unique_ptr<Actor>> m_actors;
		std::vector<std::unique_ptr<Actor>> m_pendingActors;

		class Game* m_game{ nullptr };
		glm::vec3 m_ambientLight{0.2f};
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
	inline std::vector<T*> Scene::GetActorByTag(const std::string& tag)
	{
		std::vector<T*> result;
		for (auto& actor : m_actors) {
			if (actor->GetTag() == tag) {
				if (T* resultT = dynamic_cast<T*>(actor.get())) {
					result.push_back(resultT);
				}
			}
		}
		return result;
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