#include "AutoStart.h"
#include <tchar.h>
#include <iostream>
using namespace std;

AutoStart::AutoStart(void)
{
}


AutoStart::~AutoStart(void)
{
}

//���ÿ���������
void AutoStart::autoStart(WCHAR *value)
{
	WCHAR system[MAX_PATH];		//ϵͳĿ¼·��
	WCHAR filePath[MAX_PATH];	//Ҫ�������е��ļ�������·��
	WCHAR fileName[MAX_PATH];	//�ļ���(\CZPlayer.exe)

	//�õ���ǰִ���ļ���ȫ·��
	HMODULE hModule = GetModuleHandle(NULL);
	GetModuleFileName(hModule, filePath, sizeof(filePath));

	//�õ��ļ���
	for (int i = lstrlen(filePath) - 1; i >= 0; --i)
	{
		if (filePath[i] == '\\')
		{
			lstrcpy(fileName, &filePath[i]);
			break;
		}
	}

	//�õ�ϵͳ�ļ�����Ŀ¼��·������c:\windows\system32
	GetSystemDirectory(system, sizeof(system));

	//�γ�Ҫ���Ƶ���ȫ·������c:\windows\system32\CZPlayer.exe
	wcscat_s(system, fileName);

	//���Ҹ��Ƶ�Ŀ��·��
	CopyFile(filePath, system, false);

	//д��ע����Ա㿪���Զ�����
	HKEY hKey;

	//��ע���:·������
	//HEKY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Run
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_SET_VALUE, &hKey ) == ERROR_SUCCESS)
	{
		//����һ��ֵ,������������,ֵΪҪ�������е��ļ�������·��
		RegSetValueEx(hKey, value, 0, REG_SZ, (const unsigned char*)system, sizeof(system));
		//�ر�ע���:
		RegCloseKey(hKey);

		cout << "���ÿ����������ɹ���" << endl;
	}
	else
	{
		cout << "��ע���ʧ��" << endl;
	}
}

//ȡ������������
void AutoStart::cancelAutoStart(WCHAR *value)
{
	WCHAR system[MAX_PATH];		//ϵͳĿ¼·��
	WCHAR filePath[MAX_PATH];	//Ҫ�������е��ļ�������·��
	WCHAR fileName[MAX_PATH];	//�ļ���(\CZPlayer.exe)

	//�õ���ǰִ���ļ���ȫ·��
	HMODULE hModule = GetModuleHandle(NULL);
	GetModuleFileName(hModule, filePath, sizeof(filePath));

	//�õ��ļ���
	for (int i = lstrlen(filePath) - 1; i >= 0; --i)
	{
		if (filePath[i] == '\\')
		{
			lstrcpy(fileName, &filePath[i]);
			break;
		}
	}

	//�õ�ϵͳ�ļ�����Ŀ¼��·������c:\windows\system32
	GetSystemDirectory(system, sizeof(system));

	//�γ�Ҫ���Ƶ���ȫ·������c:\windows\system32\CZPlayer.exe
	wcscat_s(system, fileName);

	HKEY hKey;
	DeleteFile(filePath);	//ɾ����Ӧ���ļ�
	//��ע���:·������
	//HEKY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Run
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		//ɾ����ֵ
		RegDeleteValue(hKey, value);
		//�ر�ע���
		RegCloseKey(hKey);

		cout << "ȡ�������������ɹ���" << endl;
	}
	else
	{
		cout << "��ע���ʧ��" << endl;
	}
}

//�жϲ������Ƿ�������
bool AutoStart::isAutoStart(WCHAR *value)
{
	WCHAR system[MAX_PATH];		//ϵͳĿ¼·��
	WCHAR filePath[MAX_PATH];	//Ҫ�������е��ļ�������·��
	WCHAR fileName[MAX_PATH];	//�ļ���(\CZPlayer.exe)

	//�õ���ǰִ���ļ���ȫ·��
	HMODULE hModule = GetModuleHandle(NULL);
	GetModuleFileName(hModule, filePath, sizeof(filePath));

	//�õ��ļ���
	for (int i = lstrlen(filePath) - 1; i >= 0; --i)
	{
		if (filePath[i] == '\\')
		{
			lstrcpy(fileName, &filePath[i]);
			break;
		}
	}

	//�õ�ϵͳ�ļ�����Ŀ¼��·������c:\windows\system32
	GetSystemDirectory(system, sizeof(system));

	//�γ�Ҫ���Ƶ���ȫ·������c:\windows\system32\CZPlayer.exe
	wcscat_s(system, fileName);

	HKEY hKey;
	bool bIsAutoStart =false;
	//��ע���:·������
	//HEKY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Run
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		WCHAR wData[MAX_PATH];
		DWORD dwLen = MAX_PATH * sizeof(WCHAR);
		if (RegQueryValueEx(hKey, value, 0, 0, (BYTE*)wData, &dwLen) == ERROR_SUCCESS)
		{
			bIsAutoStart =  true;				//�ó���������������
		}
		else
		{
			bIsAutoStart =  false;				//�ó���û������������
		}
	}
	else
	{
		cout << "��ע���ʧ��" << endl;
	}

	RegCloseKey(hKey);
	return bIsAutoStart;
}
