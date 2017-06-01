#include "player.h"

player::player()
{
}

player::player(int radius, sf::Vector2f nPosition, int index, bool isPlayer)

	:shape(float(radius)),
	radius(radius),
	Position(nPosition),
	index(index),
	isPlayerControlled(isPlayer),
	PlayerTexture(),
	PlayerSprite()
{
}
/*
*/
void player::draw(sf::RenderWindow & window)
{
	if (SpriteLoaded == false) {
		if (!PlayerTexture.loadFromFile("ufo.png")) {
			std::cout << "Error loading ufo file" << std::endl;
		}
		PlayerTexture.setSmooth(true);
		PlayerSprite.setTexture(PlayerTexture);
		SpriteLoaded = true;
	}

	//window.draw(shape);
	PlayerSprite.setScale(float(radius)*2.f / 512.f, float(radius)*2.f / 512.f);
	PlayerSprite.setPosition(Position);

	for (int i = 0; i < int(PlayerBullets.size()); i++) {
		PlayerBullets[i].draw(window);
	}

	if (isPlayerControlled) {
		float r = float(radius);
		sf::CircleShape ServerShadow(r);
		ServerShadow.setPosition(ServerPosition);
		window.draw(ServerShadow);
	}

	window.draw(PlayerSprite);
}

void player::UpdatePosition(sf::Vector2f pos, sf::Int32 TimeStamp)
{
	if (isPlayerControlled) {
		ServerPosition = pos;
	}
	else {
		PositionPacket NewPosition(.1f, pos, TimeStamp);
		NextPositions.push_back(NewPosition);
	}
}

void player::update(float DeltaTime, sf::RenderWindow &window)
{
	if (!isPlayerControlled) {
		// Non controllable
		sf::Int32 LastTimeStamp = 0;
		int deleted = 0;
		for (int i = 0; i < int(NextPositions.size()); i++) {
			if (NextPositions[i].getTimeStamp() < LastTimeStamp) {
				NextPositions.erase(NextPositions.begin() + i);
				deleted++;
				//std::cout << "Jumped over this one" << std::endl;
				//Package arrived after its children
				continue;
			}
			float t = NextPositions[i].getMoveVector2f(Position, DeltaTime, i - deleted, radius);

			if (t <= 0) {
				LastTimeStamp = NextPositions[i].getTimeStamp();
				NextPositions.erase(NextPositions.begin() + i);
				deleted++;
			}
		}
	}
	else {
		//This is the player object

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			sf::Vector2f center = { Position.x + float(radius), Position.y + float(radius) };
			sf::Vector2f MousePosition = { float(sf::Mouse::getPosition(window).x), float(sf::Mouse::getPosition(window).y) };
			float dx = MousePosition.x - center.x;
			float dy = MousePosition.y - center.y;
			float distance = sqrt(fabs(dx) * fabs(dx) + fabs(dy) * fabs(dy));
			sf::Vector2f forward = { (dx) / distance , (dy) / distance };
			Position += forward * (speed)* DeltaTime;
		}

		LastPositions.push_back(Position);

		TimeSinceLastMatch += DeltaTime;


		const int l = LastPositions.size();

		for (int i = 0; i < l; i++) {
			if (LastPositions[i] == ServerPosition) {
				// A Position match
				ServerClientPositionMatch(i);
				break;
			}
			else {
				float dx = LastPositions[i].x - ServerPosition.x;
				float dy = LastPositions[i].y - ServerPosition.y;
				float d = fabs(dx) * fabs(dx) + fabs(dy) * fabs(dy);

				if (d <= MisMatchDistanceAllowed * MisMatchDistanceAllowed) {
					ServerClientPositionMatch(i);
					break;
				}
			}
		}


		if (TimeSinceLastMatch > 3.f)//if server and client positions havent matched for three seconds, apply server pos
		{
			std::cout << "No match since 3 seconds" << std::endl;
			Position = ServerPosition;
			TimeSinceLastMatch = 0.f;
		}


	}

	

	for (int i = 0; i < int(PlayerBullets.size()); i++) {
 		PlayerBullets[i].update(DeltaTime);
	}
	shape.setPosition(Position);
	//std::cout << Position.x
}

void player::ServerClientPositionMatch(const int i) {
	for (int q = i; q >= 0; q--) {
		LastPositions.erase(LastPositions.begin() + q);
	}

	TimeSinceLastMatch = 0.f;
}

void player::setPlayer(bool val)
{
	isPlayerControlled = true;
}

bool player::getIsPlayerControlled() const
{
	return isPlayerControlled;
}

int player::getBulletLength() const
{
	return int(PlayerBullets.size());
}

void player::addBullet(bullet &newBullet)
{
	PlayerBullets.push_back(newBullet);
}

void player::updateBulletPosition(int index, sf::Vector2f pos, sf::Int32 TimeStamp, float DeltaTime)
{
	PlayerBullets[index].UpdatePosition(pos, TimeStamp, DeltaTime);
	 
}

void player::removeBulletAtIndex(int i)
{
	PlayerBullets.erase(PlayerBullets.begin() + i);
}

