#pragma once
#include <thread>
class AutoRequestMicThread
{
public:
	AutoRequestMicThread();
	~AutoRequestMicThread();
	void start();
	void operator()();
private:
	std::thread instance_thread;
	bool start_flag = false;
};

