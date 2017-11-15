#include "stdafx.h"
#include "SocketClient.h"
#include "ptt.pb.h"
#include "myLog.h"
#include <time.h>
#include "Audio.h"
#include "CommendType.h"
#include <iostream>

#pragma comment(lib,"ws2_32.lib")

using namespace std;
//TODO 静态成员变量时序问题,建立新类管理所有全局变量

SocketClient::SocketClient()
{
}

void SocketClient::connect_server()
{
	this->start();
	log("SocketClient::connect_server");
	this->connect_tcp(ip, tcp_port);
	log("SocketClient::connect_server");
	this->connect_udp(ip, udp_port);
	this->to_login();
	//instance.close();
}




SocketClient::~SocketClient()
{
}

bool SocketClient::connect_tcp(const string& ip, const int& port)
{
	my_tcp_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (my_tcp_socket == INVALID_SOCKET)
	{
		log("invalid my_tcp_socket !");
		return false;
	}
	else
	{
		log("my_tcp_socket suss !");
	}

	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(port);
	serAddr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());

	if (connect(my_tcp_socket, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		log("psn_tcp_fd connect error !");
		closesocket(my_tcp_socket);
		return false;
	}
	else
	{
		tcp_listener.start(my_tcp_socket);
	}

	return true;
}

bool SocketClient::connect_udp(const string& ip, const int& port)
{
	my_udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (my_udp_socket <= 0)
	{
		return false;
	}
	else
	{
		log("my_udp_socket is%d\n", my_udp_socket);
	}
	my_udp_addrServ.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
	my_udp_addrServ.sin_family = AF_INET;
	my_udp_addrServ.sin_port = htons(port);

	if (connect(my_udp_socket, (sockaddr *)&my_udp_addrServ, sizeof(my_udp_addrServ)) == SOCKET_ERROR)
	{
		log("my_udp_socket connect error !");
		closesocket(my_udp_socket);
		return false;
	}
	else
	{
		udp_listener.start(my_udp_socket);
		log("my_udp_socket connect success!\n");
	}
	return true;
}

bool SocketClient::send_udp_msg(const vector<char>& content)
{
	if (!content.size())
	{
		return false;
	}
	int send_len = sendto(my_udp_socket, &content[0], content.size(), 0, (SOCKADDR*)&my_udp_addrServ, sizeof(SOCKADDR));
	return send_len>0;
}

bool SocketClient::send_tcp_msg(const vector<char>& content)
{
	if (!content.size())
	{
		return false;
	}
	cout <<"this"<<(int) this << endl;
	int send_len = send(my_tcp_socket, &content[0], content.size(), 0);
	return send_len>0;
}

short get_check_sum(const vector<char>& in) {



	int a = 1, b = 0;
	for (int i = 0; i<in.size(); i++)
	{
		a = (a + in.at(i)) % 45521;
		b = (b + a) % 45521;
	}
	b = b & 0xFF;
	a = a & 0xFF;
	return (short)((b << 8) | a);
}

/**
* 转义0x7e(0x7d5e) 0x7d(0x7d5d)
*
* @param bts
* @return
*/
int convert_msg(const vector<char>& raw, vector<char>& out) {

	for (char bt : raw) {
		if (bt == 0x7E) {
			out.push_back((char)0X7D);
			out.push_back((char)0X5E);
		}
		else if (bt == 0x7D) {
			out.push_back((char)0X7D);
			out.push_back((char)0X5D);
		}
		else {
			out.push_back(bt);
		}
	}
	return 0;
}

/**
* 转义0x7e(0x7d5e) 0x7d(0x7d5d)
*
* @param bts
* @return
*/
int convert_convert_msg(const vector<char>& raw, vector<char>& out) {
	bool convert_flag = false;
	for (char bt : raw)
	{
		if (convert_flag)
		{
			if (0X5E == bt)
			{
				out.push_back((char)0X7E);
				convert_flag = false;
			}
			else if (0x5D == bt)
			{
				out.push_back((char)0X7D);
				convert_flag = false;
			}
			else
			{
				log("error msg");
				return -1;
			}


		}
		else
		{
			if (0x7D == bt)
			{
				convert_flag = true;
			}
			else
			{
				out.push_back((char)bt);
			}

		}

	}
	return 0;
}




int get_tcp_output_msg(const vector<char>& content, const CommandType& command, vector<char>& out) {

	out.push_back(0x7E); // 包头[1]
	vector<char> mid_content;//TODO
	mid_content.push_back(command); // 消息指令[1]
	if (NULL == &mid_content)
	{
		return -1;
	}
	int length = content.size() + 3;// 长度[2]
	mid_content.reserve(length);

	mid_content.push_back(length & 0xff);
	mid_content.push_back(length >>8 & 0xff);
	
	mid_content.insert(mid_content.end(), content.begin(), content.end());// 可变消息[可变]
	for (char bt: mid_content)
	{
		printf("%d ", (int)bt);
	}
	short sum = get_check_sum(mid_content); // TODO 校验位[2]
	printf("sum:%d\n", sum);

	mid_content.push_back(sum & 0xff);
	mid_content.push_back(sum >> 8 & 0xff);
	
	vector<char> mid_out;
	if (NULL == &mid_out)
	{
		return -1;
	}
	mid_out.reserve(mid_content.size() * 2);
	printf("mid_content.size():%d\n", mid_content.size());
	convert_msg(mid_content, mid_out);

	printf("mid_out.size():%d\n", mid_out.size());
	out.insert(out.end(), mid_out.begin(), mid_out.end());
	out.push_back(0x7E); // 包尾[1]

	return 0;
}


void SocketClient::init(const std::string & ip, const int & tcp_port, const int & udp_port)
{
	this->ip = ip;
	this->tcp_port = tcp_port;
	this->udp_port = udp_port;
}

bool SocketClient::send_udp_heart()
{
	char udp_heart_beat[19] = { 0x80, 0x58, 0x00, 0x00, 0x00, 0x00,0x00, 0x00 };
	long uid = Audio::get_instance()->get_account()->get_user_id();
	udp_heart_beat[8] = (byte)(0xff & uid);
	udp_heart_beat[9] = (byte)((0xff00 & uid) >> 8);
	udp_heart_beat[10] = (byte)((0xff0000 & uid) >> 16);
	udp_heart_beat[11] = (byte)((0xff000000 & uid) >> 24);
	udp_heart_beat[12] = 0x41;
	udp_heart_beat[13] = 0x05;
	udp_heart_beat[14] = 0x00;
	udp_heart_beat[15] = 0x08;
	udp_heart_beat[16] = 0x00;
	udp_heart_beat[17] = 0x00;
	udp_heart_beat[18] = 0x00;
	;
	vector<char> mid_msg(begin(udp_heart_beat), end(udp_heart_beat));
	if (!send_udp_msg(mid_msg))
	{
		log("send udp heart fail\n");
		return false;
	}
	else
	{
		log("send udp heart suss\n");
	}

	return true;
}

bool SocketClient::send_tcp_heart()
{
	Ping p;
	time_t t = time(NULL);


	p.set_timestamp((unsigned int)t);

	char serial_len = p.ByteSize();
	char *serial_buf = new char[serial_len];
	if (NULL == serial_buf)
	{
		log("");
	}
	p.SerializeToArray(serial_buf, serial_len);
	vector<char> content(serial_buf, serial_buf + serial_len);
	delete[] serial_buf;
	vector<char> out;
	get_tcp_output_msg(content, PING, out);
	//printf("发送tcp心跳\n");
	//print_hex(send_buf,send_len);

	cout << "this" << (int)this << endl;
	int send_len = send_tcp_msg(out);



	if (send_len <= 0)
	{
		return false;
	}
	else
	{
		printf("send voice heart suss!\n");
	}
	return true;
}




bool SocketClient::to_login()
{
	Login login;

	login.set_account(Audio::get_instance()->get_account()->get_user_name());

	std::string platform("brew");
	login.set_platform(platform);

	std::string device("HS001C");
	login.set_device(device);

	login.set_password("");


	char serial_len = login.ByteSize();
	char *serial_buf = new char[serial_len];
	if (NULL == serial_buf)
	{
		log("NULL == serial_buf");
	}
	else
	{
		printf("serial_len:%d\n", serial_len);
	}
	login.SerializeToArray(serial_buf, serial_len);
	vector<char> content(serial_buf, serial_buf + serial_len);
	delete[] serial_buf;
	vector<char> out;
	printf("content.size():%d\n", content.size());
	get_tcp_output_msg(content, LOGIN, out);
	printf("out.size():%d\n", out.size());
	int send_len = send(my_tcp_socket, &out[0], out.size(), 0);

	if (send_len <= 0)
	{
		return false;
	}
	else
	{
		printf("login send_len:%d\n", send_len);
	}
	 print_hex(&out[0], out.size());


	
	return  0;
}

/**
* PTT话语权获取
*/
bool SocketClient::get_ptt()
{
	
	RequestMic reqMic;

	char serial_len = reqMic.ByteSize();
	char *serial_buf = new char[serial_len];
	if (NULL == serial_buf)
	{
		log("NULL == serial_buf");
	}
	else
	{
		printf("serial_len:%d\n", serial_len);
	}
	reqMic.SerializeToArray(serial_buf, serial_len);
	vector<char> content(serial_buf, serial_buf + serial_len);
	delete[] serial_buf;
	vector<char> out;
	printf("content.size():%d\n", content.size());
	get_tcp_output_msg(content, REQUEST_MIC, out);
	printf("out.size():%d\n", out.size());
	int send_len = send(my_tcp_socket, &out[0], out.size(), 0);

	if (send_len <= 0)
	{
		return false;
	}
	else
	{
		printf("login send_len:%d\n", send_len);
	}
	print_hex(&out[0], out.size());

	return false;
}



bool SocketClient::start()
{
	//初始化WSA
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}
	else 
	{
		log("SocketClient::start suss");
	}
	return false;
}

bool SocketClient::close()
{
	closesocket(my_udp_socket);
	closesocket(my_tcp_socket);
	WSACleanup();
	return true;
}


