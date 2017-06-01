///TODO
//Comment
#include <iostream>
#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>
#include "PlayerIndexSorter.h"
#include "player.h"
#include "bullet.h"
#include "StaticWorldItem.h"

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

#define PLAYER_DIED 12

bool StartWithFixServerIp = true;
sf::IpAddress FixServerIp = sf::IpAddress::Broadcast;

bool SetupCompleted = false;

unsigned int PlayerIndex = -1;

sf::UdpSocket socket;
unsigned short port = 4356;

unsigned short SERVER_PORT = 54000;
sf::IpAddress SERVER_IP;

unsigned short CUSTOM_SERVER_PORT;

std::vector <StaticWorldItem> StaticWorldItems; //add static world object and stuff

bool RespawnAvailable = false;

class Connection {
public:
	Connection() {};
	bool SendDiscover();
	bool SendCreatePlayer();
};


class GameManager {
private:
	bool loadingFontLoaded = false;
	sf::Font loadingFont;
	float s = 1.f;
	int frameCount = 0;
	int tempFrameCount = 0;
	float wait = 1.f;

	std::vector <player> PlayersVector;

	PlayerIndexSorter PlayerIndexKeeper;

	sf::Clock GameLoopClock;
	float DeltaTime = 0.f;
	float GameInterval = 0.f;
	Connection ConnectionObject;
public:
	GameManager():ConnectionObject() {};
	bool setup();
	bool ConnectToServer();
	bool SendPlayerRequest();
	bool CheckForNewData();
	bool Update(sf::RenderWindow &window);
	bool draw(sf::RenderWindow &window);
	bool drawMenu(sf::RenderWindow &window);
	bool displayText(sf::RenderWindow &window, sf::String text);
	void CheckForWindowClose(sf::RenderWindow &window);
	void drawFPS(sf::RenderWindow &window, const int frames);
	void Respawn();
};
GameManager GameManagerObject;
int main() {

	
	/* Create Window */
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(1000, 500), "Client", sf::Style::Close, settings);

	GameManagerObject.displayText(window, "Preparing socket");
	GameManagerObject.setup();

	GameManagerObject.displayText(window, "Connecting to server");

	if (GameManagerObject.ConnectToServer()) {
		GameManagerObject.displayText(window, "Connection succesfull");
	}
	else {
		GameManagerObject.displayText(window, "Connection to server failed");
		while (true) { //wait for user to close window
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed) { 
					window.close();
					return 0;
				}
			}
		}
		
	}

	/*
	
	*/

	GameManagerObject.displayText(window, "Preparing Objects");
	
	StaticWorldItem bg({ 0.f,0.f }, 1000, 500);
	bg.setTextureFile("bg.png");
	bg.LoadTexture();



	StaticWorldItems.push_back(bg);
	
	while (window.isOpen()) {
		GameManagerObject.CheckForWindowClose(window);
		if (SetupCompleted && RespawnAvailable == false) {
			
			GameManagerObject.CheckForNewData();
			GameManagerObject.Update(window);
			GameManagerObject.draw(window);
		} else if(!RespawnAvailable) {
			GameManagerObject.drawMenu(window);
		}
		else if(RespawnAvailable) {
			GameManagerObject.displayText(window, "Press enter to respawn");
			///Respawn window

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
				RespawnAvailable = false;
				GameManagerObject.Respawn();
				GameManagerObject.SendPlayerRequest();
			}

		}
	}

	return 0;
}



bool Connection::SendDiscover()
{

	int PacketsSent = 0;

	sf::Packet packet;
	packet << DISCOVER;
	socket.send(packet, SERVER_IP, SERVER_PORT);

	//Wait for Response
	sf::Clock WaitingTime;
	while (true) {
		sf::Packet newPacket;
		sf::IpAddress newIp;
		unsigned short newPort;
		if (socket.receive(newPacket, newIp, newPort) == sf::Socket::Done) {
			int msg;
			newPacket >> msg;
			if (msg == DISCOVER_CONFIRMATION) {

				//std::cout << "Connected with server." << std::endl;
				return true;

			}

		}

		if (WaitingTime.getElapsedTime().asSeconds() >= 1.25) {
			if (PacketsSent == 3) {
				return false;
			}
			sf::Packet packet;
			packet << DISCOVER;
			socket.send(packet, SERVER_IP, SERVER_PORT);
			std::cout << "Retrying..." << std::endl;
			WaitingTime.restart();
			PacketsSent++;
		}

	}
}

bool Connection::SendCreatePlayer()
{
	sf::Packet packet;
	packet << CREATE_PLAYER;
	socket.send(packet, SERVER_IP, SERVER_PORT);

	//Wait for Response

	while (true) {
		sf::Packet newPacket;
		sf::IpAddress newIp;
		unsigned short newPort;
		if (socket.receive(newPacket, newIp, newPort) == sf::Socket::Done) {
			int msg;
			newPacket >> msg;
			if (msg == CREATE_PLAYER_CONFIRMATION) {
				sf::Int32 index;
				newPacket >> index;
				PlayerIndex = int(index); // Player unique id
				CUSTOM_SERVER_PORT = newPort;

				std::cout << "Connected|" << newPort << std::endl;
				return true;

			}
			else if (msg == SERVER_FULL) {
				std::cout << "Server is full. Queue doesnt exist yet..." << std::endl;
				return false;
			}

		}
	}
}

bool GameManager::setup()
{
	while (socket.bind(port) != sf::Socket::Done) {
		port++;
	}
	socket.setBlocking(false);

	std::cout << sf::IpAddress::getLocalAddress() << "|" << port << std::endl;
	if (StartWithFixServerIp) {
		SERVER_IP = FixServerIp;
	}
	else {
		std::cout << "Enter the IpAdress of the server:" << std::endl;
		std::cin >> SERVER_IP;
	}
	return true;
}

bool GameManager::ConnectToServer()
{
	while (!ConnectionObject.SendDiscover()) {
		std::cout << "Couldnt reach server.." << std::endl;
		return false;
		/*std::cout << "Enter the IpAdress of the server:" << std::endl;

		std::cin >> SERVER_IP;*/
	}


	/*if (!ConnectionObject.SendCreatePlayer()) {
		std::cout << "Something went wrong requesting player" << std::endl;
		return false;
	}*/

	return true;
}

bool GameManager::SendPlayerRequest() {
	if (!ConnectionObject.SendCreatePlayer()) {
		std::cout << "Something went wrong requesting player" << std::endl;
		return false;
	}
	
	return true;
}

bool GameManager::CheckForNewData()
{
	//Insert Player Data
	//if there is new one
	sf::Packet NewPacket;
	sf::IpAddress NewIp;
	unsigned short NewPort;

	if (socket.receive(NewPacket, NewIp, NewPort) == sf::Socket::Done) {
		int type;
		NewPacket >> type;
		if (type == GAME_DATA) {

			sf::Int32 TimeStamp;
			NewPacket >> TimeStamp;

			while (true) {

				int inner_type;
				if (NewPacket >> inner_type) {
					if (inner_type == FOLLOW_PLAYER_DATA) {

						sf::Uint32 UniqueId;
						sf::Vector2f NewPos;
						sf::Int64 radius;


						NewPacket >> UniqueId;
						NewPacket >> NewPos.x;
						NewPacket >> NewPos.y;
						NewPacket >> radius;

						int index = PlayerIndexKeeper.getIndex(unsigned int(UniqueId));
						if (index != INDEX_NOT_FOUND) {
							PlayersVector[index].UpdatePosition(NewPos, TimeStamp);
							std::cout << "Updating message received" << std::endl;
						}
						else {
							std::cout << "New Player joined" << std::endl;
							PlayerIndexKeeper.addCombo(unsigned int(UniqueId), int(PlayersVector.size()));
							PlayersVector.push_back(player(int(radius), NewPos, int(index)));

							if (unsigned int(UniqueId) == PlayerIndex) {
								PlayersVector[int(PlayersVector.size()) - 1].setPlayer(true);
							}

							///find out if new player is this player
							///maybe make server send extra message
						}
					}
					else if (inner_type == DELETE_PLAYER) {
						/// Player Deletion
						sf::Uint32 UniqueId;
						NewPacket >> UniqueId;

						int i = PlayerIndexKeeper.getIndex(unsigned int(UniqueId));
						if (i != INDEX_NOT_FOUND) {
							PlayersVector.erase(PlayersVector.begin() + i);
							PlayerIndexKeeper.removeCombo(i);
							std::cout << "A player has left." << std::endl;
						}
						else {
							std::cout << "This player cant be deleted due to unknown unique id" << std::endl;
						}
					}
					else if (inner_type == PLAYER_DIED) {
						sf::Uint32 UniqueId;
						NewPacket >> UniqueId;

						if (unsigned int(UniqueId) != PlayerIndex) {

							int i = PlayerIndexKeeper.getIndex(unsigned int(UniqueId));
							if (i != INDEX_NOT_FOUND) {
								PlayersVector.erase(PlayersVector.begin() + i);
								PlayerIndexKeeper.removeCombo(i);
								std::cout << "Player died" << std::endl;
							}
						}
						else {
							std::cout << "respawn" << std::endl;
							RespawnAvailable = true;
						}
					}
					else if (inner_type == FOLLOW_BULLET_DATA) {
					
						sf::Int32 PlayerUniqueId;
						sf::Int32 BulletUniqueId;
						sf::Int64 BulletRadius;
						sf::Vector2f BulletPosition;

						NewPacket >> PlayerUniqueId;
						NewPacket >> BulletUniqueId;
						NewPacket >> BulletPosition.x;
						NewPacket >> BulletPosition.y;
						NewPacket >> BulletRadius;

						int PlayerI = PlayerIndexKeeper.getIndex(unsigned int(PlayerUniqueId));

						if (PlayerI != INDEX_NOT_FOUND) {
							int BulletI = PlayersVector[PlayerI].BulletIndexer.getIndex(unsigned int(BulletUniqueId));
							/// add keeper to player for bullets
							if (BulletI != INDEX_NOT_FOUND) {
								PlayersVector[PlayerI].updateBulletPosition(BulletI, BulletPosition, TimeStamp, DeltaTime);
							}
							else {
								///Create Bullet
								//std::cout << PlayersVector[PlayerI].getBulletLength() << std::endl;
								PlayersVector[PlayerI].BulletIndexer.addCombo(unsigned int(BulletUniqueId), PlayersVector[PlayerI].getBulletLength());
								PlayersVector[PlayerI].addBullet(bullet(unsigned int(BulletUniqueId), int(BulletRadius), BulletPosition));

							}

						}


					}
					else if (inner_type == DELETE_BULLET) {
						///delete bullet
						sf::Int32 PlayerUniqueId;
						sf::Int32 BulletUniqueId;

						NewPacket >> PlayerUniqueId;
						NewPacket >> BulletUniqueId;

						int PlayerI = PlayerIndexKeeper.getIndex(unsigned int(PlayerUniqueId));

						//std::cout << PlayerI << std::endl;

						if (PlayerI != INDEX_NOT_FOUND) {
							int BulletI = PlayersVector[PlayerI].BulletIndexer.getIndex(unsigned int(BulletUniqueId));
							//std::cout << BulletI << std::endl;
							/// add keeper to player for bullets
							if (BulletI != INDEX_NOT_FOUND) {
								PlayersVector[PlayerI].removeBulletAtIndex(BulletI);
								PlayersVector[PlayerI].BulletIndexer.removeCombo(BulletI);

			
							}
						}


					}
					else {
						break;
					}
				}
				else {
					break;
				}

			}

		}
		return true;
	}
	return false;
}

bool GameManager::Update(sf::RenderWindow &window)
{
	if (GameInterval <= 0.f) {
		//Do game Physiscs
		sf::Packet InputPacket;
		InputPacket << sf::Int64(sf::Mouse::getPosition(window).x);
		InputPacket << sf::Int64(sf::Mouse::getPosition(window).y);
		InputPacket << sf::Keyboard::isKeyPressed(sf::Keyboard::W);
		InputPacket << sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
		if (socket.send(InputPacket, SERVER_IP, CUSTOM_SERVER_PORT) != sf::Socket::Done) {
			std::cout << "Error sending packet " << std::endl;
		}
		GameInterval += .015f;
	}
	else {
		return false;
	}
	return true;
}

bool GameManager::draw(sf::RenderWindow & window)
{
	window.clear();

	const int l = int(StaticWorldItems.size());

	for (int i = 0; i < l; i++) {
		StaticWorldItems[i].draw(window);
	}

	for (int i = 0; i < int(PlayersVector.size()); i++) {
		PlayersVector[i].update(DeltaTime, window);
	}

	for (int i = 0; i < int(PlayersVector.size()); i++) {
		PlayersVector[i].draw(window);
	}

	drawFPS(window, tempFrameCount);

	window.display();

	s -= DeltaTime;
	frameCount++;
	if (s <= 0) {
		s = 1.f;
		tempFrameCount = frameCount;
		//std::cout << "Frames: " << frameCount << std::endl;
		frameCount = 0;
	}

	GameInterval -= DeltaTime;
	DeltaTime = GameLoopClock.restart().asSeconds();
	return true;
}

bool GameManager::drawMenu(sf::RenderWindow & window)
{
	window.clear();


	sf::Text txt;
	txt.setFont(loadingFont);
	txt.setString("Press Enter to play");
	txt.setCharacterSize(25);
	txt.setPosition({ 0.f , 0.f });
	window.draw(txt);
	window.display();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
			if (GameManagerObject.SendPlayerRequest()) {
				SetupCompleted = true;
			}
		}
	return true;
}

bool GameManager::displayText(sf::RenderWindow &window, sf::String text)
{
	if (loadingFontLoaded == false) {
		if (!loadingFont.loadFromFile("Source_Sans_Pro/SourceSansPro-Regular.ttf"))
		{
			std::cout << "Error loading font" << std::endl;
		}
		loadingFontLoaded = true;//will be set true even with error, dont want a loop
	}

	sf::Text txt;
	txt.setFont(loadingFont);
	txt.setString(text);
	txt.setCharacterSize(25);
	txt.setPosition({ 0.f , 0.f });

	window.clear();

	window.draw(txt);

	window.display();

	return false;
}

void GameManager::CheckForWindowClose(sf::RenderWindow & window)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
	}
}

void GameManager::drawFPS(sf::RenderWindow & window, const int frames)
{
	sf::Text txt;
	txt.setFont(loadingFont);
	txt.setString(std::to_string(frames) + " FPS");
	txt.setCharacterSize(25);
	txt.setPosition({ 900.f , 0.f });

	window.draw(txt);
}

void GameManager::Respawn()
{
	GameManagerObject = GameManager();
}

