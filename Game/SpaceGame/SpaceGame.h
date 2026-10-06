#pragma once
#include "Framework/Game.h"
#include "Math/Vector2.h"
#include "Renderer/Text.h"
#include "Renderer/Texture.h"
#include "Resources/ResourceManager.h"

#include <vector>

class SpaceGame : public nu::Game {

public:
	enum class GameState {
		Title,
		StartGame,
		StartLevel,
		Game,
		GameOver
	};

	SpaceGame() = default;
	SpaceGame(nu::Scene* scene)
	{}

	bool Initialize() override;
	void Shutdown() override;

	void Update(float dt, const std::vector<nu::Vector2>& mousePoints);
	void Update(float dt) override;
	void Draw(class nu::Renderer& renderer) override;

	float PointToLineSegDistance(const nu::Vector2& A, const nu::Vector2& B, const nu::Vector2& C);
	void CheckLineCollisions(const std::vector<nu::Vector2>& mousePoints);

	void OnPlayerDead();
	void AddPoints(int points) { m_score += points; };
	void AddKillStreakPoints(int basePoints);

private:
	
	void SpawnPlayer();
	void SpawnEnemy();
	
	int m_prevScore{ -1 };
	int m_prevLives{ -1 };

	int m_score{ 0 };
	int m_lives{0};

	float m_stateTimer = 0.0f;

	float m_spawnTimer = 0.0f;
	float m_spawnTime = 5.0f;
	int m_spawnCount = 5;

	int m_killstreak = 0;
	float m_killstreakTimer = 0.0f;
	const float m_killstreakWindow = 2.5f;

	std::shared_ptr<nu::Texture> m_backgroundTexture{nullptr};

	nu::res_t<nu::Font> m_titleFont;
	nu::res_t<nu::Font> m_gameFont;

	nu::Text* m_titleText{ nullptr };

	nu::Font* m_gameOverFont{ nullptr };
	nu::Text* m_gameOverText{ nullptr };


	nu::Text* m_scoreText{ nullptr };
	nu::Text* m_livesText{ nullptr };

	nu::Font* m_playFont{ nullptr };
	nu::Text* m_playText{ nullptr };

	GameState m_gameState = GameState::Title;
};