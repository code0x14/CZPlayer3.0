#pragma once

#include <Windows.h>

//������������
class AutoStart
{
public:
	AutoStart(void);
	~AutoStart(void);

public:
	static void autoStart(WCHAR *value);			//���ÿ���������
	static void cancelAutoStart(WCHAR *value);		//ȡ������������
	static bool isAutoStart(WCHAR *value);			//�жϳ����Ƿ�������
};

