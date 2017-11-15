#include "stdafx.h"
#include "UdpHeartSendThread.h"
#include <chrono>
#include "Audio.h"


using namespace std;

UdpHeartSendThread::UdpHeartSendThread()
{
}


UdpHeartSendThread::~UdpHeartSendThread()
{
}

void UdpHeartSendThread::send_heart()
{
}

void UdpHeartSendThread::try_send_heart()
{
}

void UdpHeartSendThread::start()
{
	start_flag = true;
	instance_thread = thread(ref(*this));
}

void UdpHeartSendThread::operator()()
{
	this_thread::sleep_for(chrono::seconds(1));
	while (start_flag)
	{
		Audio::get_instance()->get_socket()->send_udp_heart();
		this_thread::sleep_for(chrono::seconds(10));
	}
}
