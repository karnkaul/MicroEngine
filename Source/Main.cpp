#include <assert.h>
#include <iostream>
#include <thread>
#include "SFML/Graphics.hpp"
#include "GameTypes.h"

struct Foo
{
	int val = 0;
};

using HPrim = int;

int main()
{
	ME::Factory<Foo, HPrim> factory;
	HPrim handle = factory.New<Foo>();
	auto pFoo = factory.Find<Foo>(handle);
	if (pFoo)
	{
		pFoo->val = 5;
		std::cout << "pFoo->val: " << pFoo->val << std::endl;
	}
	if (factory.Destroy(handle))
	{
		std::cout << handle << " destroyed" << std::endl;
	}
	pFoo = factory.Find<Foo>(handle);
	assert(pFoo == nullptr);
	std::cout << "Hello!" << std::endl;

	sf::RenderWindow rw;
	rw.create(sf::VideoMode(1280, 720), "Test");
	bool bExit = false;
	while (rw.isOpen() && !bExit)
	{
		sf::Event ev;
		while (rw.pollEvent(ev))
		{
			switch (ev.type)
			{
			default:
				break;
			case sf::Event::Closed:
				bExit = true;
				break;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}
