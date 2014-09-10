#include <iostream>

#include "boost/chrono.hpp"
#include "boost/thread.hpp"
#include "boost/any.hpp"
#include "boost/scoped_ptr.hpp"

#include "../Control/Control.h"

volatile int global = 0;
//
//int  test_int(int i)
//{
//	boost::any var;
//	var = i;
//	return boost::any_cast<int>(var);
//}
//
//std::string test_string(const std::string& str)
//{
//	boost::any var;
//	var = str;
//	return boost::any_cast<std::string>(var);
//}

char buffer[4] = {0};

void test(int i)
{
	for(int j=0; ; j++)
	{
		global = j;
		std::cout<<&global<<std::endl;
	}
}

int main()
{
	boost::thread thrd0([](){test(0);});
	boost::thread thrd1([](){test(1);});
	//boost::thread thrd2([&global](){test(2);});
	//boost::thread thrd3([&global](){test(3);});

	char chr;
	std::cin>>chr;

//	std::string str = "hello";
//	using namespace boost::chrono;
//	time_point<system_clock> t1, t2;
//	t1 = system_clock::now();
//	for(int i=0; i<100000; i++)
//	{
//		test_int(i);
//	}
//	t2 = system_clock::now();
//	std::cout<<"test int, total time(s): "<<duration_cast<milliseconds>(t2-t1).count()/1000.0<<std::endl;
//
//	t1 = system_clock::now();
//	for(int i=0; i<100000; i++)
//	{
//		test_string(str);
//	}
//	t2 = system_clock::now();
//	std::cout<<"test string, total time(s): "<<duration_cast<milliseconds>(t2-t1).count()/1000.0<<std::endl;

//	Initialize();
//
//	boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
//
//	auto f = [](){global = 1;};
//	f();
//	std::cout<<global<<std::endl;
//
//	Terminate();

	return 0;
}