#include "GameManager.h"



GameManager::GameManager()
{
}

bool GameManager::CheckForNewData(sf::UdpSocket &socket)
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
					else if (inner_type == FOLLOW_BULLET_DATA) {
						///insert bulet data

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

								///remove bullet
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
	}
	return true;
}

