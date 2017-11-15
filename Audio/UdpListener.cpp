#include "stdafx.h"
#include "UdpListener.h"
#include "myLog.h"


UdpListener::UdpListener()
{
}


UdpListener::~UdpListener()
{
}

void UdpListener::start(SOCKET & socket)
{
	start_flag = true;
	my_socket = &socket;
	instance_thread = std::thread(std::ref(*this));
}

void UdpListener::operator()()
{
	char recv_buf[1024] = { 0 };
	int recv_len = 0;
	struct sockaddr_in addr;
	int addr_len = sizeof(struct sockaddr_in);
	while (start_flag)
	{
		if ((recv_len = recvfrom(*my_socket, recv_buf, recv_len, 0, (struct sockaddr*)&addr, &addr_len)) <= 0)
		{
			start_flag = false;
			log("my_socket error ");
			break;
		}
		else
		{
			log("receive msg >>");
			print_hex(recv_buf, recv_len);

			cat_msg((unsigned char*)recv_buf, recv_len);
		}
	}

}

void UdpListener::cat_msg(const unsigned char * msg, const int & len)
{



}
