#pragma once
#include <mutex>          // std::mutex, std::lock_guard, std::adopt_lock
#include <string>
#include <vector>
#include "Account.h"
#include "SocketClient.h"
#include "TcpHeartSendThread.h"
#include "UdpHeartSendThread.h"
#include "EvrcLibLoad.h"

enum AudioStateType {
	OnLine = 0x01,
	InTemporaryGroup = 0x10,
	Talking = 0x100,

};


class Audio
{
public:
	static Audio* get_instance();
	unsigned init(const char* psn, const char* ip, const long tcp_port, const long udp_port);
	unsigned start();
	unsigned close();
	Account* get_account() ;
	SocketClient* get_socket();
	EvrcLibLoad* get_evrc();
	void init(const std::string& name, const std::string& pwd,  const std::string& ip, const int& tcp_port, const int& udp_port);
	void start_heart_thread();
	int update_time();
	unsigned get_audio_state();
	void set_audio_state(unsigned&  state);
	~Audio();
private :
	static  std::mutex* lock;
	static Audio* instance;
	Account* account = new Account();
	SocketClient* socket_client = new SocketClient();
	Audio();
	std::chrono::steady_clock::time_point  last_time = std::chrono::steady_clock::now();//last receive msg  from server
	TcpHeartSendThread* tcp_heart_thread = new TcpHeartSendThread();
	UdpHeartSendThread* udp_heart_thread = new UdpHeartSendThread();
	EvrcLibLoad* evrc_instance = new EvrcLibLoad();
	unsigned state = 0;
};






