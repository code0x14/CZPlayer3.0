#include "SearchPanel.h"

SearchPanel::SearchPanel( QWidget *widget /*= 0*/ ) 
	: m_parent(widget), m_nMusicNum(0), m_nStartPage(0), m_strSearchKey(""), m_bLoadStared(false)
{
	//���ô��ڻ�������
	this->resize(400, 662);//���ô����С
	this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);//ȥ������߿�,������������ʾͼ��
	this->setAttribute(Qt::WA_TranslucentBackground);//���ñ���͸��
	this->setWindowIcon(QIcon(":/app/images/CZPlayer.png"));//����logo
	this->setWindowTitle(tr("��������"));

	//������
	navigationBarLabel = new QLabel(this);
	navigationBarLabel->setObjectName(tr("navigationBarLabel"));

	//����
	searchMusicButton = new QPushButton(this);
	searchMusicButton->setObjectName(tr("searchMusicButton"));
	searchMusicButton->setStyleSheet("QPushButton:!hover{border-image: url(:/app/images/searchMusicButton.png);}"
										"QPushButton:hover{border-image: url(:/app/images/searchMusicButton2.png);}"
										"QPushButton:pressed{border-image: url(:/app/images/searchMusicButton3.png);}");

	//��̨
	broadcastingStationButton = new QPushButton(this);
	broadcastingStationButton->setObjectName(tr("broadcastingStationButton"));
	broadcastingStationButton->setStyleSheet("QPushButton:!hover{border-image: url(:/app/images/broadcastingStationButton.png);}"
		"QPushButton:hover{border-image: url(:/app/images/broadcastingStationButton2.png);}"
		"QPushButton:pressed{border-image: url(:/app/images/broadcastingStationButton3.png);}");

	//��
	listButton = new QPushButton(this);
	listButton->setObjectName(tr("listButton"));
	listButton->setStyleSheet("QPushButton:!hover{border-image: url(:/app/images/listButton.png);}"
		"QPushButton:hover{border-image: url(:/app/images/listButton2.png);}"
		"QPushButton:pressed{border-image: url(:/app/images/listButton3.png);}");

	//�����б�
	downLoadListButton = new QPushButton(this);
	downLoadListButton->setObjectName(tr("downLoadListButton"));
	downLoadListButton->setStyleSheet("QPushButton:!hover{border-image: url(:/app/images/downLoadListButton.png);}"
		"QPushButton:hover{border-image: url(:/app/images/downLoadListButton2.png);}"
		"QPushButton:pressed{border-image: url(:/app/images/downLoadListButton3.png);}");

	//�ղ��б�
	favoritesListButton = new QPushButton(this);
	favoritesListButton->setObjectName(tr("favoritesListButton"));
	favoritesListButton->setStyleSheet("QPushButton:!hover{border-image: url(:/app/images/favoritesListButton.png);}"
		"QPushButton:hover{border-image: url(:/app/images/favoritesListButton2.png);}"
		"QPushButton:pressed{border-image: url(:/app/images/favoritesListButton3.png);}");

	//���д��
	lyricsPhotoBarButton = new QPushButton(this);
	lyricsPhotoBarButton->setObjectName(tr("lyricsPhotoBarButton"));
	lyricsPhotoBarButton->setStyleSheet("QPushButton:!hover{border-image: url(:/app/images/lyricsPhotoBarButton.png);}"
		"QPushButton:hover{border-image: url(:/app/images/lyricsPhotoBarButton2.png);}"
		"QPushButton:pressed{border-image: url(:/app/images/lyricsPhotoBarButton3.png);}");

	//������
	searchLineEdit = new QLineEdit(this);
	searchLineEdit->setObjectName(tr("searchLineEdit"));
	searchLineEdit->setText(tr("�������֡�������ר��"));
	searchLineEdit->installEventFilter(this);
	//searchLineEdit->setPlaceholderText(tr("�������֡�������ר��"));

	//������ť
	searchButton = new QPushButton(this);
	searchButton->setObjectName(tr("searchButton"));

	//������Ϣ��ʾ
	searchInfoLabel = new QLabel(this);
	searchInfoLabel->setObjectName(tr("searchInfoLabel"));
	searchInfoLabel->setText(tr("CZPlayer����������"));

	searchInfoLabel2 = new QLabel(this);
	searchInfoLabel2->setObjectName(tr("searchInfoLabel"));

	//ȫѡ
	QCheckBox *allChecked = new QCheckBox(tr("ȫѡ"), this);
	allChecked->setObjectName(tr("allChecked"));
	allChecked->setFont(QFont("΢���ź�", 10));
	allChecked->setStyleSheet("QCheckBox{color:white;}"
		"QCheckBox::indicator{width:10px;height:10px;border: 1px solid white;border-radius:2px}"
		"QCheckBox::indicator:checked {image: url(:/app/images/checked2.png);}");

	//���ذ�ť
	downButton = new QPushButton(this);
	downButton->setObjectName(tr("downButton"));
	downButton->setToolTip(tr("����"));

	//�ղ�
	collectButton = new QPushButton(this);
	collectButton->setObjectName(tr("collectButton"));
	collectButton->setToolTip(tr("�ղ�"));

	//�����б�
	searchList = new SearchList(this);
	searchList->setObjectName(tr("searchList"));

	//for (int i = 0; i < 10; ++i)
	//{
	//	//������
	//	//QCheckBox *musicNameCheckBox = new QCheckBox("�������");
	//	//musicNameCheckBox->setObjectName(tr("musicNameCheckBox"));
	//	//musicNameCheckBox->setFont(QFont("΢���ź�", 10));
	//	//musicNameCheckBox->setStyleSheet("QCheckBox{color:white;}"
	//	//	"QCheckBox::indicator{width:10px;height:10px;border: 1px solid white;border-radius:2px}"
	//	//	"QCheckBox::indicator:checked {image: url(:/app/images/checked2.png);}");

	//	QTableWidgetItem *musicNameItem = new QTableWidgetItem("�������");
	//	musicNameItem->setTextAlignment(Qt::AlignCenter);
	//	musicNameItem->setFont(QFont("΢���ź�", 10));
	//	musicNameItem->setCheckState(Qt::Unchecked);

	//	//����
	//	QTableWidgetItem *artistItem = new QTableWidgetItem("beyond");
	//	artistItem->setTextAlignment(Qt::AlignCenter);
	//	artistItem->setFont(QFont("΢���ź�", 10));

	//	//ר��
	//	QTableWidgetItem *albumItem = new QTableWidgetItem("��������ա�");
	//	albumItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	//	albumItem->setFont(QFont("΢���ź�", 10));

	//	//���벥���б�
	//	int currentRows = searchList->rowCount();//�����б��е�����
	//	searchList->insertRow(currentRows);//�Ӳ����б��еĵ�ǰ�в���
	//	//searchList->setCellWidget(currentRows, 0, musicNameCheckBox);
	//	searchList->setItem(currentRows, 0, musicNameItem);
	//	searchList->setItem(currentRows, 1, artistItem);
	//	searchList->setItem(currentRows, 2, albumItem);
	//}

	//��ȡQTreeWidget�Ĺ�����
	scrollBar = searchList->verticalScrollBar();
	scrollBar->setObjectName(tr("scrollBar"));

	QSpacerItem *topItem = new QSpacerItem(380, 160, QSizePolicy::Fixed, QSizePolicy::Fixed);
	QSpacerItem *bottomItem = new QSpacerItem(380, 30, QSizePolicy::Fixed, QSizePolicy::Fixed);
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addItem(topItem);
	mainLayout->addWidget(searchList);
	mainLayout->addItem(bottomItem);
	this->setLayout(mainLayout);

	//��ʱ��
	timer = new QTimer(this);
	timer->start(1000);

	//�����߳�
	searchThread = new SearchThread(searchList);

	//searchLineEdit->setGeometry(27, 7, 250, 30);
	//searchButton->setGeometry(287, 9, 90, 30);
	//searchInfoLabel->setGeometry(11, 50, 380, 15);
	//searchInfoLabel2->setGeometry(11, 625, 380, 15);
	//allChecked->setGeometry(11, 80, 50, 15);
	//downButton->setGeometry(280, 79, 16, 16);
	//collectButton->setGeometry(320, 79, 16, 16);

	navigationBarLabel->setGeometry(5, 10, 390, 33);
	searchMusicButton->setGeometry(7, 12, 51, 29);
	broadcastingStationButton->setGeometry(60, 12, 57, 29);
	listButton->setGeometry(119, 12, 59, 29);
	downLoadListButton->setGeometry(180, 12, 70, 29);
	favoritesListButton->setGeometry(253, 12, 69, 29);
	lyricsPhotoBarButton->setGeometry(325, 12, 68, 29);

	searchLineEdit->setGeometry(27, 77, 250, 30);
	searchButton->setGeometry(287, 79, 90, 30);
	searchInfoLabel->setGeometry(11, 120, 380, 15);
	searchInfoLabel2->setGeometry(11, 625, 380, 15);
	allChecked->setGeometry(11, 150, 50, 15);
	downButton->setGeometry(280, 149, 16, 16);
	collectButton->setGeometry(320, 149, 16, 16);

	connect(searchButton, SIGNAL(clicked()), this, SLOT(slot_SearchMusic()));					//��������
	connect(timer, SIGNAL(timeout()), this, SLOT(slot_LineEditFocus()));						//�����۽�
	connect(scrollBar, SIGNAL(valueChanged(int)), this, SLOT(slot_ScrollBarEvent(int)));		//�������¼�
	connect(allChecked, SIGNAL(stateChanged(int)), this, SLOT(slot_CheckBoxStateChanged(int)));	//��ѡ��״̬�ı�
	connect(searchThread, SIGNAL(sig_LoadFinished()), this, SLOT(slot_LoadFinished()));			//�������
	connect(searchThread, SIGNAL(sig_SearchNum(int)), this, SLOT(slot_SearchNum(int)));			//����������ظ�������
	connect(searchThread, SIGNAL(sig_SearchTimeout()), this, SLOT(slot_SearchTimeout()));		//������ʱ
}

SearchPanel::~SearchPanel(void)
{
}

//��������
void SearchPanel::slot_SearchMusic()
{
	m_strSearchKey = searchLineEdit->text();	//��ȡ��������������
	if (m_strSearchKey == "" || m_strSearchKey == "�������֡�������ר��")
	{
		QMessageBox::information(NULL, tr("��Ϣ"), tr("�����������������Ϊ�գ�"), QMessageBox::Yes);
		return;
	}

	//��������б�
	searchList->clear();
	while (searchList->rowCount() > 0)
	{
		searchList->removeRow(0);
	}

	//��ʼ������
	m_nStartPage = 1;		//���ؿ�ʼλ��(���ص�һҳ)
	m_nMusicNum = 0;
	m_bLoadStared = false;
	searchInfoLabel->setText(tr("�������������Ե�......"));
	searchInfoLabel2->setText(tr(""));

	//�����߳�
	//SearchThread *searchThread = new SearchThread(m_strSearchKey, m_nStartPage, searchList);
	//connect(searchThread, SIGNAL(sig_LoadFinished()), this, SLOT(slot_LoadFinished()));
	//connect(searchThread, SIGNAL(sig_SearchNum(int)), this, SLOT(slot_SearchNum(int)));
	//searchThread->start();//�����߳�
	searchThread->setParam(m_strSearchKey, m_nStartPage);
	searchThread->startFunc();
}

//�����۽�
void SearchPanel::slot_LineEditFocus()
{
	if ("" == searchLineEdit->text() && !searchLineEdit->hasFocus())
	{
		searchLineEdit->setText(tr("�������֡�������ר��"));
	}
}

//�������¼�
void SearchPanel::slot_ScrollBarEvent( int value )
{
	//�жϹ������Ƿ���������·� ,���һ�û�п�ʼ����
	if (scrollBar->maximum() == value && m_bLoadStared == false)
	{
		qDebug() << "����������ײ�";
		//�ж��Ѿ����ص����������Ƿ�С����������
		if ((m_nStartPage - 1) * 8 < m_nMusicNum)
		{
			searchInfoLabel2->setText(tr("���ڼ��أ����Ե�......"));

			//�����߳�
			//SearchThread *searchThread = new SearchThread(m_strSearchKey, m_nStartPage, searchList);
			//connect(searchThread, SIGNAL(sig_LoadFinished()), this, SLOT(slot_LoadFinished()));
			//connect(searchThread, SIGNAL(sig_SearchNum(int)), this, SLOT(slot_SearchNum(int)));
			//searchThread->start();	//�����߳�
			searchThread->setParam(m_strSearchKey, m_nStartPage);
			searchThread->startFunc();

			m_bLoadStared = true;	//��ʼ����
		}
	}
}

//�ѵ�����ظ�������
void SearchPanel::slot_SearchNum( int nNum )
{
	searchInfoLabel->setText(tr("������%1�����ҵ���ظ���%2��").arg(m_strSearchKey).arg(nNum));
	m_nMusicNum = nNum;		//��������
}

//�������
void SearchPanel::slot_LoadFinished()
{
	m_nStartPage = m_nStartPage + 2;		//����ҳ����2
	searchInfoLabel2->setText(tr("�Ѽ��ظ���%1��").arg(searchList->rowCount()));
	m_bLoadStared = false;					//û�п�ʼ����

	searchThread->terminate(); 
	//terminate:��ֹ�߳�
	//QThread *thread = dynamic_cast<QThread *>(sender());
	//if (thread->isRunning()) { thread->terminate(); delete thread; thread = 0; }
}

//������ʱ
void SearchPanel::slot_SearchTimeout()
{
	searchInfoLabel->setText(tr("������ʱ����������������߱�������ǽ����"));
}

//��ѡ��״̬�ı�
void SearchPanel::slot_CheckBoxStateChanged( int state )
{
	if (state == Qt::Checked)			//ѡ��
	{
	}
	else if (state == Qt::Unchecked)	//ȡ��ѡ��
	{
	}
}

//��дpaintEvent,��ӱ���ͼƬ
void SearchPanel::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QPixmap backgroundImage;
	backgroundImage.load(":/app/images/searchPanel.png");

	//��ͨ��pix�ķ������ͼƬ�Ĺ��˵�͸���Ĳ��ֵõ���ͼƬ����ΪloginPanel�Ĳ�����߿�
	this->setMask(backgroundImage.mask());
	painter.drawPixmap(0, 0, 400, 662, backgroundImage);
	event->accept();
}

//�¼�������
bool SearchPanel::eventFilter(QObject *target, QEvent *event)
{
	if (target == searchLineEdit)
	{
		if (event->type() == QEvent::KeyPress)
		{
			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
			if (keyEvent->key() == Qt::Key_Return)
			{
				searchButton->click();//ѡ�����ذ�ť
				return true;
			}
		}

		if (event->type() == QEvent::MouseButtonPress)
		{
			if ("�������֡�������ר��" == searchLineEdit->text())
			{
				searchLineEdit->setText(tr(""));
			}
			return true;
		}
	}

	return QWidget::eventFilter(target, event);
}