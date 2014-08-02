#include "SharedDialog.h"
#include <tchar.h>

//ascii�ַ�ת����Unicode�ַ�
static int astr_ustr( char *ansistr, WCHAR *unicodestr )
{
	int result = 0;
	try
	{
		int needlen = MultiByteToWideChar(CP_ACP, 0, ansistr, -1, NULL, 0 );
		if( needlen < 0 )
		{
			return needlen;
		}

		result = MultiByteToWideChar(CP_ACP, 0, ansistr, -1, unicodestr, needlen);
		if( result < 0 )
		{
			return result;
		}
		return result;
	}
	catch( ... )
	{

	}
	return result;
}

SharedDialog::SharedDialog(QWidget *parent) : m_parent(parent)
{
	//���ô��ڻ�������
	this->resize(420, 280);//���ô����С
	this->setMinimumSize(420, 280);
	this->setMaximumSize(420, 280);
	this->setWindowTitle(tr("�������"));

	//ר��ͼƬ
	albumLabel = new QLabel(this);
	albumLabel->setToolTip(tr("ר��ͼƬ"));

	//�����ı������
	textEdit = new QTextEdit(this);
	textEdit->setObjectName(tr("textEdit"));

	//����
	sharedLabel = new QLabel(this);
	sharedLabel->setObjectName(tr("sharedLabel"));
	sharedLabel->setText(tr("����"));

	//����΢��
	xinlangweiboCheckBox = new QCheckBox(this);
	xinlangweiboCheckBox->setToolTip(tr("��������΢��"));
	xinlangweiboCheckBox->setStyleSheet("QCheckBox{color:black;}"
		"QCheckBox::indicator{width:10px;height:10px;border: 1px solid black;border-radius:2px}"
		"QCheckBox::indicator:checked {image: url(:/app/images/checked2.png);}");
	xinlangweiboLabel = new QLabel(this);
	xinlangweiboLabel->setToolTip(tr("��������΢��"));
	xinlangweiboLabel->setObjectName(tr("xinlangweiboLabel"));

	//��Ѷ΢��
	tengxunweiboCheckBox = new QCheckBox(this);
	tengxunweiboCheckBox->setToolTip(tr("������Ѷ΢��"));
	tengxunweiboCheckBox->setStyleSheet("QCheckBox{color:black;}"
		"QCheckBox::indicator{width:10px;height:10px;border: 1px solid black;border-radius:2px}"
		"QCheckBox::indicator:checked {image: url(:/app/images/checked2.png);}");
	tengxunweiboLabel = new QLabel(this);
	tengxunweiboLabel->setToolTip(tr("������Ѷ΢��"));
	tengxunweiboLabel->setObjectName(tr("tengxunweiboLabel"));

	//QQ�ռ�
	qqkongjianCheckBox = new QCheckBox(this);
	qqkongjianCheckBox->setToolTip(tr("����QQ�ռ�"));
	qqkongjianCheckBox->setStyleSheet("QCheckBox{color:black;}"
		"QCheckBox::indicator{width:10px;height:10px;border: 1px solid black;border-radius:2px}"
		"QCheckBox::indicator:checked {image: url(:/app/images/checked2.png);}");
	qqkongjianLabel = new QLabel(this);
	qqkongjianLabel->setToolTip(tr("����QQ�ռ�"));
	qqkongjianLabel->setObjectName(tr("qqkongjianLabel"));

	//������
	renrenCheckBox = new QCheckBox(this);
	renrenCheckBox->setToolTip(tr("����������"));
	renrenCheckBox->setStyleSheet("QCheckBox{color:black;}"
		"QCheckBox::indicator{width:10px;height:10px;border: 1px solid black;border-radius:2px}"
		"QCheckBox::indicator:checked {image: url(:/app/images/checked2.png);}");
	renrenLabel = new QLabel(this);
	renrenLabel->setToolTip(tr("����������"));
	renrenLabel->setObjectName(tr("renrenLabel"));

	//������
	kaixinCheckBox = new QCheckBox(this);
	kaixinCheckBox->setToolTip(tr("����������"));
	kaixinCheckBox->setStyleSheet("QCheckBox{color:black;}"
		"QCheckBox::indicator{width:10px;height:10px;border: 1px solid black;border-radius:2px}"
		"QCheckBox::indicator:checked {image: url(:/app/images/checked2.png);}");
	kaixinLabel = new QLabel(this);
	kaixinLabel->setToolTip(tr("����������"));
	kaixinLabel->setObjectName(tr("kaixinLabel"));

	//������
	doubanCheckBox = new QCheckBox(this);
	doubanCheckBox->setToolTip(tr("����������"));
	doubanCheckBox->setStyleSheet("QCheckBox{color:black;}"
		"QCheckBox::indicator{width:10px;height:10px;border: 1px solid black;border-radius:2px}"
		"QCheckBox::indicator:checked {image: url(:/app/images/checked2.png);}");
	doubanLabel = new QLabel(this);
	doubanLabel->setToolTip(tr("����������"));
	doubanLabel->setObjectName(tr("doubanLabel"));

	//�Ѻ�΢��
	souhuweiboCheckBox = new QCheckBox(this);
	souhuweiboCheckBox->setToolTip(tr("�����Ѻ�΢��"));
	souhuweiboCheckBox->setStyleSheet("QCheckBox{color:black;}"
		"QCheckBox::indicator{width:10px;height:10px;border: 1px solid black;border-radius:2px}"
		"QCheckBox::indicator:checked {image: url(:/app/images/checked2.png);}");
	souhuweiboLabel = new QLabel(this);
	souhuweiboLabel->setToolTip(tr("�����Ѻ�΢��"));
	souhuweiboLabel->setObjectName(tr("souhuweiboLabel"));

	//QQ����
	qqhaoyouCheckBox = new QCheckBox(this);
	qqhaoyouCheckBox->setToolTip(tr("����QQ����"));
	qqhaoyouCheckBox->setStyleSheet("QCheckBox{color:black;}"
		"QCheckBox::indicator{width:10px;height:10px;border: 1px solid black;border-radius:2px}"
		"QCheckBox::indicator:checked {image: url(:/app/images/checked2.png);}");
	qqhaoyouLabel = new QLabel(this);
	qqhaoyouLabel->setToolTip(tr("����QQ����"));
	qqhaoyouLabel->setObjectName(tr("qqhaoyouLabel"));

	//����ť
	shareButton = new QPushButton(this);
	shareButton->setText(tr("����"));

	//ȡ����ť
	cancelButton = new QPushButton(this);
	cancelButton->setText(tr("ȡ��"));

	albumLabel->setGeometry(25, 20, 110, 103);
	textEdit->setGeometry(150, 20, 250, 103);
	sharedLabel->setGeometry(25, 150, 100, 20);
	xinlangweiboCheckBox->setGeometry(80, 150, 20, 20);
	xinlangweiboLabel->setGeometry(95, 143, 30, 30);
	tengxunweiboCheckBox->setGeometry(145, 150, 20, 20);
	tengxunweiboLabel->setGeometry(160, 143, 30, 30);
	qqkongjianCheckBox->setGeometry(210, 150, 20, 20);
	qqkongjianLabel->setGeometry(225, 143, 30, 30);
	renrenCheckBox->setGeometry(275, 150, 20, 20);
	renrenLabel->setGeometry(290, 143, 30, 30);
	kaixinCheckBox->setGeometry(340, 150, 20, 20);
	kaixinLabel->setGeometry(355, 143, 30, 30);
	doubanCheckBox->setGeometry(80, 200, 20, 20);
	doubanLabel->setGeometry(95, 193, 30, 30);
	souhuweiboCheckBox->setGeometry(145, 200, 20, 20);
	souhuweiboLabel->setGeometry(160, 193, 30, 30);
	qqhaoyouCheckBox->setGeometry(210, 200, 20, 20);
	qqhaoyouLabel->setGeometry(225, 193, 30, 30);
	shareButton->setGeometry(240, 240, 80, 25);
	cancelButton->setGeometry(330, 240, 80, 25);

	//��ѡ��״̬�ı�
	connect(xinlangweiboCheckBox, SIGNAL(stateChanged(int)), this, SLOT(slot_CheckBoxStateChanged(int)));
	connect(tengxunweiboCheckBox, SIGNAL(stateChanged(int)), this, SLOT(slot_CheckBoxStateChanged(int)));
	connect(qqkongjianCheckBox, SIGNAL(stateChanged(int)), this, SLOT(slot_CheckBoxStateChanged(int)));
	connect(renrenCheckBox, SIGNAL(stateChanged(int)), this, SLOT(slot_CheckBoxStateChanged(int)));
	connect(kaixinCheckBox, SIGNAL(stateChanged(int)), this, SLOT(slot_CheckBoxStateChanged(int)));
	connect(doubanCheckBox, SIGNAL(stateChanged(int)), this, SLOT(slot_CheckBoxStateChanged(int)));
	connect(souhuweiboCheckBox, SIGNAL(stateChanged(int)), this, SLOT(slot_CheckBoxStateChanged(int)));
	connect(qqhaoyouCheckBox, SIGNAL(stateChanged(int)), this, SLOT(slot_CheckBoxStateChanged(int)));

	connect(shareButton, SIGNAL(clicked()), this, SLOT(slot_SharedMusic()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(hide()));
}

SharedDialog::~SharedDialog(void)
{
}

//��ʼ������Ի���
void SharedDialog::initSharedDialog( QString albumImagePath, QString strMusicName, QString strAlbumUrlPath )
{
	if (albumImagePath == ":/app/images/albumImage.png")
	{
		albumLabel->setPixmap(QPixmap(albumImagePath));
	}
	else
	{
		//��ͼƬת����label��С����ʾ
		QImage image(albumImagePath);
		QImage newImg = image.scaled(albumLabel->width(), albumLabel->height());
		albumLabel->setPixmap(QPixmap::fromImage(newImg));
	}

	QString strText = "��CZPlayer�����������ס�" + strMusicName + "��������������������";
	textEdit->setText(strText);
	m_strMusicName = strMusicName;
	m_strAlbumUrlPath = strAlbumUrlPath;

	xinlangweiboCheckBox->setCheckState(Qt::Checked);
	tengxunweiboCheckBox->setCheckState(Qt::Unchecked);
	qqkongjianCheckBox->setCheckState(Qt::Unchecked);
	renrenCheckBox->setCheckState(Qt::Unchecked);
	kaixinCheckBox->setCheckState(Qt::Unchecked);
	doubanCheckBox->setCheckState(Qt::Unchecked);
	souhuweiboCheckBox->setCheckState(Qt::Unchecked);
	qqhaoyouCheckBox->setCheckState(Qt::Unchecked);
}

//��ѡ��״̬�ı�
void SharedDialog::slot_CheckBoxStateChanged( int state )
{
	if (state == Qt::Checked)			//ѡ��
	{
		QCheckBox *checkBox = dynamic_cast<QCheckBox*>(sender());
		if (checkBox == xinlangweiboCheckBox)
		{
			tengxunweiboCheckBox->setCheckState(Qt::Unchecked);
			qqkongjianCheckBox->setCheckState(Qt::Unchecked);
			renrenCheckBox->setCheckState(Qt::Unchecked);
			kaixinCheckBox->setCheckState(Qt::Unchecked);
			doubanCheckBox->setCheckState(Qt::Unchecked);
			souhuweiboCheckBox->setCheckState(Qt::Unchecked);
			qqhaoyouCheckBox->setCheckState(Qt::Unchecked);
		}
		else if (checkBox == tengxunweiboCheckBox)
		{
			xinlangweiboCheckBox->setCheckState(Qt::Unchecked);
			qqkongjianCheckBox->setCheckState(Qt::Unchecked);
			renrenCheckBox->setCheckState(Qt::Unchecked);
			kaixinCheckBox->setCheckState(Qt::Unchecked);
			doubanCheckBox->setCheckState(Qt::Unchecked);
			souhuweiboCheckBox->setCheckState(Qt::Unchecked);
			qqhaoyouCheckBox->setCheckState(Qt::Unchecked);
		}
		else if (checkBox == qqkongjianCheckBox)
		{
			xinlangweiboCheckBox->setCheckState(Qt::Unchecked);
			tengxunweiboCheckBox->setCheckState(Qt::Unchecked);
			renrenCheckBox->setCheckState(Qt::Unchecked);
			kaixinCheckBox->setCheckState(Qt::Unchecked);
			doubanCheckBox->setCheckState(Qt::Unchecked);
			souhuweiboCheckBox->setCheckState(Qt::Unchecked);
			qqhaoyouCheckBox->setCheckState(Qt::Unchecked);
		}
		else if (checkBox == renrenCheckBox)
		{
			xinlangweiboCheckBox->setCheckState(Qt::Unchecked);
			tengxunweiboCheckBox->setCheckState(Qt::Unchecked);
			qqkongjianCheckBox->setCheckState(Qt::Unchecked);
			kaixinCheckBox->setCheckState(Qt::Unchecked);
			doubanCheckBox->setCheckState(Qt::Unchecked);
			souhuweiboCheckBox->setCheckState(Qt::Unchecked);
			qqhaoyouCheckBox->setCheckState(Qt::Unchecked);
		}
		else if (checkBox == kaixinCheckBox)
		{
			xinlangweiboCheckBox->setCheckState(Qt::Unchecked);
			tengxunweiboCheckBox->setCheckState(Qt::Unchecked);
			qqkongjianCheckBox->setCheckState(Qt::Unchecked);
			renrenCheckBox->setCheckState(Qt::Unchecked);
			doubanCheckBox->setCheckState(Qt::Unchecked);
			souhuweiboCheckBox->setCheckState(Qt::Unchecked);
			qqhaoyouCheckBox->setCheckState(Qt::Unchecked);
		}
		else if (checkBox == doubanCheckBox)
		{
			xinlangweiboCheckBox->setCheckState(Qt::Unchecked);
			tengxunweiboCheckBox->setCheckState(Qt::Unchecked);
			qqkongjianCheckBox->setCheckState(Qt::Unchecked);
			renrenCheckBox->setCheckState(Qt::Unchecked);
			kaixinCheckBox->setCheckState(Qt::Unchecked);
			souhuweiboCheckBox->setCheckState(Qt::Unchecked);
			qqhaoyouCheckBox->setCheckState(Qt::Unchecked);
		}
		else if (checkBox == souhuweiboCheckBox)
		{
			xinlangweiboCheckBox->setCheckState(Qt::Unchecked);
			tengxunweiboCheckBox->setCheckState(Qt::Unchecked);
			qqkongjianCheckBox->setCheckState(Qt::Unchecked);
			renrenCheckBox->setCheckState(Qt::Unchecked);
			kaixinCheckBox->setCheckState(Qt::Unchecked);
			doubanCheckBox->setCheckState(Qt::Unchecked);
			qqhaoyouCheckBox->setCheckState(Qt::Unchecked);
		}
		else if (checkBox == qqhaoyouCheckBox)
		{
			xinlangweiboCheckBox->setCheckState(Qt::Unchecked);
			tengxunweiboCheckBox->setCheckState(Qt::Unchecked);
			qqkongjianCheckBox->setCheckState(Qt::Unchecked);
			renrenCheckBox->setCheckState(Qt::Unchecked);
			kaixinCheckBox->setCheckState(Qt::Unchecked);
			doubanCheckBox->setCheckState(Qt::Unchecked);
			souhuweiboCheckBox->setCheckState(Qt::Unchecked);
		}
	}
}

//�������
void SharedDialog::slot_SharedMusic()
{
	if (textEdit->toPlainText() == "")
	{
		QMessageBox::information(NULL, tr("��Ϣ"), tr("��������ݲ���Ϊ�գ�"), QMessageBox::Yes);
		return;
	}
	if (xinlangweiboCheckBox->checkState() == Qt::Unchecked
		&& tengxunweiboCheckBox->checkState() == Qt::Unchecked
		&& qqkongjianCheckBox->checkState() == Qt::Unchecked
		&& renrenCheckBox->checkState() == Qt::Unchecked
		&& kaixinCheckBox->checkState() == Qt::Unchecked
		&& doubanCheckBox->checkState() == Qt::Unchecked
		&& souhuweiboCheckBox->checkState() == Qt::Unchecked
		&& qqhaoyouCheckBox->checkState() == Qt::Unchecked)
	{
		QMessageBox::information(NULL, tr("��Ϣ"), tr("�빴ѡҪ��������վ��"), QMessageBox::Yes);
		return;
	}

	QString strMyIndex = "http://www.qtcn.org/bbs/read-htm-tid-55824.html";

	if (xinlangweiboCheckBox->checkState() == Qt::Checked)
	{
		qDebug() << "��������΢��";

		QString qStrUrl = "http://service.weibo.com/share/share.php?url=" 
			+ strMyIndex + "&title=" + textEdit->toPlainText() + "&pic=" + m_strAlbumUrlPath;
		this->openBrowser(qStrUrl);
	}
	else if (tengxunweiboCheckBox->checkState() == Qt::Checked)
	{
		qDebug() << "������Ѷ΢��";

		QString qStrUrl = "http://share.v.t.qq.com/index.php?c=share&a=index&url=" 
			+ strMyIndex + "&title=" + textEdit->toPlainText() + "&pic=" + m_strAlbumUrlPath;
		this->openBrowser(qStrUrl);
	}
	else if (qqkongjianCheckBox->checkState() == Qt::Checked)
	{
		qDebug() << "����QQ�ռ�";

		QString qStrUrl = "http://sns.qzone.qq.com/cgi-bin/qzshare/cgi_qzshare_onekey?url=" 
			+ strMyIndex + "&title=������CZPlayer&desc=" + textEdit->toPlainText() + "&pics=" + m_strAlbumUrlPath;
		this->openBrowser(qStrUrl);
	}
	else if (renrenCheckBox->checkState() == Qt::Checked)
	{
		qDebug() << "����������";

		QString qStrUrl = "http://widget.renren.com/dialog/share?resourceUrl=" 
			+ strMyIndex + "&description=" + textEdit->toPlainText() + "&pic=" + m_strAlbumUrlPath;
		this->openBrowser(qStrUrl);
	}
	else if (kaixinCheckBox->checkState() == Qt::Checked)
	{
		qDebug() << "����������";

		QString qStrUrl = "http://www.kaixin001.com/rest/records.php?content="
			+ textEdit->toPlainText() + "&url=" + strMyIndex + "&starid=&aid=100018706&style=11&pic=" + m_strAlbumUrlPath;
		this->openBrowser(qStrUrl);
	}
	else if (doubanCheckBox->checkState() == Qt::Checked)
	{
		qDebug() << "����������";

		QString qStrUrl = "http://www.douban.com/share/service?href=" + m_strAlbumUrlPath + "&name="
			+ textEdit->toPlainText() + "&text=" + strMyIndex;
		this->openBrowser(qStrUrl);
	}
	else if (souhuweiboCheckBox->checkState() == Qt::Checked)
	{
		qDebug() << "�����Ѻ�΢��";

		QString qStrUrl = "http://t.sohu.com/third/post.jsp?url=" + strMyIndex + "&title=" + textEdit->toPlainText();
		this->openBrowser(qStrUrl);
	}
	else if (qqhaoyouCheckBox->checkState() == Qt::Checked)
	{
		qDebug() << "����QQ����";

		QString qStrUrl = "http://connect.qq.com/widget/shareqq/index.html?url=" 
			+ strMyIndex + "&desc=" + textEdit->toPlainText() + "&title=" 
			+ m_strMusicName + "&summary=�����֣���CZPlayer&pics=" + m_strAlbumUrlPath;
		this->openBrowser(qStrUrl);
	}
}

//�������
void SharedDialog::openBrowser( QString &qStrUrl )
{
	WCHAR wcUrl[4096];
	astr_ustr(const_cast<char*>(qStrUrl.toStdString().c_str()), wcUrl);
	ShellExecute(NULL, _T("open"), wcUrl, NULL, NULL, SW_SHOWNORMAL);
}