#include "Client/PlayingState.h"
#include "Client/Application.h"
#include "Shared/NetProtocol.h"
#include "Shared/Wall.h"
#include "Shared/Zombie.h"
#include "Shared/Human.h"
PlayingState::PlayingState()
{
}


PlayingState::~PlayingState()
{
}
void PlayingState::loadGUI(Application & app)
{

}
void PlayingState::clearGUI(Application & app)
{
	
}
void PlayingState::onEnter(Application & app)
{
	loadGUI(app);
	sf::Packet packet;
	packet << Cl::Ready; // ready to receive mapdata
	app.getSocket().send(packet);
}
void PlayingState::handleEvent(Application & app)
{
	sf::Event ev;
	app.getActions().clearEvents();
	while (app.getWindow().pollEvent(ev))
	{
		if (ev.type == sf::Event::Closed)
			app.quit();
		app.getDesktop().HandleEvent(ev);
		app.getActions().pushEvent(ev);
		mGame.handleEvent(app.getActions());
	}

	handlePackets(app);
}
void PlayingState::step(Application & app)
{
	app.getDesktop().Update(app.TimeStep.asSeconds());
	
	mGame.step(app.getWindow(), app.getSocket(), app.TimeStep.asSeconds());
}
void PlayingState::draw(Application & app)
{
	sf::RenderWindow & window = app.getWindow();
	window.clear();
	mGame.draw(app.getTextures(), window);
	window.display();
}
void PlayingState::onExit(Application & app)
{
	clearGUI(app);
}
void PlayingState::onObscure(Application & app)
{
	clearGUI(app);
}
void PlayingState::onReveal(Application & app)
{
	loadGUI(app);
}
void PlayingState::handlePackets(Application & app)
{
	sf::TcpSocket & socket = app.getSocket();

	sf::Packet packet;
	sf::Socket::Status s = socket.receive(packet);

	while (s == sf::Socket::Done)
	{
		handlePacket(app, packet);
		packet.clear();
		s=socket.receive(packet);
	}
}

void PlayingState::handlePacket(Application & app, sf::Packet & packet)
{
	Sv s;
	packet >> s;
	
	switch (s)
	{
	case Sv::GameMapData:
		mGame.onGameMapData(packet);
		break;
	case Sv::PlayersData:
		mGame.onPlayersData(packet);
		break;
	case Sv::GameEvent:
		mGame.handlePacket(packet);
		break;
	case Sv::StartGame:
		mGame.start();
		break;
	default:
		break;
	}
}