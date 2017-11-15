#pragma once
#include <thread>
class UdpHeartSendThread
{
public:
	UdpHeartSendThread();
	~UdpHeartSendThread();
	void send_heart();
	void try_send_heart();
	void start();
	void operator()();
private:
	std::thread instance_thread;
	bool start_flag = false;
};

