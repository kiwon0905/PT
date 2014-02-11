#include "Client/LobbyState.h"
#include "Client/Application.h"
#include "Thor/Vectors.hpp"
#include <SFML/Graphics.hpp>


LobbyState::LobbyState() :e(1)
{
	e.setSize({ 50.f, 50.f });
	e.setAcceleration(4000.f);
	
}


LobbyState::~LobbyState()
{
}

void LobbyState::onEnter(Application & app)
{

}
void LobbyState::handleEvent(Application & app)
{
	sf::RenderWindow & window = app.getWindow();
	sf::Event event;
	auto & actions = app.getActions();
	actions.clearEvents();

	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			app.quit();
		actions.pushEvent(event);
	}
}
void LobbyState::step(Application & app)
{
	const float speed = 500.f;
	auto & actions = app.getActions();
	
	if (actions.isActive(Player::MoveNE))
		e.setGoalVelocity(thor::PolarVector2f(speed, -45));
	else if (actions.isActive(Player::MoveNW))
		e.setGoalVelocity(thor::PolarVector2f(speed, -135));
	else if (actions.isActive(Player::MoveSW))
		e.setGoalVelocity(thor::PolarVector2f(speed, -225));
	else if (actions.isActive(Player::MoveSE))
		e.setGoalVelocity(thor::PolarVector2f(speed, -315));

	else if (actions.isActive(Player::MoveE))
		e.setGoalVelocity(thor::PolarVector2f(speed, 0));
	else if (actions.isActive(Player::MoveN))
		e.setGoalVelocity(thor::PolarVector2f(-speed, 90));
	else if (actions.isActive(Player::MoveW))
		e.setGoalVelocity(thor::PolarVector2f(speed, 180));
	else if (actions.isActive(Player::MoveS))
		e.setGoalVelocity(thor::PolarVector2f(-speed, 270));
	
	else
		e.setGoalVelocity({ 0, 0 });

	sf::Vector2i mousepos = sf::Mouse::getPosition(app.getWindow());
	float dx = mousepos.x - e.getPosition().x;
	float dy = mousepos.y - e.getPosition().y;
	e.setRotation(thor::toDegree(std::atan2f(dy, dx)));

	e.update(app.TimeStep.asSeconds());

}
void LobbyState::draw(Application & app)
{
	sf::RenderWindow & window = app.getWindow();
	window.clear();

	sf::RectangleShape r;
	r.setRotation(e.getRotation());

	sf::RectangleShape bound;
	bound.setOrigin(e.getSize() / 2.f);
	bound.setPosition(e.getPosition());
	bound.setSize(e.getSize());
	bound.setOutlineColor(sf::Color::Green);
	bound.setFillColor(sf::Color::Transparent);
	bound.setOutlineThickness(5.f);

	r.setSize({ 50.f, 50.f });
	r.setOrigin(e.getSize() / 2.f);
	r.setPosition(e.getPosition());
	window.draw(bound);
	window.draw(r);
	window.display();
}
void LobbyState::onExit(Application & app)
{

}
void LobbyState::onObscure(Application & app)
{

}
void LobbyState::onReveal(Application & app)
{

}
