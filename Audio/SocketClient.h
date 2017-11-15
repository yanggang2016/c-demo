#pragma once
#include <string>
#include <Winsock2.h>
#include <vector>
#include "Listener.h"
#include "UdpListener.h"


class SocketClient
{
public:

	void init(const std::string& ip, const int& tcp_port, const int& udp_port);
	void connect_server();
	bool send_udp_heart();
	bool send_tcp_heart();
	bool send_tcp_msg(const std::vector<char>&);
	bool get_ptt();

	~SocketClient();
	SocketClient();
private:
	
	SOCKET my_udp_socket ;
	SOCKET my_tcp_socket ;
	SOCKADDR_IN  my_udp_addrServ;
	bool connect_tcp(const std::string&, const int&);
	bool connect_udp(const std::string&, const int&);
	bool send_udp_msg(const std::vector<char>&);
	
	
	bool to_login();
	

	std::string ip;
	int tcp_port = 0;
	int udp_port = 0;
	bool start();
	bool close();

	TcpListener& tcp_listener  = TcpListener();
	UdpListener& udp_listener	= UdpListener();
};


