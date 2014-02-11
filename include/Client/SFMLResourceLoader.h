#pragma once
#include <string>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>

template <class R>
struct Loader
{
	R * operator()(const std::string & s);
};

template <class R>
R * Loader<R>::operator()(const std::string & s)
{
	R * resource = new R;
	if (!resource->loadFromFile(s))
	{
		delete resource;
		return nullptr;
	}
	return resource;

}

typedef Loader<sf::Font> FontLoader;
typedef Loader<sf::Texture> TextureLoader;
typedef Loader<sf::SoundBuffer> SoundBufferLoader;