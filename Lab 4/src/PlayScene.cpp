#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "InputType.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::Start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::Draw()
{
	DrawDisplayList();
	
	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 255, 255, 255, 255);
}

void PlayScene::Update()
{
	UpdateDisplayList();
}

void PlayScene::Clean()
{
	RemoveAllChildren();
}

void PlayScene::HandleEvents()
{
	EventManager::Instance().Update();

	GetPlayerInput();
	GetKeyboardInput();
}

void PlayScene::GetPlayerInput()
{
	switch (m_pCurrentInputType)
	{
	case static_cast<int>(InputType::GAME_CONTROLLER):
	{
		// handle player movement with GameController
		if (SDL_NumJoysticks() > 0)
		{
			if (EventManager::Instance().GetGameController(0) != nullptr)
			{
				constexpr auto dead_zone = 10000;
				if (EventManager::Instance().GetGameController(0)->STICK_LEFT_HORIZONTAL > dead_zone)
				{
					
				}
				else if (EventManager::Instance().GetGameController(0)->STICK_LEFT_HORIZONTAL < -dead_zone)
				{
					
				}
				else
				{
					
				}
			}
		}
	}
	break;
	case static_cast<int>(InputType::KEYBOARD_MOUSE):
	{
		
	}
	break;
	case static_cast<int>(InputType::ALL):
	{
		if (SDL_NumJoysticks() > 0)
		{
			if (EventManager::Instance().GetGameController(0) != nullptr)
			{
				constexpr auto dead_zone = 10000;
				if (EventManager::Instance().GetGameController(0)->STICK_LEFT_HORIZONTAL > dead_zone
					|| EventManager::Instance().IsKeyDown(SDL_SCANCODE_D))
				{
					
				}
				else if (EventManager::Instance().GetGameController(0)->STICK_LEFT_HORIZONTAL < -dead_zone
					|| EventManager::Instance().IsKeyDown(SDL_SCANCODE_A))
				{
					
				}
				else
				{
					
				}
			}
		}
	}
	break;
	}
}

void PlayScene::GetKeyboardInput()
{
	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		Game::Instance().Quit();
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_1))
	{
		Game::Instance().ChangeSceneState(SceneState::START);
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_2))
	{
		Game::Instance().ChangeSceneState(SceneState::END);
	}
}

void PlayScene::BuildGrid()
{
	auto tileSize = Config::TILE_SIZE;
	// Add tiles to grid
	for (int row = 0; row < Config::ROW_NUM; row++)
	{
		for (int col = 0; col < Config::COL_NUM; col++)
		{
			Tile* tile = new Tile();
			tile->GetTransform()->position = glm::vec2(col * tileSize, row * tileSize);
			tile->SetGridPosition(col, row);
			AddChild(tile);
			tile->AddLabels();
			tile->SetEnabled(false);
			m_pGrid.push_back(tile);
		}
	}
}

void PlayScene::SetGridEnabled(bool state)
{
	m_isGridEnabled = state;
	for (auto tile : m_pGrid)
	{
		tile->SetEnabled(m_isGridEnabled);
		tile->SetLabelsEnabled(m_isGridEnabled);
	}
}

bool PlayScene::GetGridEnabled() const
{
	return m_isGridEnabled;
}

void PlayScene::m_computeTileCosts()
{
	float distance = 0.0f;
	float x = 0.0f;
	float y = 0.0f;
	for (auto tile : m_pGrid)
	{
		switch (m_CurrentHeuristic)
		{
		case MANHATTAN:
			x = abs(tile->GetGridPosition().x - m_pTarget->GetGridPosition().x);
			x = abs(tile->GetGridPosition().y - m_pTarget->GetGridPosition().y);
			distance = x + y;
			break;
		case EUCLIDEAN:
			distance = Util::Distance(tile->GetGridPosition(), m_pTarget->GetGridPosition());
			break;
		}
		tile->SetTileCost(distance);
	}
}
Tile* PlayScene::GetTile(int col, int row)
{
	return m_pGrid[(row * Config::COL_NUM) + col];
}

Tile* PlayScene::GetTile(glm::vec2 grid_position)
{
	const auto col = grid_position.x;
	const auto row = grid_position.y;
	return m_pGrid[(row * Config::COL_NUM) + col];
}

void PlayScene::Start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	// Setup the grid
	BuildGrid();
	SetGridEnabled(true);
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	m_CurrentHeuristic = MANHATTAN;
	// Set Input Type
	m_pCurrentInputType = static_cast<int>(InputType::KEYBOARD_MOUSE);

	// Add Game Objects
	m_pTarget = new Target();
	m_pTarget->GetTransform()->position = GetTile(15, 11)->GetTransform()->position + offset;
	m_pTarget->SetGridPosition(15.0f, 11.0f);
	AddChild(m_pTarget);
	
	m_pStarship = new Starship();
	m_pStarship->GetTransform()->position = GetTile(1, 3)->GetTransform()->position + offset;
	m_pStarship->SetGridPosition(1.0f, 3.0f);
	AddChild(m_pStarship);

	// Preload Sounds
	SoundManager::Instance().Load("../Assets/Audio/yay.ogg", "yay", SoundType::SOUND_SFX);
	SoundManager::Instance().Load("../Assets/Audio/thunder.ogg", "thunder", SoundType::SOUND_SFX);
	
	m_computeTileCosts();
	/* DO NOT REMOVE */
	ImGuiWindowFrame::Instance().SetGuiFunction([this] { GUI_Function(); });
}

void PlayScene::GUI_Function() 
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Lab 4 Debug Properties", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	ImGui::Text("Player Input");
	ImGui::RadioButton("Keyboard / Mouse", &m_pCurrentInputType, static_cast<int>(InputType::KEYBOARD_MOUSE)); ImGui::SameLine();
	ImGui::RadioButton("Game Controller", &m_pCurrentInputType, static_cast<int>(InputType::GAME_CONTROLLER)); ImGui::SameLine();
	ImGui::RadioButton("Both", &m_pCurrentInputType, static_cast<int>(InputType::ALL));

	ImGui::Separator();

	static bool toggleGrid = m_isGridEnabled;
	if (ImGui::Checkbox("Toggle Grid", &toggleGrid))
	{
		SetGridEnabled(toggleGrid);
	}

	ImGui::Separator();
	static int radio = m_CurrentHeuristic;
	ImGui::Text("Heuristic type");
	ImGui::RadioButton("Manhattan", &radio, MANHATTAN);
	ImGui::SameLine();
	ImGui::RadioButton("Euclidean", &radio, EUCLIDEAN);
	if (m_CurrentHeuristic != radio)
	{
		m_CurrentHeuristic = static_cast<Heuristic>(radio);
		m_computeTileCosts();
	}

	// Start Position (Starship)
	static int startPos[2] = { m_pStarship->GetGridPosition().x, m_pStarship->GetGridPosition().y };
	ImGui::PushItemWidth(100.0f);
	if (ImGui::SliderInt("Start X", &startPos[0], 0, Config::COL_NUM - 1))
	{
		m_pStarship->GetTransform()->position.x =
			GetTile(startPos[0], startPos[1])->GetTransform()->position.x + offset.x;
		m_pStarship->SetGridPosition(startPos[0], startPos[1]);
	}
	ImGui::SameLine();
	if (ImGui::SliderInt("Start Y", &startPos[1], 0, Config::ROW_NUM - 1))
	{
		m_pStarship->GetTransform()->position.y =
			GetTile(startPos[0], startPos[1])->GetTransform()->position.y + offset.y;
		m_pStarship->SetGridPosition(startPos[0], startPos[1]);
	}

	// Goal Position (Target)
	static int goalPos[2] = { m_pTarget->GetGridPosition().x, m_pTarget->GetGridPosition().y};
	ImGui::PushItemWidth(100.0f);
	if (ImGui::SliderInt("Goal X", &goalPos[0], 0, Config::COL_NUM-1))
	{
		m_pTarget->GetTransform()->position.x = 
			GetTile(goalPos[0], goalPos[1])->GetTransform()->position.x + offset.x;
		m_pTarget->SetGridPosition(goalPos[0], goalPos[1]);
	}
	ImGui::SameLine();
	if (ImGui::SliderInt("Goal Y", &goalPos[1], 0, Config::ROW_NUM - 1))
	{
		m_pTarget->GetTransform()->position.y =
			GetTile(goalPos[0], goalPos[1])->GetTransform()->position.y + offset.y;
		m_pTarget->SetGridPosition(goalPos[0], goalPos[1]);
	}

	ImGui::End();
}
