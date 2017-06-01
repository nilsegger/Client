#pragma once
#include <SFML\System.hpp>
#include <iostream>
class PositionPacket
{
private:
	float t;
	float tOrigin;
	sf::Vector2f TargetedPosition;
	sf::Int32 TimeStamp;
	float DeltaTime;
public:
	PositionPacket();
	PositionPacket(float t,  sf::Vector2f TargetedPosition, sf::Int32 TimeStamp);
	PositionPacket(float t, sf::Vector2f TargetedPosition, sf::Int32 TimeStamp, float DeltaTime);
	float getMoveVector2f(sf::Vector2f &CurrentPosition, const float DeltaTime, int index,int radius);
	sf::Int32 getTimeStamp() const;
	sf::Vector2f getTargetedPosition() const;
	void subT(float amount);
	float getT() const;
	float getDeltaTime() const;
};

