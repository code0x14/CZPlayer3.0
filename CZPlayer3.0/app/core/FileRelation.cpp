#include "FileRelation.h"
#include <tchar.h>

FileRelation::FileRelation(void)
{
}


FileRelation::~FileRelation(void)
{
}

// ע���ļ�����  
// wExt: Ҫ������չ��(����: ".wav")  
// wAppPath: Ҫ������Ӧ�ó�����(����: "C:/MyApp/MyApp.exe")  
// wAppKey: wExt��չ����ע����еļ�ֵ(����: "CZPlayer2.WAV")  
// wDefaultIcon: ��չ��ΪwAppPath��ͼ���ļ�(����: "D:/visual studio 2010 Projects/spectrum/debug/spectrum.exe,0")  
// wDescribe: �ļ��������� 
void FileRelation::registerFileRelation( WCHAR *wExt, WCHAR *wAppPath, WCHAR *wAppKey, 
	WCHAR *wDefaultIcon, WCHAR *wDescribe )
{
	WCHAR wTemp[MAX_PATH];  
	HKEY hKey;  

	//������Ϊ.wav,��������ֵΪCZPlayer2.WAV
	RegCreateKey(HKEY_CLASSES_ROOT, wExt, &hKey);							//.wav
	RegSetValue(hKey, _T(""), REG_SZ, wAppKey, lstrlen(wAppKey) + 1);		//CZPlayer2.WAV
	RegCloseKey(hKey);  

	//������ΪCZPlayer2.WAV,��������ֵΪWAV�ļ�
	RegCreateKey(HKEY_CLASSES_ROOT, wAppKey, &hKey);						 //CZPlayer2.WAV
	RegSetValue(hKey, _T(""), REG_SZ, wDescribe, lstrlen(wDescribe) + 1);	 //WAV�ļ�
	RegCloseKey(hKey);  

	//����Ĭ��ͼ��(����:D:/visual studio 2010 Projects/spectrum/debug/spectrum.exe,0)
	wsprintf(wTemp, _T("%s\\DefaultIcon"), wAppKey);						 				
	RegCreateKey(HKEY_CLASSES_ROOT, wTemp, &hKey);  
	RegSetValue(hKey, _T(""), REG_SZ, wDefaultIcon, lstrlen(wDefaultIcon) + 1);  
	RegCloseKey(hKey);  

	//SHCNE_ASSOCCHANGED:�޸��ļ�����   
	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);//�þ�Ϊˢ��ϵͳ����

	wsprintf(wTemp, _T("%s\\Shell\\Open"), wAppKey);  
	RegCreateKey(HKEY_CLASSES_ROOT, wTemp, &hKey);  
	RegSetValue(hKey, _T(""), REG_SZ, _T("ʹ�� CZPlayer ����(&O)"), strlen("ʹ�� CZPlayer ����(&O)") + 1);  
	RegCloseKey(hKey);  

	//���ù�������(����:"D:/visual studio 2010 Projects/spectrum/debug/spectrum.exe" "%1")
	wsprintf(wTemp, _T("%s\\Shell\\Open\\Command"), wAppKey);  
	RegCreateKey(HKEY_CLASSES_ROOT, wTemp, &hKey);  
	wsprintf(wTemp ,_T("\"%s\" \"%%1\""), wAppPath);  
	RegSetValue(hKey, _T(""), REG_SZ, wTemp, lstrlen(wTemp) + 1);
	RegCloseKey(hKey);
}

//ȡ���ļ�����
void FileRelation::cancelFileRelation( WCHAR *wExt, WCHAR *wAppKey )
{
	WCHAR wTemp[MAX_PATH];  
	HKEY hKey;  

	//������Ϊ.wav,��������ֵΪCZPlayer2.WAV
	RegCreateKey(HKEY_CLASSES_ROOT, wExt, &hKey);		//.wav
	RegSetValue(hKey, _T(""), REG_SZ, _T(""), 2);		//CZPlayer2.WAV
	RegCloseKey(hKey);  

	//����Ĭ��ͼ��
	wsprintf(wTemp, _T("%s\\DefaultIcon"), wAppKey);						 				
	RegCreateKey(HKEY_CLASSES_ROOT, wTemp, &hKey);  
	RegSetValue(hKey, _T(""), REG_SZ, _T(""), 2 + 1);  
	RegCloseKey(hKey);  

	//SHCNE_ASSOCCHANGED:�޸��ļ�����   
	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);//�þ�Ϊˢ��ϵͳ���� 
}

// ����ļ��������  
// wExt: Ҫ������չ��(����: ".wav")  
// wAppKey: ExeName��չ����ע����еļ�ֵ(����: "CZPlayer2.0.WAV")  
// ����true: ��ʾ�ѹ�����false: ��ʾδ���� 
bool FileRelation::checkFileRelation( const WCHAR* wExt, const WCHAR *wAppKey )
{
	bool isRelation = false;
	HKEY hKey;

	if(RegOpenKey(HKEY_CLASSES_ROOT, wExt, &hKey)==ERROR_SUCCESS)
	{
		WCHAR wData[MAX_PATH];
		DWORD dwLen = MAX_PATH * sizeof(WCHAR);
		if (RegQueryValueEx(hKey, 0, 0, 0, (BYTE*)wData, &dwLen) == ERROR_SUCCESS)
		{
			if (lstrcmp(wData, wAppKey) == 0)
			{
				isRelation = true;
			}
		}

		RegCloseKey(hKey);
		return isRelation;
	}
	return isRelation;
}