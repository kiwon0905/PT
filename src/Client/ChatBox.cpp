#include "Client/ChatBox.h"
#include "Shared/NetProtocol.h"
#include <SFML/Network.hpp>


ChatBox::ChatBox(const sf::Vector2f & windowSize, const sf::Vector2f & entrySize) : mBufferLimit(20)
{
	mWindow = sfg::Window::Create();
	mLabel = sfg::Label::Create();
	mButton = sfg::Button::Create("Send");
	mScrolledWindow = sfg::ScrolledWindow::Create();
	mScrolledWindow->SetScrollbarPolicy
		(sfg::ScrolledWindow::HORIZONTAL_NEVER | sfg::ScrolledWindow::VERTICAL_ALWAYS);
	mEntry = sfg::Entry::Create();
	
	

	mScrolledWindow->AddWithViewport(mLabel);
	
	
	sfg::Box::Ptr box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	
	sfg::Box::Ptr box2 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);

	box->Pack(mScrolledWindow);

	box->Pack(box2);
	box2->Pack(mEntry);
	box2->Pack(mButton);

	auto onSend = [this]()
	{
		//push(mEntry->GetText());
		sf::Packet packet;
		packet << Cl::Chat << mEntry->GetText().toAnsiString();
		mSocket->send(packet);
		mEntry->SetText("");
	};
	mButton->GetSignal(sfg::Button::OnLeftClick).Connect(onSend);
	mEntry->SetRequisition(entrySize);
	

	mScrolledWindow->SetRequisition(windowSize);

	mWindow->Add(box);
}


ChatBox::~ChatBox()
{
}
void ChatBox::send()
{
	sf::Packet packet;
	packet << Cl::Chat << mEntry->GetText().toAnsiString();
	mSocket->send(packet);
	mEntry->SetText("");
}
void ChatBox::setVisible(bool visible)
{
	mWindow->Show(visible);
}
void ChatBox::setPosition(const sf::Vector2f & v)
{
	mWindow->SetPosition(v);
}
void ChatBox::setBufferLimit(std::size_t i)
{
	mBufferLimit = i;
}

void ChatBox::add(sfg::Desktop & desktop)
{
	desktop.Add(mWindow);
}

void ChatBox::push(const std::string & s)
{
	mLabel->SetText("");
	if (mBuffer.size() > mBufferLimit)
		mBuffer.pop_front();
	mBuffer.push_back(s);

	sf::String string;
	for (const std::string & s : mBuffer)
		string += s + "\n";
	mLabel->SetText(string);
}

void ChatBox::remove(sfg::Desktop & desktop)
{
	desktop.Remove(mWindow);
}

void ChatBox::setSocket(sf::TcpSocket & socket)
{
	mSocket = &socket;
}