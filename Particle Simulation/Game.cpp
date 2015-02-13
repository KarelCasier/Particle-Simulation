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
, mWorldView(sf::Vector2f(0.f, 0.f), sf::Vector2f(1000, 1000))	///< View used to render world
, mUIView(sf::Vector2f(500, 500), sf::Vector2f(1000, 1000))		///< View used to render UI
, camPos(0, 0)
, bWindowInFocus(true)
, buttonCooldown(sf::seconds(0.1f))								///< Can press button every 0.1 seconds
, buttonTimer()
, bDirectInteraction(true)
, particleNumberOptions({ { 10 * 10, 100 * 100, 200 * 200, 300 * 300, 400 * 400, 500 * 500 } })
, particleNumberSelector(2)
{
	mWindow.setKeyRepeatEnabled(false);

	std::cout << "Controls" << std::endl;
	std::cout << "-------------------------------" << std::endl;
	std::cout << "|   Key   |      Action       |" << std::endl;
	std::cout << "-------------------------------" << std::endl;
	std::cout << "|   'M'   |    Change Mode    |" << std::endl;
	std::cout << "| L Click | Attract Particles | <- In Direct Interaction Mode" << std::endl;
	std::cout << "| R Click |  Repel Particles  | <- In Direct Interaction Mode" << std::endl;
	std::cout << "| L Click |Place Gravity Well | <- In Effector Placement Mode" << std::endl;
	std::cout << "| R Click |  Place Repulsor   | <- In Effector Placement Mode" << std::endl;
	std::cout << "|   'C'   |  Clear Effectors  |" << std::endl;
	std::cout << "|   'R'   |  Reset Particles  |" << std::endl;
	std::cout << "|   'W'   |  Toggle Wrapping  |" << std::endl;
	std::cout << "|   'D'   |   Toggle Drag     |" << std::endl;
	std::cout << "| R Arrow |Increase Particle #| <- Not optimized, may lag for a while" << std::endl;
	std::cout << "| L Arrow |Decrease Particle #| <- Not optimized, may lag for a while" << std::endl;
	std::cout << "-------------------------------" << std::endl;

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

	mParticleSystem = new ParticleSystem(particleNumberOptions.at(particleNumberSelector), 1000, 1000);				///< Initialize particle system with the desired number of particles and window size
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
	if (buttonTimer.getElapsedTime().asSeconds() >= buttonCooldown.asSeconds())
	{
		if (!bDirectInteraction)
		{
			if (getMouseButtonState(sf::Mouse::Left))
			{

				std::cout << "Place Gravity Well" << std::endl;
				mParticleSystem->addEffector(EffectorType::GRAVITYWELL, getMousePosition());
			}
			if (getMouseButtonState(sf::Mouse::Right))
			{
				std::cout << "Place Repulsor" << std::endl;
				mParticleSystem->addEffector(EffectorType::REPULSOR, getMousePosition());
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
		{
			if (bDirectInteraction)
			{
				bDirectInteraction = false;
				std::cout << "Entered Effector Placement Mode" << std::endl;
				mParticleSystem->setDirectInteractionState(false);
			}
			else
			{
				bDirectInteraction = true;
				std::cout << "Entered Direct Interaction Mode" << std::endl;
				mParticleSystem->setDirectInteractionState(true);
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			std::cout << "Toggling Particle Drag" << std::endl;
			mParticleSystem->toggleParticleDrag();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			std::cout << "Toggling Particle Wrapping" << std::endl;
			mParticleSystem->toggleParticleWrapping();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		{
			std::cout << "Particles Reset" << std::endl;
			mParticleSystem->resetParticles();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
		{
			std::cout << "Effectors Cleared" << std::endl;
			mParticleSystem->clearEffectors();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			if (particleNumberSelector > 0)
			{
				particleNumberSelector--;
			}
			delete mParticleSystem;
			mParticleSystem = new ParticleSystem(particleNumberOptions.at(particleNumberSelector), 1000, 1000);
			std::cout << "New Particle Count: " << particleNumberOptions.at(particleNumberSelector) << std::endl;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			if (particleNumberSelector < particleNumberOptions.size() -1)
			{
				particleNumberSelector++;
			}

			delete mParticleSystem;
			mParticleSystem = new ParticleSystem(particleNumberOptions.at(particleNumberSelector), 1000, 1000);
			std::cout << "New Particle Count: " << particleNumberOptions.at(particleNumberSelector) << std::endl;
		}

		buttonTimer.restart();
	}
}

void Game::update(sf::Time dTime)
{
	if (bWindowInFocus)
		mParticleSystem->update(dTime);
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
	mWindow.draw(*mParticleSystem);
	mWindow.setView(mUIView);
	mWindow.draw(mTextBackground);
	mWindow.draw(mStatisticsText);
	mWindow.setView(mWorldView);
	mWindow.display();
}