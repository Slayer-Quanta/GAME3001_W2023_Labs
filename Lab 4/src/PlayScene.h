#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Target.h"
#include "Starship.h"
#include "Tile.h"
#include "Heuristic.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene() override;

	// Scene LifeCycle Functions
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
	virtual void HandleEvents() override;
	virtual void Start() override;
private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	glm::vec2 m_mousePosition;

	// Input Control
	int m_pCurrentInputType{};
	void GetPlayerInput();
	void GetKeyboardInput();

	// Game Objects
	Target* m_pTarget;
	Starship* m_pStarship;

	// Pathfinding Members
	std::vector<Tile*> m_pGrid;
	bool m_isGridEnabled;

	void BuildGrid();
	void SetGridEnabled(bool state);
	bool GetGridEnabled() const;
	void m_computeTileCosts();

	// Convenience Methods
	Tile* GetTile(int col, int row);
	Tile* GetTile(glm::vec2 grid_position);

	Heuristic m_CurrentHeuristic;
};

#endif /* defined (__PLAY_SCENE__) */