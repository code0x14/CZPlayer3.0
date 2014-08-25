#include "stdafx.h"

#include "AudioStream.h"
#include "FastFourierTransform.h"
#include "CriticalSection.h"
#include "IO.h"
#include "Stream.h"
#include "Thread.h"
#include "Utils.h"
#include "signal_slot.h"

#include <vector>
using namespace std;

#ifndef INCLUDE_BASICPLAYER
#define INCLUDE_BASICPLAYER

// pre-defined class
class CBasicPlayer;
class CSpectrumAnalyser;

/* constants used in direct sound */
#define DEFAULT_BUFFER_SIZE 88200
#define DEFAULT_SAMPLE_RATE 44100.0F
#define DEFAULT_FRAME_SIZE	4
#define DEFAULT_BITS_PER_SAMPLE	16
#define DEFAULT_CHANNELS	2
#define DEFAULT_SECONDS		2
#define DEFAULT_DS_BUFFER_SIZE	DEFAULT_SECONDS*DEFAULT_SAMPLE_RATE*(DEFAULT_BITS_PER_SAMPLE>>3)*DEFAULT_CHANNELS

/* constants used in digital signal process */
#define DEFAULT_WIDTH 300
#define DEFAULT_HEIGHT 180
#define DEFAULT_SAMPLE_SIZE 2048
#define DEFAULT_FPS 30
#define DEFAULT_SPECTRUM_ANALYSER_FFT_SAMPLE_SIZE 512
#define DEFAULT_SPECTRUM_ANALYSER_BAND_COUNT 30				//Ƶ��bar����
#define DEFAULT_SPECTRUM_ANALYSER_DECAY 0.05F
#define DEFAULT_SPECTRUM_ANALYSER_PEAK_DELAY 5 /* the value is more lower, fall faster */
#define DEFAULT_SPECTRUM_ANALYSER_PEAK_DELAY_FPS_RATIO 0.4F
#define DEFAULT_SPECTRUM_ANALYSER_PEAK_DELAY_FPS_RATIO_RANGE 0.1F
#define MIN_SPECTRUM_ANALYSER_DECAY	0.02F
#define MAX_SPECTRUM_ANALYSER_DECAY 0.08F
#define SAMPLE_TYPE_EIGHT_BIT 1
#define SAMPLE_TYPE_SIXTEEN_BIT 2
#define CHANNEL_MODE_MONO 1
#define CHANNEL_MODE_STEREO 2

/************************************************************************/
/* CSystem                                                              */
/************************************************************************/
typedef __int64				jlong;
typedef unsigned int		juint;
typedef unsigned __int64	julong;
typedef long				jint;
typedef signed char			jbyte;

#define CONST64(x)				(x ## LL)
#define NANOS_PER_SEC			CONST64(1000000000)
#define NANOS_PER_MILLISEC		1000000

jlong as_long(LARGE_INTEGER x);
void set_high(jlong* value, jint high);
void set_low(jlong* value, jint low);

class CSystem
{
private:
	static jlong frequency;
	static int ready;

	static void init()
	{
		LARGE_INTEGER liFrequency = {0};
		QueryPerformanceFrequency(&liFrequency);
		frequency = as_long(liFrequency);
		ready = 1;
	}
public:
	static jlong nanoTime()
	{
		if(ready != 1)
			init();

		LARGE_INTEGER liCounter = {0};
		QueryPerformanceCounter(&liCounter);
		double current = as_long(liCounter);
		double freq = frequency;
		return (jlong)((current / freq) * NANOS_PER_SEC);
	}
};

/************************************************************************/
/* CPlayThread                                                          */
/************************************************************************/
class CPlayThread : public CThread
{
public:
	CPlayThread(CBasicPlayer* pPlayer);
	~CPlayThread(void);

protected:
	void Execute();					//ִ��

c_signals:
	Signal sig_Finished;			//��������ź�

private:
	CBasicPlayer* m_Player;
	CCriticalSection* m_CriticalSection;
};

/************************************************************************/
/* CSpectrumAnalyserThread                                              */
/************************************************************************/
class CSpectrumAnalyserThread : public CThread
{
public:
	CSpectrumAnalyserThread(CSpectrumAnalyser* pSpectrumAnalyser);
	~CSpectrumAnalyserThread(void);

private:
	CBasicPlayer* m_Player;
	CSpectrumAnalyser* m_SpectrumAnalyser;
	CCriticalSection* m_CriticalSection;
	bool m_process;
	jlong m_lfp;
	int m_frameSize;

private:
	int calculateSamplePosition();
	void processSamples(int pPosition);

protected:
	void Execute();
};

/************************************************************************/
/* CSpectrumAnalyser��Ƶ�׷�����                                        */
/************************************************************************/
class CSpectrumAnalyser
{
friend class CSpectrumAnalyserThread;

public:
	CSpectrumAnalyser(CBasicPlayer* pPlayer);
	~CSpectrumAnalyser(void);

public:
	void Start();
	void Stop();
	void Process(float pFrameRateRatioHint);

	jbyte* GetAudioDataBuffer() { return m_AudioDataBuffer; }

	int GetPosition() { return m_position; }
	void SetPosition(int pPosition) { m_position = pPosition; }

	DWORD GetAudioDataBufferLength() { return m_AudioDataBufferLength; }
	void SetAudioDataBufferLength(DWORD pAudioDataBufferLength) { m_AudioDataBufferLength = pAudioDataBufferLength; }

c_signals:
	Signal1<vector<float>> sig_spectrumChanged;

private:
	CBasicPlayer* m_Player;
	CSpectrumAnalyserThread* m_SpectrumAnalyserThread;	//Ƶ�׷����߳�
	CFastFourierTransform* m_FFT;						//���ٸ���Ҷ�任
	vector<float> m_vecFrequency;						//Ƶ�ʼ���

	/* digital signal process */
	DWORD m_AudioDataBufferLength;
	jbyte* m_AudioDataBuffer;
	int m_SampleSize;
	LONG m_FpsAsNS;
	LONG m_DesiredFpsAsNS;
	float* m_Left;
	float* m_Right;
	int m_position;
	int m_offset;
	int m_sampleType;
	int m_channelMode;

	/* spectrum analyser */
	int m_width;
	int m_height;
	int* m_peaks;
	int* m_peaksDelay;
	float* m_oldFFT;
	int m_saFFTSampleSize;
	int m_saBands;
	float m_saMultiplier;
	float m_saDecay;
	int m_barOffset;
	int m_peakDelay;
	int m_winwidth, m_winheight;
};

/************************************************************************/
/* CBasicPlayer                                                         */
/************************************************************************/
class CBasicPlayer
{
//friend class declare
friend class CPlayThread;
friend class CSpectrumAnalyser;

public:
	CBasicPlayer(TCHAR* pszFileName);
	~CBasicPlayer(void);

public:
	bool IsPlaying() { return m_Playing; }												//�ж��Ƿ��ڲ���

	DWORD GetBufferSize() { return m_BufferSize; }										//��û�������С
	void SetBufferSize(DWORD pBufferSize) { m_BufferSize = pBufferSize; }				//���û�������С

	float GetSampleRate() { return m_SampleRate; }										//��ò�����
	void SetSampleRate(float pSampleRate) { m_SampleRate = pSampleRate; }				//���ò�����

	WORD GetFrameSize() { return m_FrameSize; }											//���֡��С
	void SetFrameSize(WORD pFrameSize) { m_FrameSize = pFrameSize; }					//����֡��С

	WORD GetBitsPerSample() { return m_BitPerSample; }									//���ÿ�������
	void SetBitsPerSample(WORD pBitsPerSample) { m_BitPerSample = pBitsPerSample; }		//����ÿ�������

	WORD GetChannels() { return m_Channels; }											//���ͨ����
	void SetChannels(WORD pChannels) { m_Channels = pChannels; }						//����ͨ����

	CPlayThread* GetPlayThread() { return m_PlayThread; }								//��ò����߳�ָ��
	DS_Info* GetDSInfo() { return m_info; }												//���DS��Ϣ
	long GetBytePosition() { return m_bytePosition; }									//��ö�����λ��
	CFileInput* GetInput() { return m_Input; }											//���Input�ļ���

	void Start();																		//��ʼ����
	void Stop();																		//ֹͣ����
	void Pause();																		//��ͣ
	void Play();																		//�ָ�����
	jlong GetLongFramePosition();														//��ȡ֡λ��

	void setCurrentPosition(DWORD dwPlayCursor)											//���ò���λ��
	{
		m_info->playBuffer->SetCurrentPosition(dwPlayCursor);
	}

c_signals:
	Signal1<vector<float>> sig_spectrumChanged;
	Signal sig_Finished;

private c_slots:
	void slot_spectrumChanged(vector<float> vecFrequency);
	void slot_Finished();

private:
	CPlayThread* m_PlayThread;															//�����߳�
	CSpectrumAnalyser* m_SpectrumAnalyser;												//Ƶ�׷�����
	CCriticalSection* m_CriticalSection;												//�ٽ���

	//�ļ�IO
	CFileInput* m_Input;
	CWmaInput* m_WmaInput;
	CMp3Input* m_Mp3Input;
	CWaveInput* m_WavInput;
	CVorbisInput* m_VorbisInput;

	bool m_Playing;					//�Ƿ��ڲ���

	DS_Info* m_info;
	volatile long m_bytePosition;

	/* the parameters to create direct sound */
	DWORD m_BufferSize;				//��������С
	float m_SampleRate;				//������
	WORD m_FrameSize;				//֡��
	WORD m_BitPerSample;			//ÿ�������
	WORD m_Channels;				//ͨ����

	//HWND m_hWnd;					//���ڿռ��������ڻ�Ƶ��

	//���ָ�ʽ
	enum _MusicFormat
	{
		mp3 = 1,
		wav = 2,
		wma = 3,
		ogg = 4
	};

	_MusicFormat musicFormat;
};

#endif