#ifndef LRCTHREAD_H
#define LRCTHREAD_H

#include "head.h"

//�����ʾ�߳�
class LrcThread : public QThread
{
    Q_OBJECT

public:
    LrcThread(QString musicName, QString musicArtist, QString lrcFileName, QLabel *lrc);
    ~LrcThread();

protected:
    virtual void run();

signals:
	void sig_ResolveLrc();//��������ź�

private slots:
	void slot_GetMusicXML(QNetworkReply *replay);//�õ�MusicXML
	void slot_GetLrc(QNetworkReply *replay);//�õ����

private:
	void startFunc();
	void getLrcFromURL(const QUrl &url);

private:
	QFile lrcFile;
	QString m_musicName;
	QString m_musicArtist;
	QString m_lrcFilePath;

	QLabel *m_lrc;

	QNetworkAccessManager *lrcManager;
	QNetworkAccessManager *lrcManager2;
};

#endif // LRCTHREAD_H
