#include "stdafx.h"
#include "TcpHeartSendThread.h"
#include <chrono>
#include "Audio.h"

using namespace std;

TcpHeartSendThread::TcpHeartSendThread()
{


}


TcpHeartSendThread::~TcpHeartSendThread()
{


}

void TcpHeartSendThread::send_heart()
{
	if (send_flag.test_and_set())
	{

	}

}

void TcpHeartSendThread::try_send_heart()
{

}

void TcpHeartSendThread::start()
{
	start_flag = true;
	instance_thread = thread(ref(*this));
}

void TcpHeartSendThread::operator()()
{
	this_thread::sleep_for(chrono::seconds(1));
	while (start_flag)
	{
		Audio::get_instance()->get_socket()->send_tcp_heart();
		this_thread::sleep_for(chrono::seconds(5));
	}

}
