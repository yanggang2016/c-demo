#pragma once
#include <Winsock2.h>
#include <thread>
#include <vector>
class UdpListener
{
public:
	UdpListener();
	~UdpListener();
	void start(SOCKET& my_socket);
	void operator()();
private:
	void cat_msg(const unsigned char * msg, const int & len);
	std::thread instance_thread;
	SOCKET* my_socket = NULL;
	bool start_flag = false;
	std::vector<unsigned char> buf;

};

