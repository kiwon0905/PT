#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFML/Network.hpp>

#include <Thor/Input.hpp>
#include "Client/ResourceManager.h"
#include "Client/GameStateStack.h"
#include "Client/Drawer.h"
#include "Client/AudioPlayer.h"
#include "Client/Player.h"

class Application
{
public:
	const sf::Time TimeStep;


	Application();
	~Application();

	void quit();
	void push(GameState * newState);
	void pop();
	void run();

	sf::RenderWindow & getWindow();
	sfg::SFGUI & getGUI();
	sfg::Desktop & getDesktop();
	sf::TcpSocket & getSocket();
	
	AudioPlayer & getAudioPlayer();
	Textures & getTextures();
	thor::ActionMap<Player::Action> & getActions();

	const std::string & getPlayerName() const;
	void setPlayerName(const std::string & s);
private:
	GameStateStack mStates;

	sf::RenderWindow mWindow;
	sfg::SFGUI mGUI;
	sfg::Desktop mDesktop;
	sf::TcpSocket mSocket;
	
	AudioPlayer mAudioPlayer;
	Textures mTextures;
	SoundBuffers mSoundBuffers;
	thor::ActionMap<Player::Action> mActions;

	std::string mPlayerName;
};

