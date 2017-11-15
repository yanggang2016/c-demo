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
	  /*�������Ӧ�ն˵�¼����
	  ����Ŀɱ��ֶε����ݽṹ����:
		message LoginAck{
			required int32 result = 1;   //-1����ʧ�ܣ�>=0 ����ɹ�

		message User{
			required uint32 uid = 1;   //user ID,�����������ϱ������û���ʱ��ı�ʶ
		required string name = 2;   //user��name,�����������ϱ������û����Ƶ�ʱ��ı�ʶ
		optional bool online = 3;   //����״̬  
		optional uint32 usr_attri = 4;    //�û��������ã��ȳ�ʼ��Ϊ1  
										  //���Ժ������Ӷ��û����������������������  
		}
		required User usr = 2;

		message Configure{
			required uint32 default_group = 1;   //Ĭ��Ⱥ��
		optional uint32 loc_report_period = 2;  //GPS�ϱ�����
		optional bool audio_enabled = 3[default = true];   //�Ƿ�����˵��  
		optional uint32 cfg_ptt_timeout = 4;   //PTT����Ȩһ���ʱ��
		optional uint32  heart_inter = 5;   //�ն�UDP��������ʱ����
											//���Ժ����������ã��������������
		}
		required Configure conf = 3;
		optional ServerInfo tcp_server = 4;  // TCP ������ 
		optional ServerInfo udp_server = 5;  // UDP ������
		optional ServerInfo update_server = 6; // ����������
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
		log("�յ�login \n");
		cout << "this" << (int)Audio::get_instance()->get_socket() << endl;
		Audio::get_instance()->start_heart_thread();
		Audio::get_instance()->get_socket()->get_ptt();
		break;
	}
		





		/*�������Ӧ�ն˻�ȡ����Ȩ������
		message RequestMicAck{
			required int32 result = 1;   //-1����ʧ�ܣ�>=0 ����ɹ�
		}*/
	case 0x86:
	{
		RequestMicAck ack;
		ack.ParseFromArray(&mid[1],mid.size());
		cout <<"get RequestMicAck >>>>" <<ack.result() << endl;
	}




		/*�������������Ĳ�ѯ������Ҫ�ն˽�����Ӧ
		message Ping{
			optional uint32 timestamp = 1;
		}*/
	case 0xFF:
	{
		Ping ack;
		ack.ParseFromArray(&mid[1], mid.size());
		break;
	}
		




		/*��������������push��Ϣ������֪ͨ�Լ��Ѿ���ʧ�˻���Ȩ������ǲ���Ҫ�ն˽�����Ӧ�ģ��ն˴�ʱ��ҪֹͣUDP��������¼�ƺͷ��͡���������෢���ڣ�
		a  �и������ȼ����û�ǿ�˻���Ȩ
		b  ��ǰ˵����ʱ����ѵ�
		c  �Լ���һЩ������������������һ��ʱ�䶼û���յ�
		d  ����Ա�ӵ���̨ǿ�Ʋ���Լ�
		message LostMic {
		required uint32 gid = 1;
		required string reason = 2;
		}*/
	case 0xC2:
		//TODO
		break;





		/*��������������push��Ϣ������֪ͨ�Լ��Ѿ����������߳��Խ�ϵͳ���ն��յ�����Ҫ�ر�TCP, UDP�ȡ���������෢���ڣ�
		a  ����Ա�ӵ���̨ǿ���߳��Լ�
		message Kickout {
		required string reason = 1;
		}*/
	case 0xC3:
		//TODO
		break;




		/*��������������push��Ϣ������֪ͨ�Լ���ص������Ѿ�������Ա�ں�̨�޸��ˣ��ն��յ�����Ҫ�ر�������ȡ���������෢���ڣ�
		a  ����Ա�ں�̨���������˺ŵ����ò���
		message Reconfigured {
		required Configure conf = 1;
		}*/
	case 0xC4:
		//TODO
		break;




		/*��������������push��Ϣ������֪ͨ�Լ����ڵ�Ⱥ���Ѿ�������Ա�ں�̨�޸��ˣ��������Ա�ڵ���̨�а��Լ�ǿ����������һ��Ⱥ���У��ն��յ�����Ҫ����Ⱥ��������Ϣ�ȡ���������෢���ڣ�
		a  ����Ա�ڵ���̨�а��Լ�ǿ����������һ��Ⱥ����
		message CurrentGroup {
		required uint32 gid = 1;
		required string reason = 2;
		required string gname = 3;     //Ⱥ������
		}*/
	case 0xC5:
		//TODO
		break;



		/*��������������push��Ϣ������֪ͨ�Լ����ڵ�Ⱥ���б��Ѿ�������Ա�ں�̨�޸��ˣ��������Ա�ں�̨���Լ���Ⱥ���б���Ϣ�޸��ˣ��ն��յ�����Ҫ����Ⱥ��������Ϣ�ȡ���������෢���ڣ�
		a  ����Ա�ں�̨���Լ���Ⱥ���б���Ϣ�޸�
		message GroupListChanged {
		repeated Group update_groups = 1;
		repeated uint32 rm_groups = 2;
		}*/
	case 0xC6:
		//TODO
		break;




		/*��������������push��Ϣ������֪ͨ�Լ����ڵ�Ⱥ���е�������Ա�Ѿ���ȡ��MicȨ�ޣ�Ҫ�����ˡ�����push��Ϣ�Ĵ��ڻ�ʹ�û���PTT��ʱ��ʱ���Ͽ�ܶ࣬���û����ֵ�ǰȺ����û�����ڽ���������£�����ֱ�ӷ���UDP������������������෢���ڣ�
		a  ͬһȺ�������˺��Ѿ���ȡ�˻���Ȩ
		message MemberGetMic {
		required uint32 gid = 1;   //Ⱥ��ID���Ǵ��ϣ���ֹ�����ⲻ�õ���
		required uint32 uid = 2;
		}*/
	case 0xC7:
		//TODO
		break;




		/*��������������push��Ϣ������֪ͨ�Լ����ڵ�Ⱥ���е�������Ա�Ѿ��ͷ���MicȨ�ޡ�����push��Ϣ�Ĵ��ڻ�ʹ�û���PTT��ʱ��ʱ���Ͽ�ܶ࣬���û����ֵ�ǰȺ����û�����ڽ���������£�����ֱ�ӷ���UDP������������������෢���ڣ�
		a  ͬһȺ�������˺��Ѿ��ͷ��˻���Ȩ
		message MemberLostMic{
		required uint32 gid = 1;   //Ⱥ��ID���Ǵ��ϣ���ֹ�����ⲻ�õ���
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
* ת��0x7e(0x7d5e) 0x7d(0x7d5d)
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