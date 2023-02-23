#include "Starship.h"
#include "TextureManager.h"
#include "Util.h"
#include "Game.h"

Starship::Starship()
	:m_maxSpeed(0.0f)
{
	TextureManager::Instance().Load("../Assets/textures/ncl_small.png","starship");

	const auto size = TextureManager::Instance().GetTextureSize("starship");
	SetWidth(static_cast<int>(size.x));
	SetHeight(static_cast<int>(size.y));
	GetRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->bounds = glm::vec2(GetWidth(), GetHeight());
	GetRigidBody()->isColliding = false;
	
	m_angle = 0.0; // Don't need.
	m_maxSpeed = 50.0f;
	Reset();
	SetType(GameObjectType::AGENT);

	// New for Lab 3
	SetLOSDistance(300.0f);
	SetWhiskerAngle(30.0f);
}

Starship::~Starship()
= default;

void Starship::Draw()
{
	// draw the target
	TextureManager::Instance().Draw("starship", 
		GetTransform()->position, static_cast<double>(GetCurrentHeading()), 255, true);
}

void Starship::Update()
{
	// Move();
}

void Starship::Clean()
{
}

float Starship::GetMaxSpeed() const
{
	return m_maxSpeed;
}

float Starship::GetTurnRate() const
{
	return m_turnRate;
}

float Starship::GetAccelerationRate() const
{
	return m_accelerationRate;
}

glm::vec2 Starship::GetDesiredVelocity() const
{
	return m_desiredVelocity;
}

void Starship::SetMaxSpeed(float speed)
{
	m_maxSpeed = speed;
}

void Starship::SetTurnRate(float angle)
{
	m_turnRate = angle;
}

void Starship::SetAccelerationRate(float rate)
{
	m_accelerationRate = rate;
}

void Starship::SetDesiredVelocity(glm::vec2 target_position)
{
	SetTargetPosition(target_position);
	m_desiredVelocity = Util::Normalize(target_position - GetTransform()->position);
	GetRigidBody()->velocity = m_desiredVelocity - GetRigidBody()->velocity;
}

void Starship::Seek()
{
	SetDesiredVelocity(GetTargetPosition());
	const glm::vec2 steering_direction = GetDesiredVelocity() - GetCurrentDirection();
	LookWhereYoureGoing(steering_direction);
	GetRigidBody()->acceleration = GetCurrentDirection() * GetAccelerationRate();
}

void Starship::LookWhereYoureGoing(glm::vec2 target_direction)
{
	const float target_rotation = Util::SignedAngle(GetCurrentDirection(), target_direction);
	if (GetCollisionWhiskers()[0])
	{
		SetCurrentHeading(GetCurrentHeading() + GetTurnRate());
	}
	else if (GetCollisionWhiskers()[2])
	{
		SetCurrentHeading(GetCurrentHeading() - GetTurnRate());
	}
	else if (abs(target_rotation) > m_turnRate)
	{
		SetCurrentHeading(GetCurrentHeading() + GetTurnRate() * Util::Sign(target_rotation));
	}
	UpdateWhiskers(GetWhiskerAngle());
}

void Starship::Move()
{
	Seek();
	//                      final Position  Position Term   Velocity			  Acceleration Term
	// Kinematic Equation-> Pf            = Pi +            Vi * (time) + (0.5) * Ai * (time * time)
	
	const glm::vec2 initial_position = GetTransform()->position;
	const glm::vec2 velocity_term = GetRigidBody()->velocity * Game::Instance().GetDeltaTime();
	const glm::vec2 acceleration_term = GetRigidBody()->acceleration * 0.5f;

	// Compute final position
	glm::vec2 final_position = initial_position + velocity_term + acceleration_term;
	GetTransform()->position = final_position;

	// Update velocity for next frame
	GetRigidBody()->velocity += GetRigidBody()->acceleration;
	GetRigidBody()->velocity = Util::Clamp(GetRigidBody()->velocity, GetMaxSpeed());
}

void Starship::CheckBounds(){}

void Starship::Reset()
{
	GetTransform()->position = glm::vec2(100.0f, 400.0f);
	SetCurrentHeading(0.0f);
	SetCurrentDirection(glm::vec2(1.0f, 0.0f));
	SetAccelerationRate(4.0f);
	SetTurnRate(5.0f);
}
