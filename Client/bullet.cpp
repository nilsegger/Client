#include "bullet.h"



bullet::bullet()
{
}

bullet::bullet(unsigned int UniqueId, int radius, sf::Vector2f nPosition)
	:shape(float(radius)),
	Position(Position),
	UniqueId(UniqueId)
{
}

void bullet::setPos(sf::Vector2f pos)
{
	Position = pos;
}

void bullet::draw(sf::RenderWindow & window)
{
	if (!died) {
		window.draw(shape);
	}
}

void bullet::UpdatePosition(sf::Vector2f pos, sf::Int32 TimeStamp, float DeltaTime)
{
	if (!died) {
		PositionPacket NewPosition(.1f, pos, TimeStamp, DeltaTime);
		NextPositions.push_back(NewPosition);
	}
}

void bullet::update(float DeltaTime)
{
	/// not really sure if it works properly, it doesnt
	if (!died) {



		for (int i = int(NextPositions.size()) - 1; i >= 0; i--) {
			float dx = NextPositions[i].getTargetedPosition().x - Position.x;
			float dy = NextPositions[i].getTargetedPosition().y - Position.y;
			float distance = sqrt(fabs(dx) * fabs(dx) + fabs(dy) * fabs(dy));
			if (distance > 0) {
				sf::Vector2f forward = { (dx) / distance , (dy) / distance };
				Position += forward * (distance / (NextPositions[i].getT() / NextPositions[i].getDeltaTime()));
				//Position += forward * (distance / (NextPositions[i].getT() / DeltaTime));
			}


			NextPositions[i].subT(DeltaTime);
			
			/**/

			

			/**/

		}
		for (int i = 0; i < int(NextPositions.size()); i++) {
			float t = NextPositions[i].getT();
			if (t <= 0.f) {
				Position = NextPositions[i].getTargetedPosition();
				NextPositions.erase(NextPositions.begin() + i);
			}
		}



		

		shape.setPosition(Position);
	}
}

void bullet::setDeleteOnNextInterval()
{
	DeleteOnNextInterval = true;
}

bool bullet::getDeleteOnNextInterval() const
{
	return DeleteOnNextInterval;
}

bool bullet::getDied() const
{
	return died;
}

void bullet::setDied()
{
	died = true;
}

sf::Vector2f bullet::getPositionCentered() const
{
	return {Position.x + radius, Position.y + radius};
}

unsigned int bullet::getUniqueId() const
{
	return UniqueId;
}

