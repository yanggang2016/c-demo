#include "stdafx.h"
#include "Player.h"
#include "Windows.h"
#include <iostream>
#include <mmsystem.h>  
#pragma comment(lib, "winmm.lib")
using namespace std;

#define MY_CHANNEL 1
#define MY_SAMPLE_RATE 8000
#define MY_BITS_PER_SAMPLE 16

Player::Player()
{
}


Player::~Player()
{
}
namespace asd
{
	void WaveInitFormat(LPWAVEFORMATEX m_WaveFormat, WORD nCh, DWORD nSampleRate, WORD BitsPerSample)
	{
		m_WaveFormat->wFormatTag = WAVE_FORMAT_PCM;
		m_WaveFormat->nChannels = nCh;
		m_WaveFormat->nSamplesPerSec = nSampleRate;
		m_WaveFormat->nAvgBytesPerSec = nSampleRate * nCh * BitsPerSample / 8;
		m_WaveFormat->nBlockAlign = m_WaveFormat->nChannels * BitsPerSample / 8;
		m_WaveFormat->wBitsPerSample = BitsPerSample;
		m_WaveFormat->cbSize = 0;
	}

	DWORD CALLBACK MicCallback(HWAVEIN hwaveout, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
	{
		switch (uMsg)
		{
		case WOM_OPEN:
			printf("\n设备已经打开...\n");
			break;

		case WOM_DONE:
			printf("\n缓冲区%d已播放完毕...\n", ((LPWAVEHDR)dwParam1)->dwUser);
			//waveInAddBuffer(hwaveout, (LPWAVEHDR)dwParam1, sizeof(WAVEHDR));
			//fwrite(waveH->lpData, 1, waveH->dwBytesRecorded, fpInPCM);   //写出数据
			//printf("Capture:[%d]\n", waveH->dwBytesRecorded);
			//waveInAddBuffer(hwi, waveH, sizeof(WAVEHDR));              //buffer重新放入采集队列
			break;

		case WOM_CLOSE:
			printf("\n设备已经关闭...\n");
			break;
		default:
			break;
		}
		return 0;
	}


	void play_wave()
	{
#define MUTE_LENGTH  128

		HWAVEOUT hWaveOut;//输出设备
		WAVEFORMATEX waveform;//采集音频的格式，结构体
		WAVEHDR wHdr;//采集音频时包含数据缓存的结构体
		char mute[MUTE_LENGTH];//静音符号串

		WaveInitFormat(&waveform, MY_CHANNEL, MY_SAMPLE_RATE, MY_BITS_PER_SAMPLE);

		FillMemory(mute, MUTE_LENGTH, (BYTE)0xFE);
		//用静音符号填充.
		//pcm表示的是时域信号，有震动才有声音，所以为静音并不一定表示都为0，如果都为1同样也是静音，所以只要是每个样本值没有变化都是静音 

		DWORD bufsize = 1024 * 1024;//每次开辟10k的缓存存储录音数据

									// Device   
		int nReturn = waveOutGetNumDevs();
		printf("\n输出设备数目：%d\n", nReturn);
		for (int i = 0; i<nReturn; i++)
		{
			WAVEOUTCAPS woc;
			waveOutGetDevCaps(i, &woc, sizeof(WAVEOUTCAPS));
			printf("#%d\t设备名：%s\n", i, woc.szPname);
		}
		wHdr.lpData = mute;
		wHdr.dwBufferLength = MUTE_LENGTH;
		wHdr.dwBytesRecorded = 0;
		wHdr.dwUser = 0;
		wHdr.dwFlags = 0;
		wHdr.dwLoops = 1;
		waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveform, (DWORD)MicCallback, 0, CALLBACK_FUNCTION);
		waveOutPrepareHeader(hWaveOut, &wHdr, sizeof(WAVEHDR));

		waveOutWrite(hWaveOut, &wHdr, sizeof(WAVEHDR));

		waveOutReset(hWaveOut);
		waveOutUnprepareHeader(hWaveOut, &wHdr, sizeof(WAVEHDR));
		waveOutClose(hWaveOut);


	}
}
