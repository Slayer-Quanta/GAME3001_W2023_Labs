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
	if (m_bDebugView) // Draw colliders for the objects.
	{
		Util::DrawCircle(m_pTarget->GetTransform()->position, m_pTarget->GetWidth() * 0.5f);
		if (m_pStarship->IsEnabled())
		{
			Util::DrawRect(m_pStarship->GetTransform()->position - 
			glm::vec2(m_pStarship->GetWidth() * 0.5f, m_pStarship->GetHeight() * 0.5f),
			m_pStarship->GetWidth(), m_pStarship->GetHeight());
			Util::DrawLine(m_pStarship->GetTransform()->position, m_pStarship->GetLeftLOSEndPoint(),
				m_pStarship->GetLineColor(0)); // Left whisker.
			Util::DrawLine(m_pStarship->GetTransform()->position, m_pStarship->GetMiddleLOSEndPoint(),
				m_pStarship->GetLineColor(1)); // Middle whisker.
			Util::DrawLine(m_pStarship->GetTransform()->position, m_pStarship->GetRightLOSEndPoint(),
				m_pStarship->GetLineColor(2)); // Right whisker.
			Util::DrawLine(m_pStarship->GetTransform()->position, m_pStarship->GetLefttimes2LOSEndPoint(),
				m_pStarship->GetLineColor(3)); // Left whisker.
			Util::DrawLine(m_pStarship->GetTransform()->position, m_pStarship->GetRighttimes2LOSEndPoint(),
				m_pStarship->GetLineColor(4)); // Right whisker.

			CollisionManager::RotateAABB(m_pStarship, m_pStarship->GetCurrentHeading());
		}
	}
	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 255, 255, 255, 255);
}

void PlayScene::Update()
{
	UpdateDisplayList();
	if (m_pStarship->IsEnabled())
	{
		DoWhiskerCollision();
		CollisionManager::AABBCheck(m_pObstacle, m_pStarship);
		if (!m_pStarship->GetRigidBody()->isColliding)
			CollisionManager::CircleAABBCheck(m_pTarget, m_pStarship);
	}
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

void PlayScene::DoWhiskerCollision()
{
	SDL_FRect obs = { m_pObstacle->GetTransform()->position.x - m_pObstacle->GetWidth() * 0.5f,
		m_pObstacle->GetTransform()->position.y - m_pObstacle->GetHeight() * 0.5f,
		m_pObstacle->GetWidth(), m_pObstacle->GetHeight() };

	bool collisions[5] = { false };

	SDL_FPoint ship = { m_pStarship->GetTransform()->position.x, m_pStarship->GetTransform()->position.y };
	SDL_FPoint ship_orig = ship;
	SDL_FPoint left = { m_pStarship->GetLeftLOSEndPoint().x, m_pStarship->GetLeftLOSEndPoint().y };
	SDL_FPoint middle = { m_pStarship->GetMiddleLOSEndPoint().x, m_pStarship->GetMiddleLOSEndPoint().y };
	SDL_FPoint right = { m_pStarship->GetRightLOSEndPoint().x, m_pStarship->GetRightLOSEndPoint().y };
	SDL_FPoint right2 = { m_pStarship->GetRighttimes2LOSEndPoint().x, m_pStarship->GetRighttimes2LOSEndPoint().y };
	SDL_FPoint left2 = { m_pStarship->GetLefttimes2LOSEndPoint().x, m_pStarship->GetLefttimes2LOSEndPoint().y };

	collisions[0] = SDL_IntersectFRectAndLine(&obs, &ship.x, &ship.y, &left.x, &left.y);
	ship = ship_orig;
	collisions[1] = SDL_IntersectFRectAndLine(&obs, &ship.x, &ship.y, &middle.x, &middle.y);
	ship = ship_orig;
	collisions[2] = SDL_IntersectFRectAndLine(&obs, &ship.x, &ship.y, &right.x, &right.y);
	ship = ship_orig;
	collisions[3] = SDL_IntersectFRectAndLine(&obs, &ship.x, &ship.y, &left.x, &left.y);
	ship = ship_orig;
	collisions[4] = SDL_IntersectFRectAndLine(&obs, &ship.x, &ship.y, &right.x, &right.y);
	ship = ship_orig;
	// Do two more whisker checks.

	for (unsigned i = 0; i < 3; i++)
	{
		m_pStarship->GetCollisionWhiskers()[i] = collisions[i]; // Getter used as Setter. 
		m_pStarship->SetLineColor(i, (collisions[i] ? glm::vec4(1, 0, 0, 1) : glm::vec4(0, 1, 0, 1)));
	}
}

void PlayScene::Start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";
	m_bDebugView = false;

	// Set Input Type
	m_pCurrentInputType = static_cast<int>(InputType::KEYBOARD_MOUSE);

	// Add Game Objects
	m_pTarget = new Target();
	m_pTarget->GetTransform()->position = glm::vec2(650.0f, 100.0f);
	AddChild(m_pTarget);
	
	m_pStarship = new Starship();
	m_pStarship->GetTransform()->position = glm::vec2(100.0f, 550.0f);
	m_pStarship->SetTargetPosition(m_pTarget->GetTransform()->position);
	m_pStarship->SetEnabled(false);
	AddChild(m_pStarship);

	m_pObstacle = new Obstacle();
	m_pObstacle->GetTransform()->position.x = 400.0f;
	AddChild(m_pObstacle);

	// Preload Sounds
	SoundManager::Instance().Load("../Assets/Audio/yay.ogg", "yay", SoundType::SOUND_SFX);
	SoundManager::Instance().Load("../Assets/Audio/thunder.ogg", "thunder", SoundType::SOUND_SFX);

	/* DO NOT REMOVE */
	ImGuiWindowFrame::Instance().SetGuiFunction([this] { GUI_Function(); });
}

void PlayScene::GUI_Function() 
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Your Window Title Goes Here", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	ImGui::Text("Player Input");
	ImGui::RadioButton("Keyboard / Mouse", &m_pCurrentInputType, static_cast<int>(InputType::KEYBOARD_MOUSE)); ImGui::SameLine();
	ImGui::RadioButton("Game Controller", &m_pCurrentInputType, static_cast<int>(InputType::GAME_CONTROLLER)); ImGui::SameLine();
	ImGui::RadioButton("Both", &m_pCurrentInputType, static_cast<int>(InputType::ALL));

	ImGui::Separator();

	// Debug Properties
	static bool toggleDebug = m_bDebugView;
	if (ImGui::Checkbox("Toggle Debug View", &toggleDebug))
	{
		m_bDebugView = toggleDebug;
	}

	ImGui::Separator();

	static float targetPos[2] = { 
		m_pTarget->GetTransform()->position.x,
		m_pTarget->GetTransform()->position.y
	};
	ImGui::PushItemWidth(100.0f);
	if (ImGui::SliderFloat("Target X", &targetPos[0], 0.0f, 800.0f))
	{
		m_pTarget->GetTransform()->position.x = targetPos[0];
		m_pStarship->SetTargetPosition(m_pTarget->GetTransform()->position);
	}
	ImGui::SameLine();
	if (ImGui::SliderFloat("Target Y", &targetPos[1], 0.0f, 600.0f))
	{
		m_pTarget->GetTransform()->position.y = targetPos[1];
		m_pStarship->SetTargetPosition(m_pTarget->GetTransform()->position);
	}
	ImGui::Separator();

	// obstacle Properties
	static float obstacle_position[2] = { m_pObstacle->GetTransform()->position.x,
		m_pObstacle->GetTransform()->position.y };
	if (ImGui::SliderFloat2("Obstacle Position", obstacle_position, 0.0f, 800.0f))
	{
		m_pObstacle->GetTransform()->position = glm::vec2(obstacle_position[0], obstacle_position[1]);
		m_pStarship->SetTargetPosition(m_pTarget->GetTransform()->position);
	}
	ImGui::Separator();


	ImGui::Separator();

	static bool toggleSeek = m_pStarship->IsEnabled();
	if (ImGui::Checkbox("Toggle Seek", &toggleSeek))
	{
		m_pStarship->SetEnabled(toggleSeek);
	}

	ImGui::Separator();

	static float acceleration = m_pStarship->GetAccelerationRate();
	if (ImGui::SliderFloat("Acceleration Rate", &acceleration, 0.0f, 50.0f))
	{
		m_pStarship->SetAccelerationRate(acceleration);
		m_pStarship->GetRigidBody()->acceleration =
			m_pStarship->GetCurrentDirection() * m_pStarship->GetAccelerationRate();
	}

	ImGui::Separator();

	static float turn_rate = m_pStarship->GetTurnRate();
	if (ImGui::SliderFloat("Turn Rate", &turn_rate, 0.0f, 20.0f))
	{
		m_pStarship->SetTurnRate(turn_rate);
	}
	
	ImGui::Separator();

	if (ImGui::Button("Reset"))
	{
		m_pStarship->Reset();
		m_pStarship->SetEnabled(false);
		toggleSeek = false; // Okay...?
		m_pTarget->GetTransform()->position = glm::vec2(500.0f, 100.0f);
		m_pStarship->SetTargetPosition(m_pTarget->GetTransform()->position);
	}

	ImGui::End();
}
