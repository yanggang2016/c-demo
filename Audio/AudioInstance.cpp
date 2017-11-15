#include "stdafx.h"
#include "AudioInstance.h"
#include "Audio.h"

AudioInstanceInterFace::AudioInstanceInterFace()
{
}


AudioInstanceInterFace::~AudioInstanceInterFace()
{
}

void AudioInstanceInterFace::init(const char * psn,const char * ip, const long tcp_port, const long udp_port)
{
	Audio::get_instance()->init(psn,ip,tcp_port,udp_port);
}

void AudioInstanceInterFace::close()
{
	Audio::get_instance()->close();

}

void AudioInstanceInterFace::start()
{
	Audio::get_instance()->start();
}
