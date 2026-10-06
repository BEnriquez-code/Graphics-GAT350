#pragma once

#include "Math/Vector2.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

namespace nu
{
    class Renderer
    {
    public:
        bool Initialize(const char* name, int width, int height);

        void SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255)const;
        void SetColor(float r, float g, float b, float a = 1.0f)const;

        void Clear()const ;
        void Present()const;

        bool BeginFrame();
		bool EndFrame()const;

        void DrawPoint(float x, float y)const ;
        void DrawFillRect(float x, float y, float w, float h) const;
		void DrawLine(float x1, float y1, float x2, float y2) const;
        void DrawRect(float x, float y, float w, float h)const;

        void DrawModel(const class Model& model, const struct Transform& transform) const;

        void DrawTexture(const class Texture& texture, float x, float y, float angle = 0.0f, float scale = 1.0f, bool flipH = false) const;
        void DrawTexture(const class Texture& texture, const struct Rect& source,  float x, float y, float angle = 0.0f, float scale = 1.0f, bool flipH = false) const;

        void Shutdown();

        int GetWidth() const { return m_width; }
        int GetHeight() const { return m_height; }

        void SetCameraEnabled(bool enabled = true) { m_cameraEnabled = enabled; }
        void SetCamera(const Vector2& camera) { m_camera = camera; }

        friend class Text;
		friend class Texture;

    private:
        SDL_Window* m_window = nullptr;
        SDL_Renderer* m_renderer = nullptr;

        SDL_GPUDevice* m_gpuDevice = nullptr;
        SDL_GPUCommandBuffer* m_commandBuffer = nullptr;
        SDL_GPURenderPass* m_renderPass = nullptr;
        
		bool m_cameraEnabled = true;
        Vector2 m_camera;

        int m_width = 0;
        int m_height = 0;
    };
}