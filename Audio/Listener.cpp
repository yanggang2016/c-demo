#include "stdafx.h"
#include "Listener.h"
#include <iostream>
#include <vector>
#include "mylog.h"
#include "ptt.pb.h"
#include "Audio.h"

using namespace std;

void deal_msg(vector<unsigned char>& msg);
int decode_msg(const vector<unsigned char>& msg, vector<unsigned char>& out);

TcpListener::TcpListener()
{
}

TcpListener::~TcpListener()
{

}

//TODO
void TcpListener::start(SOCKET& socket)
{

	start_flag = true;
	my_socket = &socket;
	instance_thread = std::thread(std::ref(*this));
	
}

void TcpListener::operator()()
{

	char recv_buf[1024] = { 0 };
	int recv_len = 0;
	while (start_flag)
	{
		if ((recv_len = recv(*my_socket, recv_buf, 1024, 0)) <= 0) 
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
//TODO
void TcpListener::cat_msg(const unsigned char* msg,const int& len)
{

	if (0 == buf.size())
	{
		if (0x7e!= msg[0])
		{
			log("error receive msg 1");
			return;
		}

		for (size_t i = 1; i < len; i++)
		{
			if (0x7e != msg[i])
			{
				buf.push_back(msg[i]);
			}
			else 
			{
				deal_msg(buf);
				if (len - 1 > i)
				{
					cat_msg(&msg[i+1],len-1-i);
				}
			}
		}
	}
	else 
	{
		for (size_t i = 0; i < len ; i++)
		{
			if (0x7e != msg[i])
			{
				buf.push_back(msg[i]);
			}
			else
			{
				deal_msg(buf);
				if (len -1> i)
				{
					cat_msg(&msg[i + 1], len -1 - i);
				}
			}


		}
	}


}


void deal_msg(vector<unsigned char>& msg)
{
	log("begin deal_msg");
	vector<unsigned char> mid;
	decode_msg(msg,mid);
	msg.clear();
	cout << mid.size() << endl;
	if (mid.size()<2)
	{
		return;
	}

	log("begin deal_msg 2");
	fflush(stdin);
	printf("mid[0] > %02x ", mid[0]);
	fflush(stdin);
	Audio::get_instance()->update_time();
	switch (mid[0])
	{
	  /*服务端响应终端登录请求
	  这里的可变字段的数据结构如下:
		message LoginAck{
			required int32 result = 1;   //-1代表失败，>=0 代表成功

		message User{
			required uint32 uid = 1;   //user ID,用于在主动上报其他用户的时候的标识
		required string name = 2;   //user的name,用于在主动上报其他用户名称的时候的标识
		optional bool online = 3;   //在线状态  
		optional uint32 usr_attri = 4;    //用户属性设置，先初始化为1  
										  //…以后想增加对用户的描述，可以在这里添加  
		}
		required User usr = 2;

		message Configure{
			required uint32 default_group = 1;   //默认群组
		optional uint32 loc_report_period = 2;  //GPS上报周期
		optional bool audio_enabled = 3[default = true];   //是否允许说话  
		optional uint32 cfg_ptt_timeout = 4;   //PTT话语权一次最长时间
		optional uint32  heart_inter = 5;   //终端UDP心跳包的时间间隔
											//…以后想增加设置，可以在这里添加
		}
		required Configure conf = 3;
		optional ServerInfo tcp_server = 4;  // TCP 服务器 
		optional ServerInfo udp_server = 5;  // UDP 服务器
		optional ServerInfo update_server = 6; // 升级服务器
		}
			message ServerInfo{
			required string address = 1;
		required uint32 port = 2;
		}*/
	case 0x81:
	{
		//TODO
		LoginAck ack;
		if (ack.ParseFromArray(&mid[1], mid.size()))
		{
			log("error3");
		}
		else
		{
			log("suss");
		}
		int result = ack.result();
		User a = ack.usr();
		int uid = a.uid();
		Audio::get_instance()->get_account()->set_user_id(uid);
		cout <<"uid" <<uid << endl;
		Configure b = ack.conf();
		printf("ack.result():%d\n", ack.result());
		log("收到login \n");
		cout << "this" << (int)Audio::get_instance()->get_socket() << endl;
		Audio::get_instance()->start_heart_thread();
		Audio::get_instance()->get_socket()->get_ptt();
		break;
	}
		





		/*服务端响应终端获取话语权的请求
		message RequestMicAck{
			required int32 result = 1;   //-1代表失败，>=0 代表成功
		}*/
	case 0x86:
	{
		RequestMicAck ack;
		ack.ParseFromArray(&mid[1],mid.size());
		cout <<"get RequestMicAck >>>>" <<ack.result() << endl;
	}




		/*服务器发过来的查询包，需要终端进行响应
		message Ping{
			optional uint32 timestamp = 1;
		}*/
	case 0xFF:
	{
		Ping ack;
		ack.ParseFromArray(&mid[1], mid.size());
		break;
	}
		




		/*服务器发过来的push消息，用以通知自己已经丢失了话语权，这个是不需要终端进行响应的，终端此时需要停止UDP语音包的录制和发送。这种情况多发生在：
		a  有更高优先级的用户强了话语权
		b  当前说话的时间戳已到
		c  自己的一些语音包，服务器连续一段时间都没有收到
		d  管理员从调度台强制拆除自己
		message LostMic {
		required uint32 gid = 1;
		required string reason = 2;
		}*/
	case 0xC2:
		//TODO
		break;





		/*服务器发过来的push消息，用以通知自己已经被服务器踢出对讲系统，终端收到后需要关闭TCP, UDP等。这种情况多发生在：
		a  管理员从调度台强制踢出自己
		message Kickout {
		required string reason = 1;
		}*/
	case 0xC3:
		//TODO
		break;




		/*服务器发过来的push消息，用以通知自己相关的配置已经被管理员在后台修改了，终端收到后需要关变更参数等。这种情况多发生在：
		a  管理员在后台重新配置账号的配置参数
		message Reconfigured {
		required Configure conf = 1;
		}*/
	case 0xC4:
		//TODO
		break;




		/*服务器发过来的push消息，用以通知自己所在的群组已经被管理员在后台修改了，比如管理员在调度台中把自己强拉到了另外一个群组中，终端收到后需要变更活动群组的相关信息等。这种情况多发生在：
		a  管理员在调度台中把自己强拉到了另外一个群组中
		message CurrentGroup {
		required uint32 gid = 1;
		required string reason = 2;
		required string gname = 3;     //群组名称
		}*/
	case 0xC5:
		//TODO
		break;



		/*服务器发过来的push消息，用以通知自己所在的群组列表已经被管理员在后台修改了，比如管理员在后台把自己的群组列表信息修改了，终端收到后需要变更活动群组的相关信息等。这种情况多发生在：
		a  管理员在后台把自己的群组列表信息修改
		message GroupListChanged {
		repeated Group update_groups = 1;
		repeated uint32 rm_groups = 2;
		}*/
	case 0xC6:
		//TODO
		break;




		/*服务器发过来的push消息，用以通知自己所在的群组中的其他组员已经获取了Mic权限，要讲话了。这条push消息的存在会使用户在PTT的时候时间上快很多，当用户发现当前群组中没有人在讲话的情况下，可以直接发送UDP的语音包。这种情况多发生在：
		a  同一群组其他账号已经获取了话语权
		message MemberGetMic {
		required uint32 gid = 1;   //群组ID还是带上，防止出问题不好调试
		required uint32 uid = 2;
		}*/
	case 0xC7:
		//TODO
		break;




		/*服务器发过来的push消息，用以通知自己所在的群组中的其他组员已经释放了Mic权限。这条push消息的存在会使用户在PTT的时候时间上快很多，当用户发现当前群组中没有人在讲话的情况下，可以直接发送UDP的语音包。这种情况多发生在：
		a  同一群组其他账号已经释放了话语权
		message MemberLostMic{
		required uint32 gid = 1;   //群组ID还是带上，防止出问题不好调试
		required uint32 uid = 2;
		}*/
	case 0xC8:
		//TODO
		break;
	default:
		break;
	}
}
/**
* 转义0x7e(0x7d5e) 0x7d(0x7d5d)
*
* @param bts
* @return
*/
int decode_msg(const vector<unsigned char>& msg,vector<unsigned char>& out)
{
		bool convert_flag = false;
		for (auto bt : msg)
		{
			if (convert_flag)
			{
				if (0X5E == bt)
				{
					out.push_back(0X7E);
					convert_flag = false;
				}
				else if (0x5D == bt)
				{
					out.push_back(0X7D);
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
					out.push_back(bt);
				}
			}
		}

		return 0;
}