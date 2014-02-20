#include "Client/Application.h"


Application::Application() :TimeStep(sf::seconds(1 / 60.f)), 
mWindow(), mSoundBuffers(), mTextures(),
mAudioPlayer(mSoundBuffers)
{
	mWindow.resetGLStates();
	mSocket.setBlocking(false);
}


Application::~Application()
{
}

sf::RenderWindow & Application::getWindow()
{
	return mWindow;
}
sfg::SFGUI & Application::getGUI()
{
	return mGUI;
}
sfg::Desktop & Application::getDesktop()
{
	return mDesktop;
}
sf::TcpSocket & Application::getSocket()
{
	return mSocket;
}
AudioPlayer & Application::getAudioPlayer()
{
	return mAudioPlayer;
}
Textures & Application::getTextures()
{
	return mTextures;
}
thor::ActionMap<Player::Action> & Application::getActions()
{
	return mActions;
}

void Application::push(GameState * newState)
{
	mStates.push(newState);
}

void Application::pop()
{
	mStates.pop();
}

void Application::quit()
{
	mStates.clear();
}

void Application::run()
{
	mStates.applyChanges(*this);

	sf::Clock clock;
	sf::Time elapsed = sf::Time::Zero;

	while (!mStates.isEmpty())
	{
		mStates.draw(*this);
		elapsed += clock.restart();
		while (elapsed >= TimeStep && !mStates.isEmpty())
		{
			elapsed -= TimeStep;
			mStates.handleEvent(*this);
			mStates.applyChanges(*this);
			
			if (!mStates.isEmpty())
				mStates.step(*this);
			mStates.applyChanges(*this);
		}
	}
}

const std::string & Application::getPlayerName() const
{
	return mPlayerName;
}
void Application::setPlayerName(const std::string & s)
{
	mPlayerName = s;
}