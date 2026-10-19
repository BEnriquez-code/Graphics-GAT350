// Engine.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "Engine.h"
#include "framework.h"

#include <iostream>

namespace nu {

	bool Engine::Initialize() {
		const int windowWidth = 1920;
		const int windowHeight = 1024;

		m_renderer.Initialize("Game Engine", windowWidth, windowHeight);
		m_input.Initialize();
		m_audio.Initialize();
		m_particleSystem.Initialize(1000000);

		return true;
	};
	void Engine::Shutdown() {
		m_audio.Shutdown();
		m_input.Shutdown();
		m_renderer.Shutdown();
		m_particleSystem.Shutdown();
	};
	void Engine::Update() {
		m_audio.Update();
		m_input.Update();
		m_time.Tick();
		m_particleSystem.Update(m_time.GetDeltaTime());
	};

}

