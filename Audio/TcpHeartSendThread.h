#pragma once

#include <thread>
#include <mutex>
#include <atomic>


class TcpHeartSendThread
{
public:
	TcpHeartSendThread();
	~TcpHeartSendThread();
	void send_heart();
	void try_send_heart();
	void start();
	void operator()();
private:
	std::thread instance_thread;
	bool start_flag = false;
	std::atomic_flag send_flag = ATOMIC_FLAG_INIT;
};

