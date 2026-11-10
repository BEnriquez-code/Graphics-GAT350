#include "Engine.h"
#include "Core/File.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Pipeline.h"
#include "Framework/Scene.h"
#include "Components/CameraComponent.h"

#include "Resources/ResourceManager.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

#define MAX_LIGHTS 8

using namespace nu;


int main() {
    // SET DIRECTORY
    SetWorkingDirectory("Assets");

    Engine::Get().Initialize();

	Scene scene;
	scene.Load("scenes/scene.json");


    Transform transform;
    transform.scale = glm::vec3{ 2.0f };

    // MAIN LOOP
    bool quit = false;
    while (!quit) {

        // UPDATE
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }

            if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE) {
                quit = true;
            }
        }


        // ENGINE UPDATE (audio, input, time)
        Engine::Get().Update();
		float time = Engine::Get().GetTime().GetTime();
		float dt = Engine::Get().GetTime().GetDeltaTime();

        scene.Update(dt);
               
        Engine::Get().GetRenderer().BeginFrame();

		scene.Draw(Engine::Get().GetRenderer());

        Engine::Get().GetRenderer().EndFrame();

    }

    // SHUTDOWN
    Engine::Get().Shutdown();

    return 0;
}