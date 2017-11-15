#include "stdafx.h"
#include "Audio.h"
#include "myLog.h"
#include <iostream>

using namespace std;

mutex* Audio::lock = new mutex();
Audio* Audio::instance = new Audio();
Audio::Audio()
{
}



Audio * Audio::get_instance()
{
		return instance;
}

unsigned Audio::init(const char * psn, const char * ip, const long tcp_port, const long udp_port)
{
	string mid_psn = psn;
	init(mid_psn, "", ip, tcp_port, udp_port);
	return 0;
}

unsigned  Audio::start()
{
	socket_client->connect_server();
	return 0;
}

unsigned Audio::close()
{
	
	return 0;
}

Account * Audio::get_account()
{
	return account;
}

SocketClient * Audio::get_socket()
{
	return socket_client;
}

EvrcLibLoad * Audio::get_evrc()
{
	return evrc_instance;
}

void Audio::init(const string & name, const string & pwd, const string & ip, const int & tcp_port, const int & udp_port)
{
	account->set_user_name(name);
	account->set_pwd(pwd);
	socket_client->init(ip, tcp_port, udp_port);
}

void Audio::start_heart_thread()
{
	tcp_heart_thread->start();
	udp_heart_thread->start();
}

int Audio::update_time()
{
	lock->lock();
	last_time = std::chrono::steady_clock::now();
	lock->unlock();
	return 0;
}

unsigned Audio::get_audio_state()
{
	return state;
}

void Audio::set_audio_state(unsigned & state)
{
	this->state = state;
}

Audio::~Audio()
{

}

