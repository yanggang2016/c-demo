#include "stdafx.h"
#include "MyLockClass.h"

using namespace std;
MyLockClass::MyLockClass()
{
	lock = new mutex();
}

int MyLockClass::try_lock()
{
	lock->lock();
	return 0;
}


MyLockClass::~MyLockClass()
{
	lock->unlock();
	delete lock;
	lock = NULL;
}
