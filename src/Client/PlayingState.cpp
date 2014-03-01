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

	mGameWorld.initialize(app.getTextures(), app.getWindow());
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
		mPlayer.handleEvent(app.getActions());
	}

	handlePackets(app);
}
void PlayingState::step(Application & app)
{
	app.getDesktop().Update(app.TimeStep.asSeconds());
	mGameWorld.step(app.TimeStep.asSeconds());
	mPlayer.sync(app.getSocket());
	mPlayer.update(app.getWindow());
}
void PlayingState::draw(Application & app)
{
	sf::RenderWindow & window = app.getWindow();
	window.clear();
	mGameWorld.draw(window);
	mPlayer.draw(window);
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
		onGameMapData(packet);
		break;
	case Sv::PlayersData:
		onPlayersData(packet);
	case Sv::GameEvent:
		mGameWorld.handlePacket(packet);	
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
		Wall * wall = mGameWorld.createEntity<Wall>(id, Entity::Type::Wall);
		wall->setPosition({ x, y });
		wall->setSize({ width, height });
		mGameWorld.addEntity(wall->getID());
	}
	
}

void PlayingState::onPlayersData(sf::Packet & packet)
{
	std::cout << "players data received!\n";
	Entity::Type myType;
	packet >> myType;

	if (myType == Entity::Type::Zombie)
	{		
		Entity::ID myID;
		packet >> myID;
		Zombie * me = mGameWorld.createEntity<Zombie>(myID, myType);
		mGameWorld.setPlayerEntity(me->getID());
		mPlayer.setEntity(me);
		
		sf::Int32 humanCount;
		packet >> humanCount;
		std::cout << "You are the zombie! human count: " << humanCount << std::endl;

		for (sf::Int32 i = 0; i < humanCount; ++i)
		{
			Entity::ID id;
			packet >> id;
			Human * h = mGameWorld.createEntity<Human>(id, Entity::Type::Human);
			mGameWorld.addEntity(h->getID());
		}
	}

	else if (myType == Entity::Type::Human)
	{
		Entity::ID zombieID, myID;
		packet >> zombieID >> myID;
		sf::Int32 playerCount;
		packet >> playerCount;
		std::cout << "Zombie ID: " << zombieID << "Other human count: " << playerCount<<std::endl;

		Zombie * z = mGameWorld.createEntity<Zombie>(zombieID, Entity::Type::Zombie);
		mGameWorld.addEntity(z->getID());
		Human * me = mGameWorld.createEntity<Human>(myID, Entity::Type::Human);
		mGameWorld.setPlayerEntity(me->getID());
		mPlayer.setEntity(me);

		std::cout << "Other human count: " << playerCount << std::endl;
		//create other players
		for (sf::Int32 i = 0; i < playerCount; ++i)
		{
			Entity::ID humanID;
			packet >> humanID;
			
			Human * h = mGameWorld.createEntity<Human>(humanID, Entity::Type::Human);
			mGameWorld.addEntity(h->getID());
		}
	}
	else
		assert(false && "something got fucked up");
}