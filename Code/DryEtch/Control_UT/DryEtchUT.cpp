#include <iostream>

#include "boost/chrono.hpp"
#include "boost/thread.hpp"

#include "../Control/Control.h"

int global = 0;

int main()
{
	Initialize();

	boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));

	auto f = [](){global = 1;};
	f();
	std::cout<<global<<std::endl;

	Terminate();

	return 0;
}