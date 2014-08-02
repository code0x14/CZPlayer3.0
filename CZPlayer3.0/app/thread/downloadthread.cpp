#include <QtXML>
#include "downloadthread.h"
#include "static.h"
#include "czplayerconfig.h"

DownloadThread::DownloadThread(QString musicName, QString musicArtist, DownloadList *downloadList)
    : m_musicName(musicName), m_musicArtist(musicArtist), m_downloadList(downloadList)
{
    times = 0;
    currentTime = 0;

    //����һ����ʱ��
    timer = new QTimer;
    timer2 = new QTimer;//�ж������Ƿ�����
    time = new QTime;
    connect(timer, SIGNAL(timeout()), this, SLOT(slot_GetNetSpeed()));
    connect(timer2, SIGNAL(timeout()), this, SLOT(slot_NetWorkInterrupt()));

    QString urlStr = "http://box.zhangmen.baidu.com/x?op=12&count=1&title=" + m_musicName + "$$" + m_musicArtist + "$$$";
    manager = new QNetworkAccessManager;
    manager->get(QNetworkRequest(QUrl(urlStr)));//�õ�url
    //�ź����
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_GetMusicXML(QNetworkReply*)));
    connect(downloadList, SIGNAL(sig_DelCurrentRow(int)), this, SLOT(slot_DelCurrentThread(int)));//ɾ����ǰ��
    connect(downloadList, SIGNAL(sig_PauseDownload(int)), this, SLOT(slot_PauseMusicDownload(int)));//��ͣ����
}

DownloadThread::~DownloadThread()
{
}

void DownloadThread::run()
{
}

//�õ����֣�xml��ʽ��
void DownloadThread::slot_GetMusicXML(QNetworkReply *replay)
{
    QTextCodec *codec = QTextCodec::codecForName("utf8");//ת����utf8�����ʽ
    QString musicStr = codec->toUnicode(replay->readAll());

    //û�����ӵ�����
    if (musicStr == "")
    {
        QMessageBox::information(NULL, tr("��Ϣ"), tr("���س�ʱ����������������߱�������ǽ���ã�"), QMessageBox::Yes);
        QString musicTitle = m_musicArtist + "-" + m_musicName;
        emit sig_DelCurrentMusicMapItem(musicTitle);
        return;
    }

	QString errorStr;
	int errorLine;
	int errorColumn;

	QDomDocument doc;
	if (!doc.setContent(musicStr, false, &errorStr, &errorLine, &errorColumn))
	{
		qDebug() << "�ڵ�" << errorLine << "�У���" << errorColumn << "�У���ȡ�ַ�����doc�д���" << errorStr;
        QMessageBox::information(NULL, tr("��Ϣ"), tr("û���ҵ��ø�����"), QMessageBox::Yes);
		return;
	}

	QDomElement root = doc.documentElement();//��ȡ��Ԫ��
	if (root.tagName() != "result")
	{
		QMessageBox::information(NULL, tr("��Ϣ"), tr("û���ҵ��ø�����"), QMessageBox::Yes);
		return;
	}

	//��ȡ����url
	QString sFirstPartUrl;	//ǰ�벿��
	QString sLastPartUrl;	//��벿��
	QDomNodeList nodeList = root.childNodes();//���root�����ӽڵ���б�
	for (int i = 0; i < nodeList.count(); ++i)
	{
		QDomNode node = nodeList.at(i);
		if (!node.isNull())//����ڵ㲻Ϊ��
		{
			if (node.isElement())//����ڵ���Ԫ��
			{
				QDomElement element = node.toElement();//ת����Ԫ��
				if (element.tagName() == "count")
				{
					qDebug() << "element.text() = " << element.text();
					if (element.text() == "0")//����Ԫ���ı�
					{
						QMessageBox::information(NULL, tr("��Ϣ"), tr("û���ҵ��ø�����"), QMessageBox::Yes);
						return;
					}
				}
				else if (element.tagName() == "url")
				{
					QDomNodeList nodeList2 = element.childNodes();//���url�����ӽڵ���б�
					for (int i = 0; i < nodeList2.count(); ++i)
					{
						QDomNode node2 = nodeList2.at(i);
						if (!node2.isNull())//����ڵ㲻Ϊ��
						{
							if (node2.isElement())//����ڵ���Ԫ��
							{
								QDomElement element2 = node2.toElement();//ת����Ԫ��
								if (element2.tagName() == "encode")
								{
									//qDebug() << element2.text();
									sFirstPartUrl = element2.text();
								}
								else if (element2.tagName() == "decode")
								{
									//qDebug() << element2.text();
									sLastPartUrl = element2.text();
									break;
								}
							}
							else
							{
								QMessageBox::information(NULL, tr("��Ϣ"), tr("û���ҵ��ø�����"), QMessageBox::Yes);
								return;
							}
						}
						else
						{
							QMessageBox::information(NULL, tr("��Ϣ"), tr("û���ҵ��ø�����"), QMessageBox::Yes);
							return;
						}
					}
					break;
				}
			}
			else
			{
				QMessageBox::information(NULL, tr("��Ϣ"), tr("û���ҵ��ø�����"), QMessageBox::Yes);
				return;
			}
		}
		else
		{
			QMessageBox::information(NULL, tr("��Ϣ"), tr("û���ҵ��ø�����"), QMessageBox::Yes);
			return;
		}
	}

	//�ж��Ƿ����xcode����ֹ�������ش���
	if (!sLastPartUrl.contains("xcode"))
	{
		QMessageBox::information(NULL, tr("��Ϣ"), tr("û���ҵ��ø�����"), QMessageBox::Yes);
		return;
	}

    //����url
    QString musicUrl = sFirstPartUrl + sLastPartUrl;
	//qDebug() << musicUrl;
#ifdef _WIN32_WINNT
    QString WINPATH = CZPlayerConfig::getValue("MUSICDIR_WIN").toString();
    QString musicFilePath = WINPATH + "/" + m_musicArtist + " - " + m_musicName + ".mp3";
#else
    QString X11PATH = QDir::homePath() + CZPlayerConfig::getValue("MUSICDIR_X11").toString();
    QString musicFilePath = X11PATH + "/" + m_musicArtist + " - " + m_musicName + ".mp3";
#endif

    //���������б�
    int currentRows = m_downloadList->rowCount();//�����б��е�����
    m_downloadList->insertRow(currentRows);//�������б��еĵ�ǰ�в���

    //������
    progressBar = new QProgressBar;
    progressBar->setObjectName(tr("progressBar"));
    timer->start(1000);
    time->start();

    //��������
    musicTitleItem = new QTableWidgetItem;
    musicTitleItem->setText(m_musicArtist + "-" + m_musicName);
    musicTitleItem->setTextAlignment(Qt::AlignCenter);
    musicTitleItem->setToolTip(m_musicArtist + "-" + m_musicName);

    //����״̬
    musicStatusItem = new QTableWidgetItem;
    musicStatusItem->setTextAlignment(Qt::AlignCenter);
    musicStatusItem->setText(tr("��������"));

    //��С
    musicSizeItem = new QTableWidgetItem;
    musicSizeItem->setTextAlignment(Qt::AlignCenter);

    //����
    speedItem = new QTableWidgetItem;
    speedItem->setTextAlignment(Qt::AlignCenter);

    m_downloadList->setItem(currentRows, 0, musicTitleItem);
    m_downloadList->setItem(currentRows, 1, musicStatusItem);
    m_downloadList->setItem(currentRows, 2, musicSizeItem);
    m_downloadList->setItem(currentRows, 3, speedItem);
    m_downloadList->setCellWidget(currentRows, 4, progressBar);

    static int index = 0;
    threadMap.insert(make_pair(index, this));
    listMap.insert(make_pair(index, currentRows));
    //emit musicListMap(listMap);
    m_downloadList->setMusicListMap(listMap);
    ++index;

    this->getMusicFromURL(musicUrl, musicFilePath);
    replay->deleteLater();//���Ҫ�ͷ�reply����
}

//���ظ���
void DownloadThread::getMusicFromURL(const QUrl &url, const QString &filePath)
{
    musicFile.setFileName(filePath);
    musicFile.open(QIODevice::WriteOnly);

    musicManager = new QNetworkAccessManager;
    QEventLoop loop;
    musicReply = musicManager->get(QNetworkRequest(QUrl(url)));

    connect(musicReply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(slot_UpdateDataProgress(qint64, qint64)));
    connect(musicReply, SIGNAL(finished()), this, SLOT(slot_ReplayFinished()));
    connect(musicReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slot_ReplyError(QNetworkReply::NetworkError)));
    connect(musicReply, SIGNAL(readyRead()), this, SLOT(slot_GetMusic()));//ʵ�������ݷֶ����ر��棬
                                                                     //�������������������ٱ��棬Ҫ��ʡ�ܶ��ڴ�
    loop.exec();
}

//�õ������ļ�
void DownloadThread::slot_GetMusic()
{
    //�õ������ļ��Ĵ�С
    ++times;
    if (times == 1)
    {
        QVariant var = musicReply->header(QNetworkRequest::ContentLengthHeader);
        int size = var.toLongLong();
        double musicSizeD = (double) size / 1048576;
        QString musicSizeStr = QString::number(musicSizeD, 'f', 2) + "MB";
        musicSizeItem->setText(musicSizeStr);
    }

    musicFile.write(musicReply->readAll());

    if (timer2->isActive())
    {
        timer2->stop();
    }
    timer2->start(200000);//���20�붼��û�������ļ��������ж�
}

//���ظ������
void DownloadThread::slot_ReplayFinished()
{
    musicFile.flush();
    if (musicFile.isOpen())
    {
        musicFile.close();
    }
    musicReply->deleteLater();//���Ҫ�ͷ�reply����
    this->deleteLater();//�ͷŵ�ǰ�߳�
}

//ÿ��1�����һ������
void DownloadThread::slot_GetNetSpeed()
{
    QString speedStr = QString::number(speedD, 'f', 1) + "KB/S";
    speedItem->setText(speedStr);
}

//���½�����
void DownloadThread::slot_UpdateDataProgress(qint64 bytesRead, qint64 totalBytes)
{
    progressBar->setMaximum(totalBytes);//���ֵ
    progressBar->setValue(bytesRead);//��ǰֵ

//    ++times;
//    if (times == 1)
//    {
//        double musicSizeD = (double) totalBytes / 1048576;
//        QString musicSizeStr = QString::number(musicSizeD, 'f', 2) + "MB";
//        musicSizeItem->setText(musicSizeStr);
//    }

    //�õ���ǰʱ��
    currentTime = time->elapsed();
    //��������
    speedD = bytesRead / currentTime * 1000 / 1024;

    if (bytesRead == totalBytes)
    {
        timer->stop();
        speedItem->setText(tr("0.0KB/S"));
        musicStatusItem->setText(tr("���"));
    }
}

//ɾ���������صĸ���
void DownloadThread::slot_DelCurrentThread(int row)
{
    map<int, DownloadThread*>::iterator threadMapIter = threadMap.find(row);
    if (threadMapIter != threadMap.end())
    {
        DownloadThread *currentThread = threadMapIter->second;
        if (currentThread == this)
        {
            threadMap.erase(threadMapIter);
            musicManager->deleteLater();
            musicReply->deleteLater();
            //m_downloadList->removeRow(row);
            //m_downloadList->setMusicListMap(listMap);
            if (musicFile.isOpen())
            {
                musicFile.close();
            }
            this->deleteLater();

            map<int, int>::iterator listMapIter = listMap.find(row);
            if (listMapIter != listMap.end())
            {
                qDebug() << "row = " << row;
                listMap.erase(listMapIter);
                m_downloadList->setMusicListMap(listMap);
            }
        }
    }
}

//��ͣ����
void DownloadThread::slot_PauseMusicDownload( int row )
{
    map<int, DownloadThread*>::iterator threadMapIter = threadMap.find(row);
    if (threadMapIter != threadMap.end())
    {
        DownloadThread *currentThread = threadMapIter->second;
        if (currentThread == this)
        {
            if (musicReply->isReadable())
            {
                //musicReply->deleteLater();
                musicReply->abort();
                timer->stop();
                timer2->stop();
                speedItem->setText(tr("0.0KB/S"));
                musicStatusItem->setText(tr("��ͣ"));
            }
        }
    }
}

//���ش���
void DownloadThread::slot_ReplyError(QNetworkReply::NetworkError networkError)
{
    QMessageBox::information(NULL, tr("��Ϣ"), tr("���ش��󣬴�����룺%1").arg(networkError), QMessageBox::Yes);
	qApp->exit(-1);
	//if (musicFile.isOpen())
	//{
	//	musicFile.close();
	//}
 //   musicManager->deleteLater();
 //   musicReply->deleteLater();
 //   this->deleteLater();
}

//�����ж�
void DownloadThread::slot_NetWorkInterrupt()
{
    static int i = 0;
    ++i;
    if (i == 1)
    {
        timer->stop();//ֹͣ��������
        timer2->stop();
        speedItem->setText(tr("0.0KB/S"));
        musicStatusItem->setText(tr("�ж�"));
        QMessageBox::information(NULL, tr("��Ϣ"), tr("���������Ƿ����ӣ�"), QMessageBox::Yes);
    }
}




