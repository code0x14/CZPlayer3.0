#include <parser.h>
#include "albumthread.h"
#include "static.h"
#include "czplayerconfig.h"

AlbumThread::AlbumThread(QString musicName, QString name, QString artist, QLabel *albumImage)
	: m_strMusicName(musicName), m_name(name), m_musicArtist(artist), m_albumImage(albumImage)
{
	//    QString urlStr = "http://geci.me/api/lyric/" + m_name + "/" + m_musicArtist;
	//    albumManager = new QNetworkAccessManager;
	//    albumManager->get(QNetworkRequest(QUrl(urlStr)));//�õ�url
	//    //�ź����
	//    connect(albumManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getMusicAlbumXML(QNetworkReply*)));
	this->startFunc();
}

AlbumThread::~AlbumThread()
{
}

void AlbumThread::run()
{
}

//��ʼ����
void AlbumThread::startFunc()
{
	QString urlStr = "http://geci.me/api/lyric/" + m_name + "/" + m_musicArtist;
	albumManager = new QNetworkAccessManager;
	albumManager->get(QNetworkRequest(QUrl(urlStr)));//�õ�url
	//�ź����
	connect(albumManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_GetMusicAlbumJson(QNetworkReply*)));
}

//�õ�����ר����json��ʽ��
void AlbumThread::slot_GetMusicAlbumJson(QNetworkReply *replay)
{
	QTextCodec *codec = QTextCodec::codecForName("utf8");//ת����utf8�����ʽ
	QString albumStr = codec->toUnicode(replay->readAll());

	//û�����ӵ�����
	if (albumStr == "")
	{
		qDebug() << "���س�ʱ����������������߱�������ǽ����";
		emit sig_AlbumDownloadFinished(m_strMusicName, "");	//ר���������
		m_albumImage->setPixmap(QPixmap(tr(":/app/images/albumImage.png")));
		return;
	}

	//����Json
	QJson::Parser parser;
	bool ok;
	QVariantMap result = parser.parse(albumStr.toUtf8(), &ok).toMap();
	if (!ok)
	{
		qDebug() << "ת����QVariantMapʧ��";
		emit sig_AlbumDownloadFinished(m_strMusicName, "");	//ר���������
		m_albumImage->setPixmap(QPixmap(tr(":/app/images/albumImage.png")));
		return;
	}

	//�ж�json�����Ƿ����url
	if (result["count"].toInt() == 0)
	{
		emit sig_AlbumDownloadFinished(m_strMusicName, "");	//ר���������
		m_albumImage->setPixmap(QPixmap(tr(":/app/images/albumImage.png")));
		qDebug() << "û�����ص�ר��";
		return;
	}

	//�õ�ר��aid
	QString sAid = "";
	QVariantList resultList = result["result"].toList();
	foreach (QVariant var, resultList)
	{
		QVariantMap resultMap = var.toMap();
		sAid = resultMap["aid"].toString();
		break;
	}

	QString urlStr = "http://geci.me/api/cover/" + sAid;
	//qDebug() << urlStr;
	albumManager2 = new QNetworkAccessManager;
	albumManager2->get(QNetworkRequest(QUrl(urlStr)));//�õ�url
	//�ź����
	connect(albumManager2, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_GetMusicAlbumJson2(QNetworkReply*)));
	replay->deleteLater();
}

//�õ�����ר����json��ʽ��
void AlbumThread::slot_GetMusicAlbumJson2(QNetworkReply *replay)
{
	QTextCodec *codec = QTextCodec::codecForName("utf8");//ת����utf8�����ʽ
	QString albumStr = codec->toUnicode(replay->readAll());

	//û�����ӵ�����
	if (albumStr == "")
	{
		qDebug() << "�޷����ӵ�����";
		emit sig_AlbumDownloadFinished(m_strMusicName, "");	//ר���������
		m_albumImage->setPixmap(QPixmap(tr(":/app/images/albumImage.png")));
		return;
	}

	//����Json
	QJson::Parser parser;
	bool ok;
	QVariantMap result = parser.parse(albumStr.toUtf8(), &ok).toMap();
	if (!ok)
	{
		qDebug() << "ת����QVariantMapʧ��";
		emit sig_AlbumDownloadFinished(m_strMusicName, "");	//ר���������
		m_albumImage->setPixmap(QPixmap(tr(":/app/images/albumImage.png")));
		return;
	}

	//    //�ж�xml�����Ƿ����url(url���󷵻�404ҳ��)
	//    QStringList countList = albumStr.split(", \"code\":");
	//    QString countStr = countList.at(0);
	//    QStringList countList2 = countStr.split(": ");
	//    QString countStr2 = countList2.at(1);

	//    if (countStr2 == "0")
	//    {
	//        qDebug() << "û���ҵ�ר��";
	//        return;
	//    }

	//�õ�ר��url
	QVariantMap resultMap = result["result"].toMap();
	QString sJpg = resultMap["cover"].toString();		//��׼ͼƬ
	//QString sJpg2 = resultMap["thumb"].toString();	//����ͼƬ

#ifdef _WIN32_WINNT
	QString albumImagesName = CZPlayerConfig::getValue("ALBUMDIR_WIN").toString();
#else
	QString albumImagesName = QDir::homePath() + CZPlayerConfig::getValue("ALBUMDIR_X11").toString();
#endif

	albumFilePath = albumImagesName + "/" + m_musicArtist + " - " + m_name + ".jpg";

	this->getAlbumFromURL(sJpg, albumFilePath);
	replay->deleteLater();//���Ҫ�ͷ�reply����
}

//����ר��ͼƬ
void AlbumThread::getAlbumFromURL(const QUrl &url, const QString &filePath)
{
	albumFile.setFileName(filePath);
	albumFile.open(QIODevice::WriteOnly);
	m_strAlbumUrl = url.toString();		//����ר��url

	albumManager3 = new QNetworkAccessManager;
	QEventLoop loop;
	albumReply = albumManager3->get(QNetworkRequest(QUrl(url)));

	connect(albumReply, SIGNAL(finished()), this, SLOT(slot_ReplayFinished()));
	connect(albumReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slot_ReplyError(QNetworkReply::NetworkError)));
	connect(albumReply, SIGNAL(readyRead()), this, SLOT(slot_GetAlbum()));//ʵ�������ݷֶ����ر��棬
	//�������������������ٱ��棬Ҫ��ʡ�ܶ��ڴ�
	loop.exec();
}

//�õ�ר���ļ�
void AlbumThread::slot_GetAlbum()
{
	albumFile.write(albumReply->readAll());
}

//����ר���ļ����
void AlbumThread::slot_ReplayFinished()
{
	albumFile.flush();
	if (albumFile.isOpen())
	{
		albumFile.close();
	}

	emit sig_AlbumDownloadFinished(m_strMusicName, m_strAlbumUrl);	//ר���������

	//m_albumImage->setPixmap(QPixmap(albumFilePath));
	//��ͼƬת����label��С����ʾ
	QImage image(albumFilePath);
	QImage newImg = image.scaled(m_albumImage->width(), m_albumImage->height());
	m_albumImage->setPixmap(QPixmap::fromImage(newImg));

	albumReply->deleteLater();//���Ҫ�ͷ�reply����
	this->deleteLater();//�ͷŵ�ǰ�߳�
}

//���ش���
void AlbumThread::slot_ReplyError(QNetworkReply::NetworkError networkError)
{
	if (networkError != QNetworkReply::NoError)
	{
		this->startFunc();//���¿�ʼ����
		m_albumImage->setPixmap(QPixmap(tr(":/app/images/albumImage.png")));
		QMessageBox::information(NULL, tr("��Ϣ"), tr("����ר������"), QMessageBox::Yes);
	}

	//    albumManager3->deleteLater();
	//    albumReply->deleteLater();
	//    if (albumFile.isOpen())
	//    {
	//        albumFile.close();
	//    }
	//    this->deleteLater();
	qDebug() << networkError;
}







