#include "client\GameStateStack.h"
#include "client\GameState.h"


GameStateStack::Change::Change(Action action, GameState * g) :action(action), g(g)
{
}

void GameStateStack::step(Application & app)
{
	mStates.top()->step(app);
}

void GameStateStack::handleEvent(Application & app)
{
	mStates.top()->handleEvent(app);
}

void GameStateStack::draw(Application & app)
{
	mStates.top()->draw(app);
}

void GameStateStack::pop()
{
	mChanges.push_back(Change(Change::Pop));
}

void GameStateStack::push(GameState * g)
{
	mChanges.push_back(Change(Change::Push, g));
}

void GameStateStack::clear()
{
	mChanges.push_back(Change(Change::Clear));
}

bool GameStateStack::isEmpty()
{
	return mStates.empty();
}

void GameStateStack::applyChanges(Application & app)
{
	for (const auto & change : mChanges)
	{
		switch (change.action)
		{
		case Change::Push:
			if (mStates.size() > 0)
				mStates.top()->onObscure(app);
			mStates.push(std::unique_ptr<GameState>(change.g));
			mStates.top()->onEnter(app);
			break;
		case Change::Pop:
				mStates.top()->onExit(app);
				mStates.pop();
				if (mStates.size() > 0)
					mStates.top()->onReveal(app);
			break;
		case Change::Clear:
			while (mStates.size()>1)
			{
				mStates.top()->onExit(app);
				mStates.pop();
				mStates.top()->onReveal(app);
			}
			mStates.top()->onExit(app);
			mStates.pop();
			
			break;
		default:
			break;
		}
	}
	mChanges.clear();
}