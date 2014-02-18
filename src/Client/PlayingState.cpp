#include "Client/PlayingState.h"
#include "Client/Application.h"
#include "Shared/NetProtocol.h"

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
	while (app.getWindow().pollEvent(ev))
	{
		if (ev.type == sf::Event::Closed)
			app.quit();
		app.getDesktop().HandleEvent(ev);
	}
	handlePackets(app);
}
void PlayingState::step(Application & app)
{
	app.getDesktop().Update(app.TimeStep.asSeconds());

}
void PlayingState::draw(Application & app)
{
	sf::RenderWindow & window = app.getWindow();
	window.clear();
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
	{
		std::cout << "map data received";
		std::string mapName;
		packet >> s;
		mGameWorld.loadFromFile(mapName);


	}

		break;
	case Sv::GameEvent:
		break;
	default:
		break;
	}
}