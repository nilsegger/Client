#include "PositionPacket.h"

PositionPacket::PositionPacket()
{
}

PositionPacket::PositionPacket(float t, sf::Vector2f TargetedPosition, sf::Int32 TimeStamp)
	:t(t),
	tOrigin(t),
	TargetedPosition(TargetedPosition),
	TimeStamp(TimeStamp)
{
}

PositionPacket::PositionPacket(float t, sf::Vector2f TargetedPosition, sf::Int32 TimeStamp, float DeltaTime)
	:t(t),
	tOrigin(t),
	TargetedPosition(TargetedPosition),
	TimeStamp(TimeStamp),
	DeltaTime(DeltaTime)
{
}

float PositionPacket::getMoveVector2f(sf::Vector2f &CurrentPosition, const float nDeltaTime, int index, int radius)
{
	//This function is assuming it is called every physics update
	t -= nDeltaTime;
	float temp = t;
	if (t <= 0) {
		t = 0;
		CurrentPosition = TargetedPosition;
	}
	else {
		//So... The Position must be at Targeted Position once t goes to 0

		if (index == 0) {
			sf::Vector2f center = { CurrentPosition.x + float(radius), CurrentPosition.y + float(radius) };

			float dx = TargetedPosition.x + radius - center.x;
			float dy = TargetedPosition.y + radius - center.y;
			float distance = sqrt(fabs(dx) * fabs(dx) + fabs(dy) * fabs(dy));
			if (distance > 0) {
				sf::Vector2f forward = { (dx) / distance , (dy) / distance };
				CurrentPosition += forward * (distance / (t / nDeltaTime));
			}
		}
	}
	return temp;
}


sf::Int32 PositionPacket::getTimeStamp() const
{
	return TimeStamp;
}

sf::Vector2f PositionPacket::getTargetedPosition() const
{
	return TargetedPosition;
}

void PositionPacket::subT(float amount)
{
	t -= fabs(amount);
}

float PositionPacket::getT() const
{
	return t;
}

float PositionPacket::getDeltaTime() const
{
	return DeltaTime;
}

