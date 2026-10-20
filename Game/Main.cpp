#include "Engine.h"
#include "Core/File.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Pipeline.h"

#include "Resources/ResourceManager.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>


using namespace nu;
namespace nu {
    struct Color4
    {
        float r, g, b, a;
    };
}

struct vertexUniform {
	glm::mat4 mvp;
};
vertexUniform uniform;
glm::vec3 cameraPos{ 0, 0, 5 };

struct Vertex
{
    Vector3 position;
	Color4 color;
};

std::vector<Vertex> vertices =
{
    Vertex{ Vector3{ -0.5f, -0.5f, 0.0f}, Color4{1, 0, 0, 1}}, // Bottom-Left
    Vertex{ Vector3{  0.5f, -0.5f, 0.0f}, Color4{0, 1, 0, 1}}, // Bottom-Right
    Vertex{ Vector3{  0.5f, 0.5f, 0.0f},  Color4{0, 0, 1, 1}}, //Top-Right
    Vertex{ Vector3{ -0.5f,  0.5f, 0.0f}, Color4{0, 1, 1, 1}}, // Top-left
};

std::vector<uint32_t> indices = {0, 1, 2, 2, 3, 0};


int main() {
    // SET DIRECTORY
    SetWorkingDirectory("Assets");

    Engine::Get().Initialize();
    auto model = Resources().Get<Model>("models/suzanne.obj", Engine::Get().GetRenderer());

    auto vshader = Resources().Get<nu::Shader>("shaders/unlit.vert", Engine::Get().GetRenderer(), 1);
    auto fshader = Resources().Get<nu::Shader>("shaders/unlit.frag", Engine::Get().GetRenderer(), 1);

    auto pipeline = std::make_shared<Pipeline>();

    pipeline->AddVertexBuffer(sizeof(Model::Vertex));
    pipeline->AddVertexAttribute(0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, position));
    pipeline->AddVertexAttribute(1, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, offsetof(Model::Vertex, texcoord));
    pipeline->AddVertexAttribute(2, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, normal));
    pipeline->AddVertexAttribute(3, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, tangent));

    pipeline->Create(*vshader.get(), *fshader.get(),
        Engine::Get().GetRenderer().GetGPUDevice(),
        Engine::Get().GetRenderer().GetWindow());

    Color4 color{ 1, 0, 0, 1 };

    Transform transform;

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

        glm::vec3 direction{ 0 };
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) direction.x = -1;
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) direction.x = +1;
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_E)) direction.y = +1;
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_Q)) direction.y = -1;
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W)) direction.z = -1;
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_S)) direction.z = +1;

		cameraPos += direction * dt * 4.0f;

        //Transform
        transform.rotation.z = time * 10.0f;

        //View matrix
        auto view = glm::lookAtRH(cameraPos,cameraPos + glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec3{0.0f, 1.0f, 0.0f});
        //Projection matrix
		float apectRatio = Engine::Get().GetRenderer().GetWidth() / (float)Engine::Get().GetRenderer().GetHeight();
        auto projection = glm::perspective(glm::radians(70.0f), apectRatio,  0.01f, 20.0f);

        //MVP
		uniform.mvp = (projection * view * transform.GetMatrix());

        Engine::Get().GetRenderer().BeginFrame();

        Engine::Get().GetRenderer().SetPipeline(*pipeline);
        Engine::Get().GetRenderer().SetVertexUniform(0, &uniform, sizeof(uniform));
        Engine::Get().GetRenderer().SetFragmentUniform(0, &color, sizeof(color));

        model->Draw(Engine::Get().GetRenderer());

        Engine::Get().GetRenderer().EndFrame();

    }

    // SHUTDOWN
    Engine::Get().Shutdown();

    return 0;
}