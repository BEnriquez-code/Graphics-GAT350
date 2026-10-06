#include "SpaceGame.h"
#include "Engine.h"
#include "Player.h"
#include "Enemy.h"
#include "Assets.h"

#include <algorithm>
#include <vector>
#include <memory>

using namespace nu;

bool SpaceGame::Initialize() {
	SetWorkingDirectory("SpaceGame");
	Game::Initialize();
	m_scene = std::make_unique<Scene>();
	m_scene->SetGame(this);
	m_scene->Load("Data/scene.json");


	m_backgroundTexture = Resources().Get<Texture>("Textures/background.png", Engine::Get().GetRenderer());
	
	m_titleFont = std::make_shared<Font>();
	m_gameFont = std::make_shared<Font>();

	
	m_titleText = new Text(Resources().Get<Font>("Fonts/Arcade.ttf", 64));
	m_titleText->Create(Engine::Get().GetRenderer(), "Game Title", Color{ 1.0f, 1.0f, 1.0f });

	m_gameFont->Load("Fonts/Arcade.ttf", 32);

	m_scoreText = new Text(Resources().Get<Font>("Fonts/Arcade.ttf", 32));
	m_livesText = new Text(Resources().Get<Font>("Fonts/Arcade.ttf", 32));

	m_gameOverFont = new Font();
	m_gameOverFont->Load("Fonts/Arcade.ttf", 64);

	m_gameOverText = new Text(Resources().Get<Font>("Fonts/Arcade.ttf", 64));

	m_playFont = new Font();
	m_playFont->Load("Fonts/Arcade.ttf", 32);

	m_playText = new Text(Resources().Get<Font>("Fonts/Arcade.ttf", 32));
	m_playText->Create(Engine::Get().GetRenderer(), "Press SPACE to play", Color{ 1.0f, 1.0f, 1.0f });

	Engine::Get().GetAudio().AddSound("test", "Audio/test.wav");
	Engine::Get().GetAudio().AddSound("thrust", "Audio/thrust.wav");
	Engine::Get().GetAudio().AddSound("mario", "Audio/mario.mp3");
	Engine::Get().GetAudio().AddSound("hee-hee", "Audio/hee-hee.mp3");
	Engine::Get().GetAudio().AddSound("bass", "Audio/bass.wav");
	Engine::Get().GetAudio().AddSound("background", "Audio/background_music.ogg");

	return true;
}


void SpaceGame::Update(float dt, const std::vector<nu::Vector2>& mousePoints) {
	switch (m_gameState){
		case GameState::Title:
			if (Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE)) {
				m_gameState = GameState::StartGame;
			}
			break;
		case GameState::StartGame:
			m_score = 0;
			m_lives = 5;
			m_spawnTime = 5.0f;
			m_gameState = GameState::StartLevel;
			break;
		case GameState::StartLevel:
			m_scene->RemoveAllActors();
			SpawnPlayer();
			m_gameState = GameState::Game;

			break;
		case GameState::Game:
			if (m_killstreak > 0) {
				m_killstreakTimer += dt;
				if (m_killstreakTimer > m_killstreakWindow) {
					m_killstreak = 0;
				} 
			}

			m_spawnTimer -= dt;
			if (m_spawnTimer <= 0.0f) {
				m_spawnTimer = m_spawnTime;
				SpawnEnemy();
			}

			/*if (m_scene->FindByTag<Player>("Player") == nullptr) {
				OnPlayerDead();
			}*/


			CheckLineCollisions(mousePoints);
			break;
		case GameState::GameOver:
			break;

	default:
		break;
	}

	m_scene->Update(dt);
}

void SpaceGame::Update(float dt)
{
}

void SpaceGame::Draw(Renderer& renderer) {
	renderer.SetCameraEnabled(false);
	m_scene->Draw(renderer);

	switch (m_gameState) {

	case SpaceGame::GameState::Title:
		m_titleText->Create(Engine::Get().GetRenderer(), "Game Engine", Color{ 1.0f, 1.0f, 1.0f });
		m_titleText->Draw(renderer, 400, 400);
		break;
	case SpaceGame::GameState::StartGame:
		break;
	case SpaceGame::GameState::StartLevel:
		break;
	case SpaceGame::GameState::Game:
		renderer.DrawTexture(*m_backgroundTexture, 0.0f, 0.0f,  0.0f, 0.0f, false);
		m_scene->Draw(renderer);
		Engine::Get().GetPS().Draw(Engine::Get().GetRenderer());

		m_scoreText->Create(renderer, "Score: " + std::to_string(m_score), { 1.0f, 1.0f, 1.0f });
		m_scoreText->Draw(renderer, 30, 30);

		m_livesText->Create(renderer, "Lives: " + std::to_string(m_lives), { 1.0f, 1.0f, 1.0f });
		m_livesText->Draw(renderer, 30, 100);
		break;
	case SpaceGame::GameState::GameOver:
		m_gameOverText->Create(Engine::Get().GetRenderer(), "Game Over", Color{ 1.0f, 1.0f, 1.0f });
		m_gameOverText->Draw(renderer, 400, 400);

		m_playText->Draw(renderer, 400, 450);
		break;
	default:
		break;
	}
	renderer.SetCameraEnabled();
	Game::Draw(renderer);
}

float SpaceGame::PointToLineSegDistance(const nu::Vector2& A, const nu::Vector2& B, const nu::Vector2& C) {
	Vector2 AB = B - A;
	Vector2 AC = C - A;

	float lengthSq = AB.x * AB.x + AB.y * AB.y;
	if (lengthSq == 0.0f) return (C - A).Length();

	float t = (AC.x * AB.x + AC.y * AB.y) / lengthSq;
	t = std::clamp(t, 0.0f, 1.0f);

	Vector2 projection = A + AB * t;
	return (C - projection).Length();
}

void SpaceGame::CheckLineCollisions(const std::vector<Vector2>& mousePoints) {
	if (mousePoints.size() < 2) return;

	// Iterate through line segments
	for (size_t i = 0; i < mousePoints.size() - 1; ++i) {
		Vector2 p1 = mousePoints[i];
		Vector2 p2 = mousePoints[i + 1];
		
		auto& actors = m_scene->GetActors();
		for (auto& actor : actors) {
			// Check collision for both Enemies and Bullets
			if (actor->GetTag() == "Enemy" || actor->GetTag() == "PlayerBullet") {
				Vector2 pos = actor->GetTransform().position;
				float radius = actor->GetRadius(); // TODO FIX actor->GetRadius to SpriteRenderComponent

				if (PointToLineSegDistance(p1, p2, pos) <= radius * 8.0f) {
					// Collision action:
					if (actor->GetTag() == "Enemy" || actor->GetTag() == "PlayerBullet") {
						Vector2 pushDir = (pos - (p1 + p2) * 0.5f).Normalized();
						//actor->SetVelocity(pushDir * 300.0f);
					}
				}
			}
		}
	}
}

void SpaceGame::OnPlayerDead() {
	m_lives--;
	m_gameState = (m_lives == 0) ? GameState::GameOver : GameState::StartLevel;
}

void SpaceGame::AddKillStreakPoints(int basePoints) {
	if (m_killstreakTimer <= m_killstreakWindow) {
		m_killstreak++;
	}
	else {
		m_killstreak = 1;
	}

	m_killstreakTimer = 0.0f;

	float multiplier = std::pow(1.5f, static_cast<float>(m_killstreak - 1));
	int pointsEarned = static_cast<int>(basePoints * multiplier);

	m_score += pointsEarned;

}

void SpaceGame::SpawnPlayer() {
	auto player = Factory::Instance().Create<Actor>("PlayerPrototype");
	m_scene->AddActor(std::move(player));
}

void SpaceGame::SpawnEnemy() {
	auto enemy = Factory::Instance().Create<Actor>("EnemyPrototype");
	m_scene->AddActor(std::move(enemy));
}

void SpaceGame::Shutdown() {
	m_scene.reset();
}