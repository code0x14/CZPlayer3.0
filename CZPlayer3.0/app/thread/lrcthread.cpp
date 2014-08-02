#include <QtXml>
#include "lrcthread.h"
#include "static.h"
#include "czplayerconfig.h"

LrcThread::LrcThread( QString musicName, QString musicArtist, QString lrcFileName, QLabel *lrc ) 
	: m_musicName(musicName), m_musicArtist(musicArtist), m_lrcFilePath(lrcFileName), m_lrc(lrc)
{
	this->startFunc();
}

LrcThread::~LrcThread()
{
}

void LrcThread::run()
{
}

//��ʼ����
void LrcThread::startFunc()
{
	QString urlStr = "http://box.zhangmen.baidu.com/x?op=12&count=1&title=" + m_musicName + "$$" + m_musicArtist + "$$$";
	lrcManager = new QNetworkAccessManager;
	lrcManager->get(QNetworkRequest(QUrl(urlStr)));//�õ�url
	//�ź����
	connect(lrcManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_GetMusicXML(QNetworkReply*)));
}

//�õ����֣�xml��ʽ��
void LrcThread::slot_GetMusicXML(QNetworkReply *replay)
{
	QTextCodec *codec = QTextCodec::codecForName("utf8");//ת����utf8�����ʽ
	QString musicStr = codec->toUnicode(replay->readAll());

	//û�����ӵ�����
	if (musicStr == "")
	{
		m_lrc->setText(m_musicArtist + " - " + m_musicName + tr("----�޷����ӵ�����!"));
		return;
	}

	QString errorStr;
	int errorLine;
	int errorColumn;

	QDomDocument doc;
	if (!doc.setContent(musicStr, false, &errorStr, &errorLine, &errorColumn))
	{
		qDebug() << "�ڵ�" << errorLine << "�У���" << errorColumn << "�У���ȡ�ַ�����doc�д���" << errorStr;
		m_lrc->setText(m_musicArtist + " - " + m_musicName + tr("----δ�ҵ�����ļ�!"));
		return;
	}

	QDomElement root = doc.documentElement();//��ȡ��Ԫ��
	if (root.tagName() != "result")
	{
		m_lrc->setText(m_musicArtist + " - " + m_musicName + tr("----δ�ҵ�����ļ�!"));
		return;
	}

	//��ȡlrcid
	QString sLrcID;
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
						m_lrc->setText(m_musicArtist + " - " + m_musicName + tr("----δ�ҵ�����ļ�!"));
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
								if (element2.tagName() == "lrcid")
								{
									sLrcID = element2.text();
									break;
								}
							}
							else
							{
								m_lrc->setText(m_musicArtist + " - " + m_musicName + tr("----δ�ҵ�����ļ�!"));
								return;
							}
						}
						else
						{
							m_lrc->setText(m_musicArtist + " - " + m_musicName + tr("----δ�ҵ�����ļ�!"));
							return;
						}
					}
					break;
				}
			}
			else
			{
				m_lrc->setText(m_musicArtist + " - " + m_musicName + tr("----δ�ҵ�����ļ�!"));
				return;
			}
		}
		else
		{
			m_lrc->setText(m_musicArtist + " - " + m_musicName + tr("----δ�ҵ�����ļ�!"));
			return;
		}
	}

	//�õ���������ļ���
	int nFolder = sLrcID.toInt() / 100;
	QString sFolder = QString::number(nFolder);
	//���url
	QString lrcUrl = "http://box.zhangmen.baidu.com/bdlrc/" + sFolder + "/" + sLrcID + ".lrc";

	this->getLrcFromURL(lrcUrl);
	replay->deleteLater();//���Ҫ�ͷ�reply����
}

//���ظ��
void LrcThread::getLrcFromURL(const QUrl &url)
{
	lrcFile.setFileName(m_lrcFilePath);
	lrcFile.open(QIODevice::WriteOnly);

	lrcManager2 = new QNetworkAccessManager;
	lrcManager2->get(QNetworkRequest(QUrl(url)));

	connect(lrcManager2, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_GetLrc(QNetworkReply*)));
}

//�õ�����ļ�
void LrcThread::slot_GetLrc(QNetworkReply *replay)
{
	lrcFile.write(replay->readAll());
	lrcFile.flush();
	if (lrcFile.isOpen())
	{
		lrcFile.close();
	}

	emit sig_ResolveLrc();		//������ǰ���ظ��
	replay->deleteLater();		//���Ҫ�ͷ�reply����
	this->deleteLater();		//�ͷŵ�ǰ�߳�
}







