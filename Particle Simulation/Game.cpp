#include "Game.h"
#include <algorithm>
#include <functional>
#include "Utilities.h"


const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f); // = 0.6 seconds per frame, therefore 60fps

Game* Game::s_pInstance = nullptr;								///< set game instance pointer to nullptr

Game::Game()
: mWindow(sf::VideoMode(1000, 1000), "Particle Simulator")		///< Initialize window
, mFont()
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
, mWorldView(sf::Vector2f(0.f,0.f), sf::Vector2f(1000, 1000))	///< View used to render world
, mUIView(sf::Vector2f(500, 500), sf::Vector2f(1000, 1000))		///< View used to render UI
, camPos(0, 0)
, bWindowInFocus(true)
, mParticleSystem(300*300, 1000,1000)							///< Initialize particle system with the desired number of particles and window size
, effectorPlacementCooldown(sf::seconds(0.2f))					///< Can place new effector every 0.2 seconds
, effectorPlacementTimer()
{
	mWindow.setKeyRepeatEnabled(false);

	///<------------------------------------------------------->///< Statistics Initializing
	if (!mFont.loadFromFile("MYRIADPRO-BOLD.OTF"))
	{
		std::cerr << "Can't load font" << std::endl;
	}
	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10);

	mTextBackground.setSize(sf::Vector2f(125.f, 35.f));
	mTextBackground.setFillColor(sf::Color(110, 110, 110, 80));
	///<------------------------------------------------------->
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

		if (event.type == sf::Event::GainedFocus)
		{
			bWindowInFocus = true;
		}
		else if (event.type == sf::Event::LostFocus)
		{
			bWindowInFocus = false;
		}
	}

	//Real-time events
	if (getMouseButtonState(sf::Mouse::Left))
	{
		if (effectorPlacementTimer.getElapsedTime().asSeconds() >= effectorPlacementCooldown.asSeconds())
		{
			std::cout << "Place Gravity Well" << std::endl;
			mParticleSystem.addEffector(EffectorType::GRAVITYWELL, getMousePosition());
			effectorPlacementTimer.restart();
		}
	}
	if (getMouseButtonState(sf::Mouse::Right))
	{
		if (effectorPlacementTimer.getElapsedTime().asSeconds() >= effectorPlacementCooldown.asSeconds())
		{
			std::cout << "Place Repulsor" << std::endl;
			mParticleSystem.addEffector(EffectorType::REPULSOR, getMousePosition());
			effectorPlacementTimer.restart();
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		mParticleSystem.resetParticles();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
	{
		mParticleSystem.clearEffectors();
	}
}

void Game::update(sf::Time dTime)
{
	if (bWindowInFocus)
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