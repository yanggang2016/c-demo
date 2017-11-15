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
			printf("\n�豸�Ѿ���...\n");
			break;

		case WOM_DONE:
			printf("\n������%d�Ѳ������...\n", ((LPWAVEHDR)dwParam1)->dwUser);
			//waveInAddBuffer(hwaveout, (LPWAVEHDR)dwParam1, sizeof(WAVEHDR));
			//fwrite(waveH->lpData, 1, waveH->dwBytesRecorded, fpInPCM);   //д������
			//printf("Capture:[%d]\n", waveH->dwBytesRecorded);
			//waveInAddBuffer(hwi, waveH, sizeof(WAVEHDR));              //buffer���·���ɼ�����
			break;

		case WOM_CLOSE:
			printf("\n�豸�Ѿ��ر�...\n");
			break;
		default:
			break;
		}
		return 0;
	}


	void play_wave()
	{
#define MUTE_LENGTH  128

		HWAVEOUT hWaveOut;//����豸
		WAVEFORMATEX waveform;//�ɼ���Ƶ�ĸ�ʽ���ṹ��
		WAVEHDR wHdr;//�ɼ���Ƶʱ�������ݻ���Ľṹ��
		char mute[MUTE_LENGTH];//�������Ŵ�

		WaveInitFormat(&waveform, MY_CHANNEL, MY_SAMPLE_RATE, MY_BITS_PER_SAMPLE);

		FillMemory(mute, MUTE_LENGTH, (BYTE)0xFE);
		//�þ����������.
		//pcm��ʾ����ʱ���źţ����𶯲�������������Ϊ��������һ����ʾ��Ϊ0�������Ϊ1ͬ��Ҳ�Ǿ���������ֻҪ��ÿ������ֵû�б仯���Ǿ��� 

		DWORD bufsize = 1024 * 1024;//ÿ�ο���10k�Ļ���洢¼������

									// Device   
		int nReturn = waveOutGetNumDevs();
		printf("\n����豸��Ŀ��%d\n", nReturn);
		for (int i = 0; i<nReturn; i++)
		{
			WAVEOUTCAPS woc;
			waveOutGetDevCaps(i, &woc, sizeof(WAVEOUTCAPS));
			printf("#%d\t�豸����%s\n", i, woc.szPname);
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
