#pragma once
#include "Core/Random.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/MathUtils.h"
#include "Math/Transform.h"
#include <fmod.hpp>
#include "Core/StringUtils.h"

#include "Core/Factory.h"

#include "Renderer/Text.h"


// systems
#include "Core/File.h"
#include "Renderer/Renderer.h"
#include "Input/Input.h"
#include "Core/GameTime.h"
#include "Audio/Audio.h"
#include "Renderer/ParticleSystem.h"
#include "Renderer/Texture.h"
#include "Serialization/Json.h"
#include "Physics/Physics.h"


//framework
#include "Framework/Game.h"
#include "Framework/Actor.h"
#include "Framework/Scene.h"

namespace nu {
	class Engine {
	public:
		static Engine& Get() { static Engine engine; return engine; };

		bool Initialize();
		void Shutdown();
		void Update();

		Input& GetInput() { return m_input; };
		Renderer& GetRenderer() { return m_renderer; };
		Audio& GetAudio() { return m_audio; };
		Time& GetTime() { return m_time; };
		Physics& GetPhysics() { return m_physics; }

		ParticleSystem& GetPS() { return m_particleSystem; }

		Engine(const Engine&) = delete;
		Engine& operator=(const Engine&) = delete;
		

	private:
		Engine() = default;

		Input m_input;
		Renderer m_renderer;
		Audio m_audio;
		Physics m_physics;

		Time m_time;
		ParticleSystem m_particleSystem;
	};
}