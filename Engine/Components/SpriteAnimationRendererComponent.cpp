#include "pch.h"
#include "SpriteAnimationRendererComponent.h"
#include "Core/Factory.h"
#include "Resources/ResourceManager.h"
#include "Renderer/TextureFrames.h"
#include "Math/MathUtils.h"
#include "Engine.h"
namespace nu {
	FACTORY_REGISTER(SpriteAnimationRendererComponent)



	void SpriteAnimationRendererComponent::Start(){
		if (!m_textureFramesName.empty()) {
			m_textureFrames = Resources().Get<TextureFrames>(m_textureFramesName, Engine::Get().GetRenderer());
			if (m_textureFrames) {
				m_sourceRect = m_textureFrames->GetFrameRect(0);
				m_size = Vector2{ m_sourceRect.w, m_sourceRect.h };
				m_texture = m_textureFrames->GetTexture();
			}

			if (!m_textureFrames) {
				std::cerr << "Could not load texture frames: " << m_textureFramesName << std::endl;
			}

		}
	}

	void SpriteAnimationRendererComponent::Update(float dt) {
		if (!m_textureFrames)return;
		unsigned total = m_textureFrames->GetTotalFrames();
		if (total == 0)return;

		m_frameTimer += dt;
		float frameTime = 0.1f / m_framesPerSecond;
		while (m_frameTimer >= frameTime) {
			m_frameTimer -= frameTime;
			m_frame++;
			
			if (m_frame >= total) {
				if (m_loop) {
					m_frame = m_frame % total;
				}
				else {
					m_frame = total - 1;
					m_frameTimer = 0;
					break;
				}
			}
		}
		m_sourceRect = m_textureFrames->GetFrameRect(m_frame);
	}
	void SpriteAnimationRendererComponent::Read(const json::value_t& value) {
		SpriteRendererComponent::Read(value);

		JSON_READ_OPTIONAL(value, "frames_per_second", m_framesPerSecond);
		JSON_READ_OPTIONAL(value, "loop", m_loop);

		
		JSON_READ_NAME(value, "texture_frames", m_textureFramesName);


	}
}