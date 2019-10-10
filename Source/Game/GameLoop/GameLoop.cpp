#include <thread>
#include "SFML/Graphics.hpp"
#include "GameLoop.h"

namespace ME
{
namespace
{
constexpr u8 FPS_TARGET = 60;

struct State
{
	static const Time tickRate;
	bool bClosed = false;
	bool bQuitting = false;

	bool Expired() const
	{
		return bClosed || bQuitting;
	}
};

const Time State::tickRate = Time::Seconds(1.0f / FPS_TARGET);
State state;

std::unique_ptr<sf::CircleShape> uCircle;

void Create(sf::RenderWindow& outRW, u32 width, u32 height, const std::string& title)
{
	outRW.create(sf::VideoMode(width, height), title);
	sf::View centreView(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(width, height));
	outRW.setView(centreView);
}

void PollEvents(sf::RenderWindow& rw)
{
	sf::Event ev;
	while (rw.pollEvent(ev))
	{
		switch (ev.type)
		{
		default:
			break;
		case sf::Event::Closed:
			state.bClosed = true;
			break;
		}
	}
}

void Tick(Time /*dt*/)
{
	// Fire input
	// Update game state

	if (!uCircle)
	{
		uCircle = std::make_unique<sf::CircleShape>();
		uCircle->setFillColor(sf::Color::Yellow);
		uCircle->setRadius(100.0f);
		uCircle->setOrigin({100.0f, 100.0f});
	}
}

void Render(sf::RenderWindow& rw)
{
	if (uCircle)
	{
		rw.draw(*uCircle);
	}
	rw.display();
	rw.clear();
}

void Sleep(Time frameTime)
{
	Time residue = state.tickRate - frameTime;
	if (residue > Time::Zero)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(residue.AsMilliseconds()));
	}
}

void Cleanup()
{
	uCircle = nullptr;
}
} // namespace

s32 GameLoop::Run()
{
	state = State();
	sf::RenderWindow rw;
	Create(rw, 1280, 720, "Test");
	Time frameStart = Time::Now();
	Time frameTime;
	while (rw.isOpen() && !state.Expired())
	{
		Time dt = Time::Now() - frameStart;
		frameStart = Time::Now();
		PollEvents(rw);
		Tick(dt);
		Render(rw);
		frameTime = Time::Now() - frameStart;
		Sleep(frameTime);
	}
	Cleanup();
	return 0;
}
} // namespace ME
