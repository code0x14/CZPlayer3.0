#ifndef CLIENTBASEOPERATING_H
#define CLIENTBASEOPERATING_H

#include "head.h"

class ClientBaseOperating : public QObject
{
	Q_OBJECT

public:
	ClientBaseOperating();
	~ClientBaseOperating();

public:
	static void initConfig();					//��ʼ������
	static void initConfigDialog();				//��ʼ�����öԻ���

	static void initLrcPropery();				//���ø������
	static void initHotKeyPropery();			//����ȫ���ȼ�
	static QString initPlayMode();				//��ʼ������ģʽ

private:
	static void playVoice();							//�����ʺ���
	static void touchConfigFile();						//�ж������ļ��Ƿ����
	static void mkdirDownloadDir();						//����һ�����ظ���Ŀ¼
	static void mkdirAlbumDir();						//����һ���ļ������洢���ص�ר��
	static void mkdirLrcDir();							//����һ�����ظ��Ŀ¼

	static void initGeneralSettings();			//��ʼ����������ҳ
	static void initDownloadConfig();			//��ʼ������ҳ
	static void initLrcConfig();				//��ʼ�����ҳ
	static void initHotKeyConfig();				//��ʼ��ȫ���ȼ�ҳ
};

#endif // CLIENTBASEOPERATING_H