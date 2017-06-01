#include "StaticWorldItem.h"



StaticWorldItem::StaticWorldItem()
{
}

StaticWorldItem::StaticWorldItem(sf::Vector2f Position, int width, int height)
	:Position(Position), width(width), height(height), ItemSprite()
{
	ItemSprite.setPosition(Position);
}

void StaticWorldItem::setTextureFile(sf::String f)
{
	TextureFileName = f;
}

void StaticWorldItem::setWidth(int w)
{
	width = w;
}

void StaticWorldItem::setHeight(int h)
{
	height = h;
}

void StaticWorldItem::LoadTexture()
{
	if (!ItemTexture.loadFromFile(TextureFileName))
		std::cout << "Error loading Item Sprite: " << TextureFileName.toAnsiString() << std::endl;
	ItemSprite.setTexture(ItemTexture);
}

void StaticWorldItem::draw(sf::RenderWindow & window)
{
	window.draw(ItemSprite);
}
