#pragma once

#include "head.h"
#include "MusicInfoEntity.h"

//���ظ����߳�
class LoadMusicThread : public QThread
{
	Q_OBJECT

public:
	LoadMusicThread(void);
	~LoadMusicThread(void);

public:
	//�������ļ� ���ò���
	void setParam(const QStringList &musicList, map<QString, int> *mapMusicRows, map<QString, QString> *mapMusicFilePath, QTableWidget *tableWidget);
	//�϶������ļ������ָ�ʽ�ļ� ���ò���
	void setParam(const QList<QUrl> &urls, map<QString, int> *mapMusicRows, map<QString, QString> *mapMusicFilePath, QTableWidget *tableWidget);
	//�������ָ�ʽ�ļ����ļ��� ���ò���
	void setParam(const QString &strFileOrDir, map<QString, int> *mapMusicRows, map<QString, QString> *mapMusicFilePath, QTableWidget *tableWidget);
	//�����ݿ��ȡ�����ļ� ���ò���
	void setParam(map<QString, int> *mapMusicRows, map<QString, QString> *mapMusicFilePath, QTableWidget *tableWidget);

protected:
	virtual void run();

signals:
	void sig_LoadMusicListFinished();						//�����겥���б�
	void sig_LoadMusicListError();							//���ز����б����

private:
	void startFunc();
	void loadMusicList(const QString &filePath);			//���ظ����б�
	void loadMusicList(const QStringList &list);
	void loadMusicList(const MusicInfoEntity &entity);

	void openMusicModeLoad();								//�������ļ�����ģʽ
	void dragEnterMusicModeLoad();							//�϶������ļ������ָ�ʽ�ļ�����ģʽ
	void openMusicFormatFileLoad();							//�����ָ�ʽ�ļ�����ģʽ
	void readFromDBModeLoad();								//���ݿ��ȡ�����ļ�����ģʽ
	void openMusicDirectoryMode();							//�����ļ��м���ģʽ

private:
	QStringList m_MusicList;
	QList<QUrl> m_urls;
	QString m_strMusicFormatFile;
	QString m_strMusicDirectory;
	QTableWidget *m_TableWidget;

	map<QString, int> *m_mapMusicRows;						//key����������value���к�
	map<QString, QString> *m_mapMusicFilePath;				//key����������value������·��

	//���������б�ģʽ
	enum _LoadMusicMode
	{
		openMusic = 1,
		dragEnterMusic = 2,
		openMusicFormatFile = 3,
		readFromDB = 4,
		openDirectory = 5
	};

	_LoadMusicMode loadMusicMode;
};

