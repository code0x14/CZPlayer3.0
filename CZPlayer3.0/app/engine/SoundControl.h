#ifndef SOUNDCONTROL_H   
#define SOUNDCONTROL_H   

//����������
class SoundControl
{
public:
	SoundControl();
	~SoundControl();

public:
	//�����ĸ�����������������Լ�����   
	static bool SetVolume(long dev, long vol);	//�����豸������   
	static unsigned GetVolume(int dev);			//�õ��豸������dev=0��������1WAVE   ,2MIDI   ,3   LINE   IN   
	static bool SetMute(long dev, bool vol);	//�����豸����   
	static bool GetMute(long dev);  

private:
	static bool GetVolumeControl(HMIXER hmixer , long componentType, long ctrlType, MIXERCONTROL *mxc);   
	static long GetMuteValue(HMIXER hmixer, MIXERCONTROL *mxc);   
	static unsigned GetVolumeValue(HMIXER hmixer, MIXERCONTROL *mxc);   
	static bool SetVolumeValue(HMIXER hmixer, MIXERCONTROL *mxc, long volume);   
	static bool SetMuteValue(HMIXER hmixer, MIXERCONTROL *mxc, bool mute);  
};
 
#endif