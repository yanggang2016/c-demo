#include "stdafx.h"
#include "AutoRequestMicThread.h"
#include "Audio.h"

using namespace std;
AutoRequestMicThread::AutoRequestMicThread()
{
}


AutoRequestMicThread::~AutoRequestMicThread()
{
}

void AutoRequestMicThread::start()
{
	start_flag = true;
	instance_thread = thread(ref(*this));
}

void AutoRequestMicThread::operator()()
{

	while (start_flag)
	{
		Audio::get_instance()->get_socket()->get_ptt();
		this_thread::sleep_for(chrono::seconds(15));
	}


}




