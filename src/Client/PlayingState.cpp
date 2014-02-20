#include "Client/PlayingState.h"
#include "Client/Application.h"
#include "Shared/NetProtocol.h"
#include "Shared/Wall.h"
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

	mGameWorld.initialize(app.getTextures(), app.getWindow());
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
	mGameWorld.step(app.TimeStep.asSeconds());

}
void PlayingState::draw(Application & app)
{
	sf::RenderWindow & window = app.getWindow();
	window.clear();
	mGameWorld.draw();
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
		onGameMapData(packet);
	}

		break;
	case Sv::GameEvent:
		break;
	default:
		break;
	}
}

void PlayingState::onGameMapData(sf::Packet & packet)
{
	std::string mapName;
	packet >> mapName;
	mGameWorld.loadFromFile(mapName);

	std::cout << "map data received: " << mapName << "\n";

	sf::Int32 wallCount;
	packet >> wallCount;
	std::cout << "wall count: " << wallCount << "\n";

	for (int i = 0; i < wallCount; ++i)
	{
		Entity::ID id;
		float x, y, width, height;
		packet >> id >> x >> y >> width >> height;
		Wall * wall = static_cast<Wall*>(mGameWorld.createEntity(id, Entity::Type::Wall));
		wall->setPosition({ x, y });
		wall->setSize({ width, height });
		mGameWorld.addEntity(wall->getID());
	}
	
}