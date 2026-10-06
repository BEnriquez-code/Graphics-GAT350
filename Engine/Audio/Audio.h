#pragma once
#include "fmod.hpp"
#include "iostream"
#include "vector"
using namespace std;
namespace nu {
	struct SoundInfo {
		string name;
		FMOD::Sound* sound = nullptr;
	};

	class Audio {
	public:
		Audio() = default;

		bool Initialize();
		void Update();
		void Shutdown();

		bool AddSound(const string& name, const string& filenam);
		void PlaySound(const string& name);
	private:
		FMOD::System* m_audioSystem = nullptr;
		vector<SoundInfo> m_sounds;
	};
}