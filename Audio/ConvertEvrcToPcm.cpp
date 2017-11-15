#include "stdafx.h"
#include "ConvertEvrcToPcm.h"
#include "myLog.h"
#include "Audio.h"
#define HALF_EVRC_ALL 100
#define ALL_EVRC_ALL 112
#define RTP_HEAD 16
#define HALF_EVRC_ONE 10
#define ALL_EVRC_ONE 22

using namespace std;
void decode_msg(const vector<unsigned char>& in, vector<unsigned char >& out);

ConvertEvrcToPcm::ConvertEvrcToPcm()
{
}


ConvertEvrcToPcm::~ConvertEvrcToPcm()
{
}

void ConvertEvrcToPcm::convert_msg(const std::vector<unsigned char>& in)
{
	vector<unsigned char > out;
	decode_msg(in,out);
}



void decode_msg (const vector<unsigned char>& in,vector<unsigned char >& out)
{
	 if (in.size()!=(HALF_EVRC_ALL + RTP_HEAD)|| in.size() != (ALL_EVRC_ALL+ RTP_HEAD))
	 {
		 log("error evrc lenth");
		 return;
	 }
	bool hasData = true;
	unsigned char pack_type = in[12];
	unsigned char cur_key = in[13]; //加密种子，用于解密语音数据
	vector<unsigned char> tmp;
	tmp.reserve(132);
	unsigned char* result = NULL;
	unsigned int result_len = 0;
	auto begin = in.begin();
	auto mid = begin;
	auto end = in.end();
	if (pack_type == 0xCA) 
	{
		 //第一包语音数据
		tmp.push_back(0x04);
		tmp.push_back(0x16); //全速率
		
		for (begin += RTP_HEAD ; begin < end ; begin++)
		{
			tmp.push_back((unsigned char)(cur_key^*begin));
		}
	}
	else if (pack_type == 0x0A) 
	{
			while (hasData) 
			{
				if (end-begin ==  0) 
				{
					hasData = false;
					break;
				}
				tmp.push_back(0x03);
				tmp.push_back(0x0A);
				if (end - begin >= HALF_EVRC_ONE) 
				{
					begin += HALF_EVRC_ONE;
					for (; mid < begin ; mid++) 
					{
						tmp.push_back(cur_key^*mid);
					}
				}
				else 
				{
					begin = end;
					for (; mid < begin; mid++)
					{
						tmp.push_back(cur_key^*mid);
					}
				}
			}
	}
	else
	{
		return;
	}

	Audio::get_instance()->get_evrc()->evrcToPcm(&tmp[0], tmp.size(), result, result_len);
	out.insert(out.end(),result, result+result_len);
	free(result);
	Audio::get_instance()->get_evrc()->uninitDecoder();
}
