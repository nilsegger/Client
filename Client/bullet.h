#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include "PositionPacket.h"
class bullet
{
private:
	unsigned int UniqueId;
	int radius;
	sf::CircleShape shape;
	sf::Vector2f Position;
	sf::Vector2f forward;
	std::vector <PositionPacket> NextPositions;
	bool DeleteOnNextInterval = false;
	bool died = false;
public:
	bullet();
	bullet(unsigned int UniqueId, int radius, sf::Vector2f nPosition);
	void setPos(sf::Vector2f pos);
	void draw(sf::RenderWindow & window);
	void UpdatePosition(sf::Vector2f pos, sf::Int32 TimeStamp, float DeltaTime);
	void update(float DeltaTime);
	void setDeleteOnNextInterval();
	bool getDeleteOnNextInterval() const;
	bool getDied() const;
	void setDied();
	sf::Vector2f getPositionCentered() const;
	unsigned int getUniqueId() const;
};

