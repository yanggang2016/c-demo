#include "stdafx.h"
#include "ConvertPcmToEvrc.h"
#include "Audio.h"
#include <chrono>
#define MY_SHORT_MAX 0x7F
#define MY_SHORT_MIN 0x00

using namespace std;


ConvertPcmToEvrc::ConvertPcmToEvrc()
{
}


ConvertPcmToEvrc::~ConvertPcmToEvrc()
{
}

void ConvertPcmToEvrc::set_cookie(const int & cookie)
{
	this->cookie = cookie;
}

void ConvertPcmToEvrc::send_msg(const vector<char>& msg)
{
	lock.lock();

	lock.unlock();
}


void ConvertPcmToEvrc::convert (const vector<char>& msg)
{
	int flag = 1;
	bool hasData = true; // 该包数据是否处理完
	int frameFlag = 0; // 标记第几帧，每10帧发送一次
	auto begin = msg.begin();
	auto mid = begin;
	auto end = msg.end();
	unsigned char* result = NULL;
	unsigned int result_len = 0;
	vector<unsigned char> out;
	while (hasData) 
	{
		frameFlag++;
		if ((end-begin)>=320)
		{
			begin -= 320;
			if (!(end - begin))
			{
				hasData = false;
			}
		}
		else 
		{
			begin = end;
			hasData = false;
		}
		if (flag == 1 && frameFlag == 1) 
		{
			Audio::get_instance()->get_evrc()->pcmToEvrcAll((unsigned char*)&*mid, mid-begin, result, result_len);
		}
		else 
		{
				if (flag == 1 && frameFlag == 2) 
				{ // 半速率转换
					if (hasData) 
					{
						Audio::get_instance()->get_evrc()->pcmToEvrcHalf((unsigned char*)&*mid, mid - begin, result, result_len);
						//result = EvrcUtil.pcm2evrcBt(tmp, true, false);
					}
					else 
					{
						Audio::get_instance()->get_evrc()->pcmToEvrcHalf((unsigned char*)&*mid, mid - begin, result, result_len);
						//result = EvrcUtil.pcm2evrcBt(tmp, true, true);
					}
				}
			}
		if (result_len > 2) 
		{
				out.insert(out.end(),result+2,result + result_len - 1);
				free(result);
				result = NULL;
		}

	}
	Audio::get_instance()->get_evrc()->uninitEncoder();
	vector<char> real_out;
	add_rtp_head(out, real_out);
	Audio::get_instance()->get_socket()->send_tcp_msg(real_out);
}


/**
* 添加rtp头
*
* @param evrcBt
* @return
*/
void ConvertPcmToEvrc::add_rtp_head(const vector<unsigned char>& in,vector<char>& out)
{
	out.reserve(150);
	out.push_back(0x80);
	out.push_back(0x6F);

	int seg = getSeg(); // 流水号
	out.push_back(seg &0xff);
	out.push_back(seg >> 8 &0xff);
	__int64 time = std::chrono::time_point_cast<std::chrono::duration<__int64, std::ratio<1, 1000>>>(std::chrono::system_clock::now()).time_since_epoch().count();
	out.push_back(time & 0xff);
	out.push_back(time >> 8 & 0xff);
	out.push_back(time >> 16 & 0xff);
	out.push_back(time >> 24 & 0xff);
	int uid = Audio::get_instance()->get_account()->get_user_id();
	out.push_back(uid & 0xff);
	out.push_back(uid >> 8 & 0xff);
	out.push_back(uid >> 16 & 0xff);
	out.push_back(uid >> 24 & 0xff);
	if (in.size() == 112) {
		out.push_back(0xCA); // 第一包语音数据
	}
	else {
		out.push_back(0x0A);
	}
	out.push_back(cookie & 0xff);
	out.push_back(cookie >>8 & 0xff);
	out.push_back(cookie >>16 & 0xff);
	char cookie_char = cookie & 0xff;
	for (char bt : in ) 	// 语音数据
	{
		out.push_back(bt^cookie_char);
	}
}


/**
* 获取消息流水号 同一会话语音包序列号递增 不同会话间，序列号增加100
*
* @return
*/
int ConvertPcmToEvrc::getSeg() 
{
	if (++seg > MY_SHORT_MAX) {
		seg = MY_SHORT_MIN;
	}
	return seg;
}


/**
* 设置流水号加99
*/
void ConvertPcmToEvrc::nextSeg() 
{
	seg += 99;
}


