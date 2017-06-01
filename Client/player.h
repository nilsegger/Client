#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include <iostream>
#include "PlayerIndexSorter.h"
#include "PositionPacket.h"
#include "bullet.h"
class player
{
private:
	int index;
	bool isPlayerControlled;
	int radius;
	sf::CircleShape shape;
	sf::Vector2f Position;
	sf::Vector2f ServerPosition;

	std::vector <sf::Vector2f> LastPositions;
	float TimeSinceLastMatch = .0f;

	float speed = 250.f;
	std::vector <PositionPacket> NextPositions;
	std::vector <bullet> PlayerBullets;
	float rotation = 0;

	float MisMatchDistanceAllowed = 15.f;
	void ServerClientPositionMatch(const int i);

	sf::Texture PlayerTexture;
	sf::Sprite PlayerSprite;
	bool SpriteLoaded = false;

public:
	player();
	player(int radius, sf::Vector2f nPosition, int index, bool isPlayer=false);
	void draw(sf::RenderWindow & window);
	void UpdatePosition(sf::Vector2f pos, sf::Int32 TimeStamp);
	void update(float DeltaTime, sf::RenderWindow &window);
	void setPlayer(bool val);
	bool getIsPlayerControlled() const;
	int getBulletLength() const;
	void addBullet(bullet &newBullet);
	void updateBulletPosition(int index, sf::Vector2f pos, sf::Int32 TimeStamp, float DeltaTime);
	void removeBulletAtIndex(int i);
	PlayerIndexSorter BulletIndexer;
};

