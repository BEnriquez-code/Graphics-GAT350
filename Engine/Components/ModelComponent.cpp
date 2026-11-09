#include "pch.h"
#include "ModelComponent.h"
#include "Resources/ResourceManager.h"
#include "Engine.h"

namespace nu
{
	FACTORY_REGISTER(ModelComponent)

	void ModelComponent::Draw(const Renderer& renderer) {
		ObjectUniform objectUniform{
			.model = GetOwner()->GetTransform().GetMatrix(),
			.normalMatrix = GetOwner()->GetTransform().GetMatrix()
		};

		renderer.SetVertexUniform(UniformSlot::Frame, &objectUniform, sizeof(objectUniform));
		renderer.SetFragmentUniform(UniformSlot::Frame, &objectUniform, sizeof(objectUniform));

		m_model->Draw(renderer);
	}
	void ModelComponent::Read(const json::value_t& value) {
		RendererComponent::Read(value);

		std::string modelName;
		JSON_READ_NAME(value, "model", modelName);

		m_model = Resources().Get<Model>(modelName, Engine::Get().GetRenderer());

	}
}