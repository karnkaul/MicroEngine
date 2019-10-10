#include <assert.h>
#include <iostream>
#include <thread>
#include "Game/GameLoop/GameLoop.h"

struct Foo
{
	s32 val = 0;
};

using HPrim = s32;

s32 main(s32 argc, char** argv)
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

	return ME::GameLoop::Run(argc, argv);
}
