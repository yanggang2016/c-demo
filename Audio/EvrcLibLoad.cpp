#include "stdafx.h"
#include "EvrcLibLoad.h"
#include <direct.h>
#include <string.h>
#include <malloc.h>
#include "myLog.h"
#include "windows.h"
#include <typeinfo>
#include <vector>
#include <iostream>
#pragma comment(lib, "libprotobufd.lib")  
#pragma comment(lib, "libprotobuf-lited.lib") 
#pragma comment(lib, "libprotocd.lib") 



	HINSTANCE  hDll;
	void* ptr_pcm2evrc = NULL;
	void* ptr_evrc2pcm = NULL;
	bool loadevrclib();
	void isNull(void* ptr);
	int encodeBt(const unsigned char* raw, unsigned char* &outfilebuffer, const int len);
	int encodeBtAll(const unsigned char* raw, unsigned char* &outfilebuffer, int len);
	int decodeBt(const unsigned char* raw, unsigned char* &outfilebuffer, int len);

	typedef  unsigned char uint8_t;
	typedef  unsigned int size_t;
	typedef short int16_t;


	typedef int(*evrc_get_stream_frame_count)(const uint8_t* bits, size_t bits_bytes);
	typedef int(*evrc_packet_to_stream)(const uint8_t* packet, size_t packet_size, uint8_t* stream, size_t stream_size);
	typedef int(*evrc_stream_to_packet)(const uint8_t* stream, size_t frame_count, uint8_t* packet, size_t packet_size);

	evrc_get_stream_frame_count my_evrc_get_stream_frame_count = NULL;
	evrc_packet_to_stream my_evrc_packet_to_stream = NULL;
	evrc_stream_to_packet my_evrc_stream_to_packet = NULL;

	// encoder interface
	typedef void* (*evrc_encoder_init)(int16_t min_rate, int16_t max_rate, int16_t noise_suppression);
	typedef void(*evrc_encoder_uninit)(void* c);
	typedef int(*evrc_encoder_encode_to_packet)(void* c, int16_t* speech, size_t speech_samples, uint8_t* packet, size_t packet_max_bytes);
	typedef int(*evrc_encoder_encode_to_stream)(void* c, int16_t* speech, size_t speech_samples, uint8_t* bits, size_t bits_max_bytes);
	typedef int(*evrc_encoder_max_encode)(void* c, size_t speech_samples);

	evrc_encoder_init my_evrc_encoder_init = NULL;
	evrc_encoder_uninit my_evrc_encoder_uninit = NULL;
	evrc_encoder_encode_to_packet my_evrc_encoder_encode_to_packet = NULL;
	evrc_encoder_encode_to_stream my_evrc_encoder_encode_to_stream = NULL;
	evrc_encoder_max_encode my_evrc_encoder_max_encode = NULL;

	// decoder interface
	typedef void* (*evrc_decoder_init)();
	typedef void(*evrc_decoder_uninit)(void* c);
	typedef int(*evrc_decoder_decode_from_packet)(void* c, const uint8_t* packet, size_t packet_bytes, int16_t* speech, size_t speech_max_samples);
	typedef int(*evrc_decoder_decode_from_stream)(void* c, const uint8_t* stream, size_t stream_bytes, int16_t* speech, size_t speech_max_samples);
	typedef int(*evrc_decoder_decode_stream_frame)(void* c, const uint8_t* bits, size_t bits_bytes, int16_t* speech);
	typedef int(*evrc_decoder_stream_max_sample)(const uint8_t* bits, size_t bits_bytes);
	typedef int(*evrc_decoder_stream_frame_bytes)(const uint8_t* bits);
	typedef int(*evrc_decoder_is_silence_stream_frame)(const uint8_t* bits);

	evrc_decoder_init my_evrc_decoder_init = NULL;
	evrc_decoder_uninit my_evrc_decoder_uninit = NULL;
	evrc_decoder_decode_from_packet my_evrc_decoder_decode_from_packet = NULL;
	evrc_decoder_decode_from_stream my_evrc_decoder_decode_from_stream = NULL;
	evrc_decoder_decode_stream_frame my_evrc_decoder_decode_stream_frame = NULL;
	evrc_decoder_stream_max_sample my_evrc_decoder_stream_max_sample = NULL;
	evrc_decoder_stream_frame_bytes my_evrc_decoder_stream_frame_bytes = NULL;
	evrc_decoder_is_silence_stream_frame my_evrc_decoder_is_silence_stream_frame = NULL;



	EvrcLibLoad::EvrcLibLoad()
	{
		if (loadevrclib())
		{
			log("load evrc lib suss");
		}
		else
		{
			log("load evrc lib error");
		}
	}


	EvrcLibLoad::~EvrcLibLoad()
	{
	}

	
	void EvrcLibLoad::pcmToEvrcHalf(const unsigned char * raw, const unsigned int & rawLen, unsigned char * &out, unsigned int & outLen)
	{
		if (NULL == ptr_pcm2evrc)
		{
			ptr_pcm2evrc = my_evrc_encoder_init(3, 3, 1);
		}
		outLen = encodeBt(raw, out, rawLen);
		my_evrc_encoder_uninit(ptr_pcm2evrc);
	}



	void EvrcLibLoad::pcmToEvrcAll(const unsigned char *raw, const unsigned int &rawLen, unsigned char *&out, unsigned int &outLen)
	{
		ptr_pcm2evrc = my_evrc_encoder_init(4, 4, 1);
		outLen = encodeBtAll(raw, out, rawLen);
		my_evrc_encoder_uninit(ptr_pcm2evrc);
	}

	void EvrcLibLoad::evrcToPcm(const unsigned char *raw, const unsigned int &rawLen, unsigned char *out, unsigned int &outLen)
	{

		if (NULL == ptr_evrc2pcm) {
			ptr_evrc2pcm = my_evrc_decoder_init();
		}
		outLen = decodeBt(raw, out, rawLen);
	}

	void EvrcLibLoad::uninitDecoder()
	{
		if (NULL == ptr_evrc2pcm) 
		{
		}
		else 
		{
			my_evrc_decoder_uninit(ptr_evrc2pcm);
			ptr_evrc2pcm = nullptr;
		}
		
	}

	void EvrcLibLoad::uninitEncoder()
	{
		if (NULL == ptr_pcm2evrc)
		{
		}
		else
		{
			my_evrc_encoder_uninit(ptr_pcm2evrc);
			ptr_pcm2evrc = nullptr;
		}
		
	}


	bool load_flag = true;

	bool loadevrclib()
	{
		if (NULL != hDll)
		{
			return true;
		}

		hDll = LoadLibrary(_T(".\\EvrcLib.dll"));
		if (NULL == hDll)
		{
			log("LoadLibrary NULL\n");
			return false;
		}

		my_evrc_get_stream_frame_count = (evrc_get_stream_frame_count)GetProcAddress(hDll, "evrc_get_stream_frame_count");
		my_evrc_packet_to_stream = (evrc_packet_to_stream)GetProcAddress(hDll, "evrc_packet_to_stream");
		my_evrc_stream_to_packet = (evrc_stream_to_packet)GetProcAddress(hDll, "evrc_stream_to_packet");

		isNull(my_evrc_get_stream_frame_count);
		isNull(my_evrc_packet_to_stream);
		isNull(my_evrc_stream_to_packet);


		my_evrc_encoder_init = (evrc_encoder_init)GetProcAddress(hDll, "evrc_encoder_init");
		my_evrc_encoder_uninit = (evrc_encoder_uninit)GetProcAddress(hDll, "evrc_encoder_uninit");
		my_evrc_encoder_encode_to_packet = (evrc_encoder_encode_to_packet)GetProcAddress(hDll, "evrc_encoder_encode_to_packet");
		my_evrc_encoder_encode_to_stream = (evrc_encoder_encode_to_stream)GetProcAddress(hDll, "evrc_encoder_encode_to_stream");
		my_evrc_encoder_max_encode = (evrc_encoder_max_encode)GetProcAddress(hDll, "evrc_encoder_max_encode");

		isNull(my_evrc_encoder_init);
		isNull(my_evrc_encoder_uninit);
		isNull(my_evrc_encoder_encode_to_packet);
		isNull(my_evrc_encoder_encode_to_stream);
		isNull(my_evrc_encoder_max_encode);

		my_evrc_decoder_init = (evrc_decoder_init)GetProcAddress(hDll, "evrc_decoder_init");
		my_evrc_decoder_uninit = (evrc_decoder_uninit)GetProcAddress(hDll, "evrc_decoder_uninit");
		my_evrc_decoder_decode_from_packet = (evrc_decoder_decode_from_packet)GetProcAddress(hDll, "evrc_decoder_decode_from_packet");
		my_evrc_decoder_decode_from_stream = (evrc_decoder_decode_from_stream)GetProcAddress(hDll, "evrc_decoder_decode_from_stream");
		my_evrc_decoder_decode_stream_frame = (evrc_decoder_decode_stream_frame)GetProcAddress(hDll, "evrc_decoder_decode_stream_frame");
		my_evrc_decoder_stream_max_sample = (evrc_decoder_stream_max_sample)GetProcAddress(hDll, "evrc_decoder_stream_max_sample");
		my_evrc_decoder_stream_frame_bytes = (evrc_decoder_stream_frame_bytes)GetProcAddress(hDll, "evrc_decoder_stream_frame_bytes");
		my_evrc_decoder_is_silence_stream_frame = (evrc_decoder_is_silence_stream_frame)GetProcAddress(hDll, "evrc_decoder_is_silence_stream_frame");

		isNull(my_evrc_decoder_init);
		isNull(my_evrc_decoder_uninit);
		isNull(my_evrc_decoder_decode_from_packet);
		isNull(my_evrc_decoder_decode_from_stream);
		isNull(my_evrc_decoder_decode_stream_frame);
		isNull(my_evrc_decoder_stream_max_sample);
		isNull(my_evrc_decoder_stream_frame_bytes);
		isNull(my_evrc_decoder_is_silence_stream_frame);

		return load_flag;
	}



	void isNull(void* ptr)
	{
		if (NULL == ptr)
		{
			log(strcat_s(const_cast<char*>(typeid(ptr).name()), 50, "  null"));
			load_flag = false;
		}

	}





	int encodeBt(const unsigned char* raw, unsigned char* & outfilebuffer, const int len) {
		int i = 0;
		std::vector<unsigned char> pcm;
		while (i < len) {
			pcm.push_back(raw[i]);
			i++;
		}
		unsigned char* evrc = new unsigned char[1024 * 100];
		int r = my_evrc_encoder_encode_to_stream(ptr_pcm2evrc, (short*)&(pcm[0]), pcm.size() / sizeof(short), evrc, 1024 * 100);

		outfilebuffer = (unsigned char*)malloc(r);
		if (r > 0) {
			memcpy(outfilebuffer, (char*)evrc, r);
		}
		delete[] evrc;
		return r;
	}


	int encodeBtAll(const unsigned char* raw, unsigned char* &outfilebuffer, int len) {
		int i = 0;
		std::vector<unsigned char> pcm;
		while (i < len) {
			pcm.push_back(raw[i]);
			i++;
		}

		void* ct = my_evrc_encoder_init(4, 4, 1);
		unsigned char* evrc = new unsigned char[1024 * 100];
		int r = my_evrc_encoder_encode_to_stream(ct, (short*)&(pcm[0]), pcm.size() / sizeof(short), evrc, 1024 * 100);

		outfilebuffer = (unsigned char*)malloc(r);
		if (r > 0) {
			memcpy(outfilebuffer, (char*)evrc, r);
		}
		my_evrc_encoder_uninit(ct);
		delete[] evrc;
		return r;
	}



	int decodeBt(const unsigned char* raw, unsigned char* &outfilebuffer, int len) {
		int i = 0;
		std::vector<unsigned char> bits;
		while (i < len) {
			bits.push_back(raw[i]);
			i++;
		}
		unsigned char* pcm = new unsigned char[1024 * 100];
		int bytes = 0;
		if (!bits.empty()) {
			const int words = my_evrc_decoder_stream_max_sample(&(bits[0]), bits.size());
			short* pcm_buf = new short[words];

			bytes = my_evrc_decoder_decode_from_stream(ptr_evrc2pcm, &(bits[0]), bits.size(), pcm_buf, words);
			outfilebuffer = (unsigned char*)malloc(bytes);
			if (bytes > 0) {
				memcpy(outfilebuffer, (char*)pcm_buf, bytes);
			}
			delete[] pcm_buf;
		}
		delete [] pcm;
		return bytes;
	}






