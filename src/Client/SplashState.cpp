#include "Client/SplashState.h"
#include "Client/Application.h"
#include "Client/LobbyState.h"
#include "Shared/NetProtocol.h"
#include "Client/Icon.h"
#include <cassert>

SplashState::SplashState() :GameState()
{

}
SplashState::~SplashState()
{

}

void SplashState::connect(Application & app)
{
	std::string name = std::static_pointer_cast<sfg::Entry>(sfg::Widget::GetWidgetById("nameEntry"))->GetText();
	if (name == "")
		return;

	std::string ip = std::static_pointer_cast<sfg::Entry>(sfg::Widget::GetWidgetById("ipEntry"))->GetText().toAnsiString();
	std::string sport = std::static_pointer_cast<sfg::Entry>(sfg::Widget::GetWidgetById("portEntry"))->GetText().toAnsiString();
	unsigned short uport; 

	try
	{
		uport = std::stoi(sport);
	}
	catch (...)
	{
		return;
	}

	app.getSocket().setBlocking(true);
	sf::Socket::Status s= app.getSocket().connect(ip, uport, sf::seconds(3.f));
	app.getSocket().setBlocking(false);
	if (s == sf::Socket::Done)
	{
		sf::Packet packet;
		packet << Cl::RequestJoin << name;
		app.getSocket().send(packet);
	}
}
void SplashState::loadGUI(Application & app)
{
	sfg::Desktop & desktop = app.getDesktop();
	sfg::Window::Ptr window = sfg::Window::Create(sfg::Window::Style::NO_STYLE);
		
	sfg::Box::Ptr box1 = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);
	sfg::Box::Ptr box2 = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);
	sfg::Box::Ptr box3 = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);
	sfg::Box::Ptr box4 = sfg::Box::Create(sfg::Box::Orientation::VERTICAL,10.f);
	
	sfg::Label::Ptr ipLabel = sfg::Label::Create("IP     ");
	sfg::Entry::Ptr ipEntry = sfg::Entry::Create("127.0.0.1"); ipEntry->SetId("ipEntry");
	box2->Pack(ipLabel, false, false);
	box2->Pack(ipEntry, true, true);

	sfg::Label::Ptr portLabel = sfg::Label::Create("Port  ");
	sfg::Entry::Ptr portEntry = sfg::Entry::Create(); portEntry->SetId("portEntry");
	box3->Pack(portLabel, false, false);
	box3->Pack(portEntry, true, true);

	sfg::Label::Ptr nameLabel = sfg::Label::Create("Name  ");
	sfg::Entry::Ptr nameEntry = sfg::Entry::Create(); nameEntry->SetId("nameEntry");
	box4->Pack(nameLabel, false, false);
	box4->Pack(nameEntry, true, true);

	box1->Pack(box2, false, false);
	box1->Pack(box3, false, false);
	box1->Pack(box4, false, false);


	sfg::Button::Ptr playButton = sfg::Button::Create("Launch");
	playButton->SetPosition(sf::Vector2f(400, 400));
	
	auto onConnect = [this, &app]()
	{
		connect(app);
	};
	playButton->GetSignal(sfg::Button::OnLeftClick).Connect(onConnect);

	
	sfg::Button::Ptr exitButton = sfg::Button::Create("exit");
	exitButton->SetPosition({ 600.f, 400.f });
	auto onExit = [this, &app]()
	{
		app.quit();
	};
	exitButton->GetSignal(sfg::Button::OnLeftClick).Connect(onExit);
	
	
	window->Add(box1);
	desktop.Add(playButton);
	desktop.Add(window);
	desktop.Add(exitButton);

	window->SetAllocation(sf::FloatRect(100, 350, 300, 200));


}


void SplashState::initActions(thor::ActionMap<Player::Action> & actions)
{
	std::bind(&sf::Keyboard::isKeyPressed, sf::Keyboard::D);

	actions[Player::MoveE] = thor::realtimeAction(std::bind(&sf::Keyboard::isKeyPressed, sf::Keyboard::D));
	actions[Player::MoveN] = thor::realtimeAction(std::bind(&sf::Keyboard::isKeyPressed, sf::Keyboard::W));
	actions[Player::MoveW] = thor::realtimeAction(std::bind(&sf::Keyboard::isKeyPressed, sf::Keyboard::A));
	actions[Player::MoveS] = thor::realtimeAction(std::bind(&sf::Keyboard::isKeyPressed, sf::Keyboard::S));
	
	actions[Player::MoveNE] = actions[Player::MoveN] && actions[Player::MoveE];
	actions[Player::MoveNW] = actions[Player::MoveN] && actions[Player::MoveW];
	actions[Player::MoveSW] = actions[Player::MoveS] && actions[Player::MoveW];
	actions[Player::MoveSE] = actions[Player::MoveS] && actions[Player::MoveE];

}

void SplashState::clearGUI(Application & app)
{
	sfg::Desktop & desktop = app.getDesktop();
	desktop.RemoveAll();
}

void SplashState::onEnter(Application & app)
{
	sf::RenderWindow & window = app.getWindow();

	window.create(sf::VideoMode(800, 600), "", sf::Style::Default, sf::ContextSettings(0, 0, 8, 2, 0));

	/*HWND hwnd = window.getSystemHandle();
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 200, LWA_ALPHA);*/

	app.getTextures().get("assets/Splash.png");

	loadGUI(app);
	initActions(app.getActions());
	window.setIcon(icon.width, icon.height, icon.pixel_data);

}
void SplashState::handleEvent(Application & app)
{
	sf::Event event;
	sf::RenderWindow & window = app.getWindow();
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			app.quit();
		app.getDesktop().HandleEvent(event);
	}

}
void SplashState::step(Application & app)
{

	sfg::Desktop & desktop = app.getDesktop();
	AudioPlayer & audioPlayer = app.getAudioPlayer();
	Drawer & drawer = app.getDrawer();

	sf::TcpSocket & socket = app.getSocket();

	sf::Packet packet;
	if (socket.receive(packet) == sf::Socket::Done)
	{
		Sv response, answer;
		packet >> response >> answer;
		
		assert(response == Sv::ReplyJoin);

		if (answer == Sv::Yes)
		{
			std::cout << "connected\n";
			app.setPlayerName(std::static_pointer_cast<sfg::Entry>(sfg::Widget::GetWidgetById("nameEntry"))->GetText());
			app.push(new LobbyState);
		}
	}

	drawer.update(app.TimeStep.asSeconds());
	audioPlayer.update();
	desktop.Update(app.TimeStep.asSeconds());
}
void SplashState::draw(Application & app)
{
	sf::RenderWindow & window = app.getWindow();
	sfg::SFGUI & gui = app.getGUI();

	window.clear(sf::Color::White);

	
	sf::Sprite sprite;
	sprite.setTexture(*app.getTextures().get("assets/Splash.png"));
	window.draw(sprite);
	gui.Display(window);
	window.display();
}
void SplashState::onExit(Application & app)
{
	clearGUI(app);
}
void SplashState::onObscure(Application & app)
{
	clearGUI(app);
}
void SplashState::onReveal(Application & app)
{
	loadGUI(app);
}