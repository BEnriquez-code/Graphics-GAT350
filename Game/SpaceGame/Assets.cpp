#include "Assets.h"
using namespace nu;

namespace assets{
    Mesh cockpitMesh{
        {
            Vector2{ 2, 0 },
            Vector2{ 1, 0.5f },
            Vector2{ 1, -0.5f },
            Vector2{ 2, 0 }
        },
           Color{1.0f, 1.0f, 0.0f}
    };

    Mesh bodyMesh{
        {
           Vector2{1, 0.5f},
           Vector2{-2, 0.5f},
           Vector2{-2, -0.5f},
           Vector2{1, -0.5f},
           Vector2{1, 0.5f},
        },
        Color{0.0f, 1.0f, 1.0f}
    };

    Mesh leftWingMesh{
        {
            Vector2{0, 0.5f},
            Vector2{-1.5f, 2},
            Vector2{-2, 2},
            Vector2{-1.5, 0.5f}
        },
        Color{1.0f, 0.0f, 1.0f}
    };

    Mesh rightWinMesh{
        {
            Vector2{0, -0.5f},
            Vector2{-1.5f, -2},
            Vector2{-2, -2},
            Vector2{-1.5f, -0.5f}
        },
        Color{0.0f, 1.0f, 0.0f}
    };

    Mesh flameMesh{
        {
            Vector2{-2, 0.5},
            Vector2{-3, 0.7f},
            Vector2{-2.5f, 0.3f},
            Vector2{-3, 0},
            Vector2{-2.5f, -0.3f},
            Vector2{-3, -0.7f},
            Vector2{-2, -0.5f}
        },
        Color{1.0f, 0.0f, 0.0f}
    };

    std::shared_ptr<Model> playerModel = std::make_shared<Model>(std::vector<Mesh>{cockpitMesh, bodyMesh, leftWingMesh, rightWinMesh, flameMesh});

    Mesh enemyMesh{
        {
            Vector2{ 2, 0 },
            Vector2{ -2, 2 },
            Vector2{ -1, 0 },
            Vector2{ -2, -2 },
            Vector2{ 2, 0 }
        },
        Color{ 1.0f, 0.0f, 0.0f }
    };

    std::shared_ptr<Model> enemyModel = std::make_shared<Model>(std::vector<Mesh>{enemyMesh});

    Mesh bulletMesh{
        {
            Vector2{ -1, -1 },
            Vector2{ 1, 0 },
            Vector2{ -1, 1 },
            Vector2{ -1, -1 },
        },
        Color{ 0.0f, 1.0f, 0.0f }
    };

    std::shared_ptr<Model> bulletModel = std::make_shared<Model>(std::vector<Mesh>{bulletMesh});

}