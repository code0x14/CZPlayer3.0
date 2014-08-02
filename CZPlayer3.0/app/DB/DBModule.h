#ifndef DBMODULE_H
#define DBMODULE_H

#include "head.h"
#include "MusicListSaveFormatEntity.h"
#include "MusicInfoEntity.h"

class DBModule : public QObject
{
	Q_OBJECT

public:
	DBModule();
	~DBModule();

public:
	static void initDB();																			//��ʼ�����ݿ�
	static bool createConnection();																	//�������ݿ�
	static void createTable();																		//������
	static bool readFilePath(QStringList &list);													//�����ݿ��ж���������ʾ���������б���
	static bool readMusicListSaveFormatEntity(vector<MusicListSaveFormatEntity> &vec);				//�����ݿ��ж���������ʾ��m3u�ļ���
	static bool readMusicInfoEntity(vector<MusicInfoEntity> &vec);									//�����ݿ��ȡ������Ϣ
	static bool clearDB();																			//������ݿ�
	static bool delLine(const QString &fileName);													//ɾ��һ����¼
	static bool insertLine(const QString &fileName, const QString &time, const QString &filePath);	//����һ�м�¼
	static bool updateAlbumUrlPath(const QString &fileName, const QString &albumUrlPath);			//����ר��url
	static bool readAlbumUrlPathFromDB(QString &fileName, QString &albumUrlPath);					//�����ݿ��ж�����������Ӧ��ר��url
};

#endif //DBMODULE_H