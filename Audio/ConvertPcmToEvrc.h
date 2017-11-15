#pragma once
#include <vector>
#include <mutex>
class ConvertPcmToEvrc
{
public:
	ConvertPcmToEvrc();
	~ConvertPcmToEvrc();
	void set_cookie(const int& cookie);
	void send_msg(const std::vector<char>&);
	void convert(const std::vector<char>& msg);
	void add_rtp_head(const std::vector<unsigned char>& in, std::vector<char>& out);
private:
	int cookie = 0;
	bool first_flag = false;
	std::mutex lock ;
	int seg = 0;//Á÷Ë®ºÅ
	int getSeg();
	void nextSeg();
};

