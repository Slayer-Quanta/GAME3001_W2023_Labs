#pragma once
#ifndef __STARSHIP__
#define __STARSHIP__

#include "Agent.h"

class Starship final : public Agent {
public:
	Starship();
	~Starship() override;

	// Inherited via GameObject
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;

	// Getters and Setters
	float GetMaxSpeed() const;
	float GetTurnRate() const;
	float GetAccelerationRate() const;
	glm::vec2 GetDesiredVelocity() const;

	void SetMaxSpeed(float speed);
	void SetTurnRate(float angle);
	void SetAccelerationRate(float rate);
	void SetDesiredVelocity(glm::vec2 target_position);

	// Movement methods (public)
	void Seek();
	void LookWhereYoureGoing(glm::vec2 target_direction);
	void Reset();

private:
	void Move();
	void CheckBounds();
	
	double m_angle; // This won't be needed anymore.
	float m_maxSpeed;
	float m_turnRate;
	float m_accelerationRate;
	glm::vec2 m_desiredVelocity;
};


#endif /* defined (__STARSHIP__) */