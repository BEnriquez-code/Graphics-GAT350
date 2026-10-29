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

#define CHECK_UNIFORM_BOUNDARY(T) static_assert((sizeof(T) % 16) == 0);

#define MAX_LIGHTS 8

using namespace nu;
enum class LightType : uint32_t {
    Directional,
    Point,
    Spot
};

int main() {
    // SET DIRECTORY
    SetWorkingDirectory("Assets");

    Engine::Get().Initialize();
    auto model = Resources().Get<Model>("models/spot.obj", Engine::Get().GetRenderer());
    auto texture = Resources().Get<Texture>("textures/spot_diffuse.png", Engine::Get().GetRenderer());

    auto vshader = Resources().Get<nu::Shader>("shaders/phong.vert", Engine::Get().GetRenderer(), 3);
    auto fshader = Resources().Get<nu::Shader>("shaders/phong.frag", Engine::Get().GetRenderer(), 3, 1);

    auto pipeline = std::make_shared<Pipeline>();

    pipeline->AddVertexBuffer(sizeof(Model::Vertex));
    pipeline->AddVertexAttribute(0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, position));
    pipeline->AddVertexAttribute(1, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, offsetof(Model::Vertex, texcoord));
    pipeline->AddVertexAttribute(2, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, normal));
    pipeline->AddVertexAttribute(3, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, tangent));

    pipeline->Create(*vshader.get(), *fshader.get(),
        Engine::Get().GetRenderer().GetGPUDevice(),
        Engine::Get().GetRenderer().GetWindow());


    //UNIFORMS
    struct vertexUniform {
        glm::mat4 mvp;
        glm::mat4 model;
        glm::vec3 cameraPosition;
        float padding1;
    };
    CHECK_UNIFORM_BOUNDARY(vertexUniform)
    vertexUniform vertexUniform;

    struct Light {
        glm::vec3 posiiton;
        LightType type;
        glm::vec3 direction;
        float innerCutoff;
        glm::vec3 color;
        float intensity;
        glm::vec3 ambient;
        float outerCutoff;
        float range;
        glm::vec3 padding;
    };

    struct LightUniform {
        Light lights[MAX_LIGHTS];
        uint32_t numLights;
        glm::vec3 ambient;
    };
    CHECK_UNIFORM_BOUNDARY(LightUniform)

    LightUniform lightUniform;

    Light light1{
        .posiiton{10, 10, 10},
        .type{LightType::Point},
        .direction{0, 1, 0},
        .innerCutoff{ 0.3f },
        .color{1, 1, 1},
        .intensity{ 1 },
        .ambient{0.1f},
        .outerCutoff{ 0.6f },
        .range{ 40 }
    };

    Light light2{
        .posiiton{-10, 10, 10},
        .type{LightType::Point},
        .direction{0, 1, 0},
        .innerCutoff{ 0.3f },
        .color{0, 0, 1},
        .intensity{ 1 },
        .ambient{0.1f},
        .outerCutoff{ 0.6f },
        .range{ 40 }
    };

    lightUniform.lights[0] = light1;
    lightUniform.lights[1] = light2;
    lightUniform.numLights = 2;
    lightUniform.ambient = glm::vec3{ 0.2f };

    struct MaterialUniform {
        glm::vec4 color;
        glm::vec2 tiling;
        glm::vec2 offset;
        float shininesss;
        glm::vec3 padding;
    };
    CHECK_UNIFORM_BOUNDARY(MaterialUniform)

    MaterialUniform materialUniform{
        .color{1, 1, 1, 1},
        .tiling{1.0f, 1.0f},
        .offset{0.0f, 0.0f},
        .shininesss{ 64.0f }
    };

    vertexUniform.cameraPosition = { 0, 0, 5 };

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

        glm::vec3 direction{ 0 };
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) direction.x = -1;
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) direction.x = +1;
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_E)) direction.y = +1;
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_Q)) direction.y = -1;
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W)) direction.z = -1;
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_S)) direction.z = +1;

        lightUniform.lights[0].posiiton = glm::eulerAngleY(time) * glm::vec4{0, 0, 10, 1};
        

        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_1)) { 
            lightUniform.lights[0].type = LightType::Directional;
            lightUniform.lights[0].direction = glm::vec3{ 0, 1, 0 };
        }
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_2)) { 
            lightUniform.lights[0].type = LightType::Point; 
            lightUniform.lights[0].posiiton = glm::vec3{ 10, -10, 10 };
            lightUniform.lights[0].direction = glm::vec3{0, 1, 0};
        }
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_3)) {
            lightUniform.lights[0].type = LightType::Spot;
            lightUniform.lights[0].posiiton = glm::vec3{ 0, -10, 0 };
            lightUniform.lights[0].direction = glm::vec3{0, -1, 0 };
            lightUniform.lights[0].innerCutoff = glm::cos(glm::radians(10.0f));
            lightUniform.lights[0].outerCutoff = glm::cos(glm::radians(40.0f));
        }

		vertexUniform.cameraPosition += direction * dt * 4.0f;
        
        //Transform
        //transform.rotation.y = time * 30.0f;
        

        //View matrix
        auto view = glm::lookAtRH(vertexUniform.cameraPosition,vertexUniform.cameraPosition + glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec3{0.0f, 1.0f, 0.0f});
        //Projection matrix
		float apectRatio = Engine::Get().GetRenderer().GetWidth() / (float)Engine::Get().GetRenderer().GetHeight();
        auto projection = glm::perspective(glm::radians(70.0f), apectRatio,  0.01f, 20.0f);

        //MVP
        vertexUniform.model = transform.GetMatrix();
		vertexUniform.mvp = (projection * view * vertexUniform.model);

        Engine::Get().GetRenderer().BeginFrame();

        Engine::Get().GetRenderer().SetPipeline(*pipeline);
        Engine::Get().GetRenderer().SetVertexUniform(0, &vertexUniform, sizeof(vertexUniform));
        Engine::Get().GetRenderer().SetVertexUniform(1, &lightUniform, sizeof(lightUniform));
        Engine::Get().GetRenderer().SetVertexUniform(2, &materialUniform, sizeof(materialUniform));

        Engine::Get().GetRenderer().SetFragmentUniform(0, &vertexUniform, sizeof(vertexUniform));
        Engine::Get().GetRenderer().SetFragmentUniform(1, &lightUniform, sizeof(lightUniform));
        Engine::Get().GetRenderer().SetFragmentUniform(2, &materialUniform, sizeof(materialUniform));

        Engine::Get().GetRenderer().SetFragmentTexture(0, *texture);

        model->Draw(Engine::Get().GetRenderer());

        Engine::Get().GetRenderer().EndFrame();

    }

    // SHUTDOWN
    Engine::Get().Shutdown();

    return 0;
}