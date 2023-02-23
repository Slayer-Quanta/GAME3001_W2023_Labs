#pragma once
#ifndef __TILE__
#define __TILE__

#include "Label.h"
#include "NavigationObject.h"
#include "NeighbourTile.h"
#include "TileStatus.h"

class Tile : public NavigationObject
{
public:
	// Constructor
	Tile();

	// Destructor
	~Tile();

	// DisplayObject Life-Cycle Functions
	void Draw() override;
	void Update() override;
	void Clean() override;

	// Getters and Setters
	Tile* GetNeighbourTile(NeighbourTile position);
	void SetNeighbourTile(NeighbourTile position, Tile* tile);

	float GetTileCost() const;
	void SetTileCost(float cost);

	TileStatus GetTileStatus() const;
	void SetTileStatus(TileStatus status);

	void AddLabels();
	void SetLabelsEnabled(bool state);

private:
	float m_cost;
	TileStatus m_status;

	Label* m_costLabel;
	Label* m_statusLabel;

	Tile* m_neighbours[NUM_OF_NEIGHBOUR_TILES];
};

#endif