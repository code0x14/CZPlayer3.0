#pragma once

#include <Shlobj.h>
#include <Windows.h>

//�ļ�������
class FileRelation
{
public:
	FileRelation(void);
	~FileRelation(void);

public:
	static void registerFileRelation(WCHAR *wExt, WCHAR *wAppPath, WCHAR *wAppKey, 
		WCHAR *wDefaultIcon, WCHAR *wDescribe);		//ע���ļ�����

	static void cancelFileRelation(WCHAR *wExt, WCHAR *wAppKey);	//ȡ���ļ�����

	static bool checkFileRelation(const WCHAR* wExt, const WCHAR *wAppKey);		//�ж��ļ�����
};

