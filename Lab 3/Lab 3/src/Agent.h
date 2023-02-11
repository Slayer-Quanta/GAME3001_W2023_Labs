#pragma once
#ifndef __AGENT__
#define __AGENT__

#include <glm/vec4.hpp>
#include "NavigationObject.h"

class Agent : public NavigationObject
{
public:
	Agent();
	~Agent();

	// Inherited via GameObject
	void Draw() override = 0;
	void Update() override = 0;
	void Clean() override = 0;

	// getters
	[[nodiscard]] glm::vec2 GetTargetPosition() const;
	[[nodiscard]] glm::vec2 GetCurrentDirection() const;
	[[nodiscard]] float GetLOSDistance() const;
	[[nodiscard]] bool HasLOS() const;
	[[nodiscard]] float GetCurrentHeading() const;
	[[nodiscard]] glm::vec4 GetLOSColour() const;

	glm::vec2 GetLeftLOSEndPoint() const;
	glm::vec2 GetMiddleLOSEndPoint() const;
	glm::vec2 GetRightLOSEndPoint() const;
	glm::vec2 GetRighttimes2LOSEndPoint() const;	
	glm::vec2 GetLefttimes2LOSEndPoint() const;
	bool* GetCollisionWhiskers();
	glm::vec4 GetLineColor(int index);
	float GetWhiskerAngle() const;

	// setters
	void SetTargetPosition(glm::vec2 new_position);
	void SetCurrentDirection(glm::vec2 new_direction);
	void SetLOSDistance(float distance);
	void SetHasLOS(bool state);
	void SetCurrentHeading(float heading);
	void SetLOSColour(glm::vec4 colour);

	void SetLeftLOSEndPoint(glm::vec2 point);
	void SetMiddleLOSEndPoint(glm::vec2 point);
	void SetRightLOSEndPoint(glm::vec2 point);
	void SetRighttimes2LOSEndPoint(glm::vec2 point);
	void SetLefttimes2LOSEndPoint(glm::vec2 point);
	void SetLineColor(int index, glm::vec4 color);
	void SetWhiskerAngle(float angle);

	void UpdateWhiskers(float angle);

private:
	void ChangeDirection();
	float m_currentHeading;
	glm::vec2 m_currentDirection;
	glm::vec2 m_targetPosition;

	// LOS
	float m_LOSDistance;
	bool m_hasLOS;
	glm::vec4 m_LOSColour;

	// Whisker properties
	glm::vec2 m_leftLOSEndPoint;
	glm::vec2 m_middleLOSEndPoint;
	glm::vec2 m_rightLOSEndPoint;
	glm::vec2 m_righttimes2LOSEndPoint;
	glm::vec2 m_lefttimes2LOSEndpoint;
	glm::vec4 m_lineColor[5]; // One for each whisker. You will have to make it 5 later.
	bool m_collisionWhiskers[5]; // You will have to make it 5 later.
	float m_whiskerAngle;
};



#endif /* defined ( __AGENT__) */