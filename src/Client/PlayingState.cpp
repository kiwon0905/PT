#include "Client/PlayingState.h"
#include "Client/Application.h"


PlayingState::PlayingState(const std::string & mapName)
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

}

void PlayingState::handlePacket(Application & app, sf::Packet & packet)
{

}