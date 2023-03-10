#include "Tile.h"
#include "Config.h"
#include "Util.h"
#include <sstream>
#include <iomanip>

Tile::Tile() : m_cost(0.0f)
{
	SetWidth(Config::TILE_SIZE);
	SetHeight(Config::TILE_SIZE);
}

Tile::~Tile() = default;

void Tile::Draw()
{
	Util::DrawRect(GetTransform()->position, GetWidth(), GetHeight());
}

void Tile::Update() {}

void Tile::Clean() {}

Tile* Tile::GetNeighbourTile(NeighbourTile position)
{
	return m_neighbours[position];
}

void Tile::SetNeighbourTile(NeighbourTile position, Tile * tile)
{
	m_neighbours[position] = tile;
}

float Tile::GetTileCost() const
{
	return m_cost;
}

void Tile::SetTileCost(float cost)
{
	m_cost = cost;

	// format string to 1 decimal place
	std::stringstream stream;
	stream << std::fixed << std::setprecision(1) << m_cost;
	const std::string cost_string = stream.str();

	m_costLabel->SetText(cost_string);
}

TileStatus Tile::GetTileStatus() const
{
	return m_status;
}

void Tile::SetTileStatus(TileStatus status)
{
	m_status = status;

	switch (m_status)
	{
	case UNVISITED:
		m_statusLabel->SetText("=");
		break;
	case OPEN:
		m_statusLabel->SetText("O");
		break;
	case CLOSED:
		m_statusLabel->SetText("C");
		break;
	case IMPASSABLE:
		m_statusLabel->SetText("I");
		break;
	case GOAL:
		m_statusLabel->SetText("G");
		break;
	case START:
		m_statusLabel->SetText("S");
		break;
	}
}

void Tile::AddLabels()
{
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

	// cost label
	m_costLabel = new Label("99.9", "Consolas", 12);
	m_costLabel->GetTransform()->position = GetTransform()->position + offset + glm::vec2(0.0f, -6.0f);
	GetParent()->AddChild(m_costLabel);
	m_costLabel->SetEnabled(false);

	// status label
	m_statusLabel = new Label("=", "Consolas", 12);
	m_statusLabel->GetTransform()->position = GetTransform()->position + offset + glm::vec2(0.0f, 6.0f);
	GetParent()->AddChild(m_statusLabel);
	m_statusLabel->SetEnabled(false);
}

void Tile::SetLabelsEnabled(bool state)
{
	m_costLabel->SetEnabled(state);
	m_statusLabel->SetEnabled(state);
}