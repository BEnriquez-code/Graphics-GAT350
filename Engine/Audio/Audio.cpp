#include "pch.h"
#include "Audio.h"
#include <iostream>
#include <vector>

namespace nu {

	bool Audio::Initialize(){
		if (FMOD::System_Create(&m_audioSystem) != FMOD_OK) {
			std::cout << "Falied to create FMOD system" << endl;
			return false;
		}
		void* extradriverdata = nullptr;
		if (m_audioSystem->init(32, FMOD_INIT_NORMAL, extradriverdata) != FMOD_OK) {
			std::cout << "Failed to initailize FMOD system" << endl;
			return false;
		}

		return true;
	}

	void Audio::Shutdown() {
		for (auto& soundInfo : m_sounds) {
			if (soundInfo.sound) {
				soundInfo.sound->release();
				soundInfo.sound = nullptr;
			}
		}
		m_sounds.clear();

		if (m_audioSystem) {
			m_audioSystem->close();
			m_audioSystem->release();
			m_audioSystem = nullptr;
		}
	}

	void Audio::Update() {
		m_audioSystem->update();
	}

	bool Audio::AddSound(const string& name, const string& filename) {
		FMOD::Sound* sound = nullptr;
		if (m_audioSystem->createSound(filename.c_str(), FMOD_DEFAULT, 0, &sound) == FMOD_OK) {
			m_sounds.push_back({ name , sound });
			return true;
		}
		
	}

	void Audio::PlaySound(const string& name) {
		for (const auto& soundInfo : m_sounds) {
			if (soundInfo.name == name) {
				m_audioSystem->playSound(soundInfo.sound, 0, false, nullptr);
				return;
			}
		}

	}
}