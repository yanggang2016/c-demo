#pragma once

class AudioInstanceInterFace
{
public:
	AudioInstanceInterFace();
	~AudioInstanceInterFace();
	void static init(const char* psn,const char* ip,const long tcp_port,const long udp_port);
	void static close();
	void static start();
private:

};

