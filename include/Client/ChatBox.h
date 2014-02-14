#pragma once

#include <deque>
#include <string>
#include <SFGUI/SFGUI.hpp>

class Application;
namespace sf
{
	class TcpSocket;
}

class ChatBox
{
public:
	ChatBox(const sf::Vector2f & windowSize, const sf::Vector2f & entrySize);
	~ChatBox();
	
	void setVisible(bool visible);
	void setPosition(const sf::Vector2f & v);
	void setBufferLimit(std::size_t i);
	void push(const std::string & s);

	void add(sfg::Desktop & desktop);
	void remove(sfg::Desktop & desktop);

	void setSocket(sf::TcpSocket & socket);
private:
	sf::TcpSocket * mSocket;

	std::size_t mBufferLimit;
	std::deque<std::string> mBuffer;

	sfg::Window::Ptr mWindow;
	sfg::ScrolledWindow::Ptr mScrolledWindow;
	sfg::Entry::Ptr mEntry;
	sfg::Label::Ptr mLabel;
	sfg::Button::Ptr mButton;
};

