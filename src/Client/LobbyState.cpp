#include "Client/LobbyState.h"
#include "Client/Application.h"
#include "Shared/NetProtocol.h"
#include "Client/PlayingState.h"

#include <SFML/Graphics.hpp>



LobbyState::LobbyState() : mChatBox({ 400, 200 }, { 400, 30 })
{	
}


LobbyState::~LobbyState()
{
}


void LobbyState::loadGUI(Application & app)
{
	sfg::Desktop & desktop = app.getDesktop();
	
	sfg::Button::Ptr readyButton = sfg::Button::Create("Ready");
	readyButton->SetId("readyButton");
	readyButton->SetRequisition({ 100.f, 50.f });
	readyButton->SetPosition({ 500.f, 500.f });
	
	auto onReady = [&app]()
	{
		sf::Packet packet;
		packet << Cl::Ready;
		app.getSocket().send(packet);
	};
	readyButton->GetSignal(sfg::Button::OnLeftClick).Connect(onReady);
	
	desktop.Add(readyButton);
	mChatBox.add(desktop);
}
void LobbyState::clearGUI(Application & app)
{
	app.getDesktop().RemoveAll();
	mChatBox.remove(app.getDesktop());
}
void LobbyState::onEnter(Application & app)
{
	loadGUI(app);
	mChatBox.setSocket(app.getSocket());
}
void LobbyState::handleEvent(Application & app)
{
	sf::RenderWindow & window = app.getWindow();
	sf::Event event;


	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			app.quit();

		app.getDesktop().HandleEvent(event);
	}
	handlePackets(app);
}

void LobbyState::handlePackets(Application & app)
{
	sf::Packet packet;
	sf::TcpSocket & socket = app.getSocket();

	sf::Socket::Status s = socket.receive(packet);
	while (s == sf::Socket::Done)
	{
		handlePacket(app, packet);
		packet.clear();
		s = socket.receive(packet);
	}
}

void LobbyState::handlePacket(Application & app, sf::Packet & packet)
{
	Sv type;
	packet >> type;
	switch (type)
	{
	case Sv::PlayerJoined:
		onPlayerJoin(app, packet);
		break;
	case Sv::PlayerDisconnected:
		onPlayerDisconnect(app, packet);
		break;
	case Sv::Chat:
		onChat(app, packet);
		break;
	case Sv::GameStarted:
		app.push(new PlayingState);
		break;
	default:
		break;
	}
}

void LobbyState::onPlayerJoin(Application & app, sf::Packet & packet)
{
	sf::Int32 num;
	packet >> num;
	for (sf::Int32 i = 0; i < num; ++i)
	{
		std::string name;
		packet >> name;
		if (name == app.getPlayerName())
			name = "You";
		mChatBox.push(name + " joined the room.");
	}
}

void LobbyState::onPlayerDisconnect(Application & app, sf::Packet & packet)
{
	std::string name;
	packet >> name;
	mChatBox.push(name + " left the room.");
}

void LobbyState::onChat(Application & app, sf::Packet & packet)
{
	std::string peer, msg;
	packet >> peer >> msg;
	if (peer == app.getPlayerName())
		peer = "You";
	mChatBox.push(peer + ": " + msg);
}

void LobbyState::step(Application & app)
{
	app.getDesktop().Update(app.TimeStep.asSeconds());

}
void LobbyState::draw(Application & app)
{
	sf::RenderWindow & window = app.getWindow();


	window.clear();
	app.getGUI().Display(window);
	window.display();
}
void LobbyState::onExit(Application & app)
{
	clearGUI(app);
	app.getSocket().disconnect();
}
void LobbyState::onObscure(Application & app)
{
	clearGUI(app);
}
void LobbyState::onReveal(Application & app)
{
	loadGUI(app);
}
