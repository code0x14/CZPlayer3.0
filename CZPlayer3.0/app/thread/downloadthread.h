#ifndef DOWNLOADTHREAD_H
#define DOWNLOADTHREAD_H

#include "downloadlist.h"
#include "head.h"

//���ظ����߳�
class DownloadThread : public QThread
{
    Q_OBJECT

public:
    DownloadThread(QString musicName,
                   QString musicArtist,
                   DownloadList *downloadList);
    ~DownloadThread();

protected:
    virtual void run();

signals:
    void sig_DelCurrentMusicMapItem(QString musicTitle);//ɾ����ǰ����mapIter
    //void musicListMap(map<int, int> listMap);

private slots:
    void slot_GetMusicXML(QNetworkReply *replay);//�õ�����XML��ʽ
    void slot_GetMusic();//�õ�����
    void slot_ReplayFinished();//�������
    void slot_UpdateDataProgress(qint64,qint64);//���½�����
    void slot_GetNetSpeed();//�õ�����
    void slot_DelCurrentThread(int row);//ɾ����ǰ�߳�
    void slot_PauseMusicDownload(int row);
    void slot_ReplyError(QNetworkReply::NetworkError networkError);//���ش���
    void slot_NetWorkInterrupt();//�����ж�

private:
    void getMusicFromURL(const QUrl &url, const QString &filePath);

    QNetworkAccessManager *manager;
    QNetworkAccessManager *musicManager;
    QNetworkReply *musicReply;

    QFile musicFile;
    QTimer *timer;
    QTimer *timer2;
    QTime *time;

    QString m_musicName;
    QString m_musicArtist;
    QString m_musicFileName;
    DownloadList *m_downloadList;

    QProgressBar *progressBar;
    QTableWidgetItem *musicTitleItem;
    QTableWidgetItem *musicStatusItem;
    QTableWidgetItem *musicSizeItem;
    QTableWidgetItem *speedItem;
    map<int, DownloadThread*> threadMap;
    map<int, int> listMap;
    //QMultiMap<int, DownloadThread*> threadMap;

    int times;
    double speedD;
    int currentTime;
};

#endif // DOWNLOADTHREAD_H
