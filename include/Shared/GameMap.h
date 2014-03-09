#pragma once

#include <SFML/Graphics.hpp>
class GameMap
{
public:
	GameMap();
	~GameMap();

	bool loadFromFile(const std::string & s);

	const std::vector<sf::FloatRect> & getWalls();
	const sf::Vector2f & getSize();
	const std::string & getName();
private:
	std::string mMapName;
	sf::Vector2f mSize;
	std::vector<sf::FloatRect> mWalls;

};

