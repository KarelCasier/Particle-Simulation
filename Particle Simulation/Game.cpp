#include "Game.h"
#include <algorithm>
#include <functional>
#include "Utilities.h"


const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f); // = 0.6 seconds per frame, therefore 60fps

Game* Game::s_pInstance = nullptr;

Game::Game()
: mWindow(sf::VideoMode(1000, 1000), "Particle Simulator")
, mFont()
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
, mWorldView(sf::Vector2f(0.f,0.f), sf::Vector2f(1000, 1000))
, mUIView(sf::Vector2f(500, 500), sf::Vector2f(1000, 1000))
, camPos(0, 0)
, mParticleSystem(160000)
{
		//mWindow.setKeyRepeatEnabled(false);

	if (!mFont.loadFromFile("MYRIADPRO-BOLD.OTF"))
	{
		std::cerr << "Can't load font" << std::endl;
	}
	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10);

	mTextBackground.setSize(sf::Vector2f(125.f, 35.f));
	mTextBackground.setFillColor(sf::Color(110, 110, 110, 80));
	
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart(); //Note restart returns the time on the clock then restarts the clock
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame) //Ensures that the logic is caught up before rendering again
		{
			timeSinceLastUpdate -= TimePerFrame;

			processEvents();
			update(TimePerFrame);
		}

		updateStatistics(elapsedTime);
		render();
	}
	mWindow.close();
}

void Game::processEvents()
{

	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			mWindow.close();
	}
}

void Game::update(sf::Time dTime)
{
	mParticleSystem.update(dTime);
}

void Game::updateStatistics(sf::Time elapsedTime)
{
	std::stringstream strStream;

	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		strStream << "Frames / Second = " << mStatisticsNumFrames << "\n" <<
			"Time / Update = " << mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames << "us" << std::endl;

		mStatisticsText.setString(strStream.str());

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

void Game::render()
{
	mWindow.clear();

	mWindow.setView(mWorldView);
	mWindow.draw(mParticleSystem);
	mWindow.setView(mUIView);
	mWindow.draw(mTextBackground);
	mWindow.draw(mStatisticsText);
	mWindow.setView(mWorldView);
	mWindow.display();
}