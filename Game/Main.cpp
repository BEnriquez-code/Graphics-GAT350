#include "Engine.h"
#include "Core/File.h"

#define MAX_LIGHTS 8

using namespace nu;

int main() {
    // SET DIRECTORY
    SetWorkingDirectory("Assets");

    Engine::Get().Initialize();

	Scene scene;
	scene.Load("scenes/scene.json");

    // MAIN LOOP
    bool quit = false;
    while (!quit) {

        // UPDATE
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            Engine::Get().GetEditor().ProcessEvent(event);

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
        Engine::Get().GetEditor().BeginFrame();
        Engine::Get().GetEditor().Update(scene);
               
        Engine::Get().GetRenderer().BeginFrame();

		scene.Draw(Engine::Get().GetRenderer());
        Engine::Get().GetRenderer().EndRenderPass();
        Engine::Get().GetEditor().Draw(Engine::Get().GetRenderer());

        Engine::Get().GetRenderer().EndFrame();
    }

    // SHUTDOWN
    Engine::Get().Shutdown();

    return 0;
}