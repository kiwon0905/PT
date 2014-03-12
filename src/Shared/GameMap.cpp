#include "Shared/GameMap.h"
#include "Shared/ValueParser.h"

GameMap::GameMap()
{
}


GameMap::~GameMap()
{
}

bool GameMap::loadFromFile(const std::string & s)
{
	ValueParser parser;

	if (!parser.loadFromFile(s))
		return false;
	parser.printAll();
	mMapName = s;
	// get size
	if (!parser.get("SizeX", mSize.x))
		return false;

	if (!parser.get("SizeY", mSize.y))
		return false;

	//Load wall data
	int wallCount = 0;
	if (!parser.get("WallCount", wallCount))
		return false;

	for (int i = 0; i < wallCount; ++i)
	{
		float x, y, width, height;
		if (!parser.get("Wall" + std::to_string(i) + "X", x))
			return false;

		if (!parser.get("Wall" + std::to_string(i) + "Y", y))
			return false;

		if (!parser.get("Wall" + std::to_string(i) + "Width", width))
			return false;

		if (!parser.get("Wall" + std::to_string(i) + "Height", height))
			return false;

		mWalls.push_back(sf::FloatRect(x, y, width, height));
	}
	mMapName = s;
	return true;
}

const std::vector<sf::FloatRect> & GameMap::getWalls()
{
	return mWalls;
}
const sf::Vector2f & GameMap::getSize()
{
	return mSize;
}
const std::string & GameMap::getName()
{
	return mMapName;
}
sf::FloatRect GameMap::getBound()
{
	return sf::FloatRect(0, 0, mSize.x, mSize.y);
}