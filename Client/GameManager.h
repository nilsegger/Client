#pragma once
#include <SFML\Network.hpp>
#include <iostream>
#include <vector>
#include "PlayerIndexSorter.h"

#define DISCOVER 0
#define DISCOVER_CONFIRMATION 1
#define CREATE_PLAYER 2
#define CREATE_PLAYER_CONFIRMATION 3
#define GAME_INPUTS 4
#define GAME_DATA 5
#define FOLLOW_PLAYER_DATA 6
#define FOLLOW_BULLET_DATA 7

#define DELETE_PLAYER 8
#define DELETE_BULLET 9

#define SERVER_FULL 10

#define UNIQUE_ID_NOT_FOUND 9999
#define INDEX_NOT_FOUND -1 

class GameManager
{
public:
	GameManager();
	bool CheckForNewData(sf::UdpSocket &socket);
};

