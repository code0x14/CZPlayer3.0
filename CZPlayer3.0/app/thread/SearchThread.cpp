#include "SearchThread.h"
#include <parser.h>

SearchThread::SearchThread(QTableWidget *searchList )
	: m_searchList(searchList), m_nMusicNum(0), m_strUrl(""), m_nTimes(0)
{

}

SearchThread::~SearchThread(void)
{
}

void SearchThread::run()
{
}

void SearchThread::startFunc()
{
	m_strUrl = "http://www.xiami.com/app/nineteen/search/key/" + m_strKey + "/page/" + QString::number(m_nStartPage);
	qDebug() << m_strUrl;

	searchManager = new QNetworkAccessManager;
	searchManager->get(QNetworkRequest(QUrl(m_strUrl)));//�õ�url
	//�ź����
	connect(searchManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_LoadFinished(QNetworkReply*)));
}

//�������
void SearchThread::slot_LoadFinished( QNetworkReply *replay )
{
	QTextCodec *codec = QTextCodec::codecForName("utf8");		//ת����utf8�����ʽ
	QString searchStr = codec->toUnicode(replay->readAll());
	if (searchStr == "")
	{
		emit sig_SearchTimeout();	//������ʱ
		return;
	}
	searchStr = QUrl::fromPercentEncoding(searchStr.toAscii());	//�ٷֱȱ���

	//����Json
	QJson::Parser parser;
	bool ok;
	QVariantMap result = parser.parse(searchStr.toUtf8(), &ok).toMap();
	if (!ok)
	{
		qDebug() << "ת����QVariantMapʧ��";
		return;
	}

	//������������
	m_nMusicNum = result["total"].toInt();
	emit sig_SearchNum(m_nMusicNum);

	//�õ��������
	QVariantList resultList = result["results"].toList();
	foreach (QVariant var, resultList)
	{
		QVariantMap resultMap = var.toMap();	//�õ�ÿһ���map

		//������
		QCheckBox *musicNameCheckBox = new QCheckBox(resultMap["song_name"].toString());
		musicNameCheckBox->setObjectName(tr("musicNameCheckBox"));
		musicNameCheckBox->setToolTip(resultMap["song_name"].toString());
		musicNameCheckBox->setFont(QFont("΢���ź�", 10));
		musicNameCheckBox->setStyleSheet("QCheckBox{color:white;}"
			"QCheckBox::indicator{width:10px;height:10px;border: 1px solid white;border-radius:2px}"
			"QCheckBox::indicator:checked {image: url(:/app/images/checked2.png);}");

		//����
		QTableWidgetItem *artistItem = new QTableWidgetItem(resultMap["artist_name"].toString());
		artistItem->setTextAlignment(Qt::AlignCenter);
		artistItem->setToolTip(resultMap["artist_name"].toString());
		artistItem->setFont(QFont("΢���ź�", 10));

		//ר��
		QTableWidgetItem *albumItem = new QTableWidgetItem("��" + resultMap["album_name"].toString() + "��");
		albumItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		albumItem->setToolTip("��" + resultMap["album_name"].toString() + "��");
		albumItem->setFont(QFont("΢���ź�", 10));

		//���벥���б�
		int currentRows = m_searchList->rowCount();//�����б��е�����
		m_searchList->insertRow(currentRows);//�Ӳ����б��еĵ�ǰ�в���
		m_searchList->setCellWidget(currentRows, 0, musicNameCheckBox);
		m_searchList->setItem(currentRows, 1, artistItem);
		m_searchList->setItem(currentRows, 2, albumItem);
	}

	++m_nTimes;
	if (m_nTimes >= 2)
	{
		emit sig_LoadFinished();	//�������
		return;
	}
	++m_nStartPage;
	if ((m_nStartPage - 1) * 8 >= m_nMusicNum)//�Ѿ����ص��������ڵ���������������������
	{
		emit sig_LoadFinished();	//�������
		return;
	}
	this->startFunc();				//�ڶ��μ���
}

void SearchThread::setParam( QString strKey, int nStartPage )
{
	m_strKey = strKey;
	m_nStartPage = nStartPage;
	m_nMusicNum = 0;
	m_strUrl = "";
	m_nTimes = 0;
}