#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>

class StaticWorldItem
{
private:
	sf::Sprite ItemSprite;
	sf::Texture ItemTexture;

	bool TextureLoaded;

	sf::Vector2f Position;
	int width;
	int height;
	sf::String TextureFileName;
public:
	StaticWorldItem();
	StaticWorldItem(sf::Vector2f Position, int width, int height);
	void setTextureFile(sf::String f);
	void setWidth(int w);
	void setHeight(int h);
	void LoadTexture();
	void draw(sf::RenderWindow &window);
};

