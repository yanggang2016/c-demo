#pragma once
#include <mutex>
class MyLockClass
{
public:
	MyLockClass();
	int try_lock();
	~MyLockClass();
private:
	 std::mutex* lock = NULL;
};

