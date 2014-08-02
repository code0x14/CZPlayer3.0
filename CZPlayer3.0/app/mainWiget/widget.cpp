#include "widget.h"
#include "static.h"
#include "czplayerconfig.h"
#include "ClientBaseOperating.h"
#include "DBModule.h"
#include "SoundControl.h"

//�õ����ֲ���ʱ�䣬Ŀǰ��֧��mp3��wma��wav��ʽ
static unsigned long getMusicTime(WCHAR *szPath)
{
	MCIERROR mciError;
	MCI_OPEN_PARMS mciOpen;  
	MCI_STATUS_PARMS mciStatusParms;

	mciOpen.lpstrElementName = szPath; 
	mciError = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&mciOpen);
	if (mciError)
	{
		return 0;
	}

	mciStatusParms.dwItem = MCI_STATUS_LENGTH;  
	mciSendCommand(mciOpen.wDeviceID, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatusParms);  
	mciSendCommand(mciOpen.wDeviceID, MCI_CLOSE, 0, NULL);

	return mciStatusParms.dwReturn / 1000; 
}

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

/*
CTRL_CLOSE_EVENT, CTRL_LOGOFF_EVENT��CTRL_SHUTDOWN_EVENTͨ������������һЩ�������������
Ȼ�����ExitProcess API�����⣬�������¼��г�ʱ���ƣ�CTRL_CLOSE_EVENT��5�룬����������20�롣
�������ʱ��ϵͳ���ᵯ���������̵ĶԻ�������û�ѡ���˽������̣�
�κ���������������������Ӧ���ڳ�ʱʱ������ɹ���
*/
//�¼�����ص�
BOOL HandlerRoutine(DWORD dwCtrlType)  
{  
	switch (dwCtrlType)  
	{  
	case CTRL_C_EVENT:						//Ctrl+C��ϵͳ�ᷢ�ʹ���Ϣ
		qDebug() << "ctrl+c--���򲻻�ر�";
		return TRUE;  
	case CTRL_CLOSE_EVENT:					 //���û��ر�Consoleʱ��ϵͳ�ᷢ�ʹ���Ϣ
		qDebug() << "ctrl close--�����ر�";
		return TRUE;  
	case CTRL_BREAK_EVENT:					//Ctrl+break��ϵͳ�ᷢ�ʹ���Ϣ
		qDebug() << "CTRL_BREAK_EVENT";  
		return TRUE;
	case CTRL_LOGOFF_EVENT:					//�û��˳���ע������ϵͳ�ᷢ�ʹ���Ϣ
		qDebug() << "CTRL_LOGOFF_EVENT--�û��˳���ע��";  
		return TRUE;
	case CTRL_SHUTDOWN_EVENT:				//ϵͳ�رգ�ϵͳ�ᷢ�ʹ���Ϣ
		qDebug() << "CTRL_SHUTDOWN_EVENT--ϵͳ�ر�";  
		return TRUE;
	default: 
		return FALSE;  
	}  
} 

//���캯��
Widget::Widget(char *argvMusicPath)
{
	m_argvMusicPath = argvMusicPath;//�����������Ĳ���
	this->changeWorkDir();			//�ı乤��Ŀ¼

	//WinExec("./mplayer.exe -slave -quiet -vo directx:noaccel mmsh://112.91.122.34:999/1058", SW_HIDE);
	//QString mmsName = "mms://live.cri.cn/pop";
	//QStringList args;
	//args << QString("-slave") << QString("-quiet") << QString("-vo") << QString("directx:noaccel") << mmsName;
	//if (QProcess::startDetached("./mplayer.exe", args))
	//{
	//	qDebug() << "start success!";
	//}
	//else
	//{
	//	qDebug() << "start failed!";
	//}

	m_mapMusicFilePath.clear();
	m_mapMusicRows.clear();
	m_strCurrentFilePath = "";
	m_strCurrentMusicName = "";
	m_strCurrentMusicTime = "00:00";
	m_nCurrentMusicRow = 0;
	nTime = 0;
	nPos = 170;
	isStarupLoadMusic = true;		//�Ƿ�������ʱ���ز����б�
	basicPlayer = NULL;

	//��װ�¼�����
	if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)HandlerRoutine, TRUE))
	{
		qDebug() << "��װ�¼�����ʧ�ܣ�";
	}

	qsrand(time(NULL));							//��ʼ�����������
	DBModule::initDB();							//��ʼ�����ݿ�
	ClientBaseOperating::initConfig();			//��ʼ������

	//m_engine = new Engine(this);				//��������
	this->createUI();							//��������
	this->connectUI();							//�ź����
	m_state = NoState;							//�趨һ��״̬
}

//��������
Widget::~Widget()
{
	if (searchPanel) { delete searchPanel; searchPanel = 0; }
	if (playList) { delete playList; playList = 0; }
	if (musicListWidget) { delete musicListWidget; musicListWidget = 0; }
	if (lrc) { delete lrc; lrc = 0; }
	if (minMusicWidget) { delete minMusicWidget; minMusicWidget = 0; }

	//terminate:��ֹ�߳�
	if (hotpugWatcherThread->isRunning()) { hotpugWatcherThread->terminate(); delete hotpugWatcherThread; hotpugWatcherThread = 0;}
}

//��������
void Widget::createUI()
{
	//���ô��ڻ�������
	this->resize(400, 212);//���ô����С
	this->setWindowFlags(Qt::FramelessWindowHint);//ȥ������߿�
	this->setAttribute(Qt::WA_TranslucentBackground);//���ñ���͸��
	this->setWindowIcon(QIcon(":/app/images/CZPlayer.png"));//����logo
	this->setAcceptDrops(true);//�����Ϸ��ļ�
	this->setWindowTitle(tr("CZPlayer"));

	//ר������
	albumImage = new QLabel(this);
	albumImage->setToolTip(tr("ר������"));
	albumImage->setPixmap(QPixmap(tr(":/app/images/albumImage.png")));
	movie = new QMovie(tr(":/app/images/jiaZai.gif"));

	//��Ϣ��ʾ��ǩ
	InfoLabel = new QLabel(this);
	InfoLabel->setObjectName("InfoLabel");
	InfoLabel->setText(tr("Welecome to CZPlayer!"));

	//��ǰ���Ÿ���
	currentMusicLabel = new QLabel(this);
	currentMusicLabel->setObjectName(tr("currentMusicLabel"));
	currentMusicLabel->setText(tr("Welecome to CZPlayer!"));

	//Ƶ�׽���
	m_spectrograph = new Spectrograph(this);
	m_spectrograph->setParams(0.0, 1000.0);	//��ʼ��Ƶ�ײ���

	//�������Ʋ��Ž��ȵĻ���
	seekSlider = new QSlider(Qt::Horizontal, this);
	seekSlider->setObjectName(tr("seekSlider"));

	//�����������ڻ���
	volumeSlider = new QSlider(Qt::Horizontal, this);
	volumeSlider->setObjectName(tr("volumeSlider"));
	volumeSlider->setToolTip(tr("��������"));
	volumeSlider->setRange(0, 100);
	volumeSlider->setValue(CZPlayerConfig::getValue("VOLUMEVALUE").toInt());
	SoundControl::SetVolume(0, CZPlayerConfig::getValue("VOLUMEVALUE").toInt());	//��������

	//�������ڰ�ť
	volumeButton = new QPushButton(this);
	volumeButton->setObjectName(tr("volumeButton"));
	if (CZPlayerConfig::getValue("SETMUTE").toString() == "false")
	{
		volumeButton->setToolTip(tr("����"));
		volumeButton->setStyleSheet("QPushButton:!hover{border-image: url(:/app/images/soundButton.png);}"
			"QPushButton:hover{border-image: url(:/app/images/soundButton2.png);}"
			"QPushButton:pressed{border-image: url(:/app/images/soundButton3.png);}");
	}
	else
	{
		SoundControl::SetVolume(0, 0);		//����
		volumeButton->setToolTip(tr("�ָ�����"));
		volumeButton->setStyleSheet("QPushButton:!hover{border-image: url(:/app/images/soundButton4.png);}"
			"QPushButton:hover{border-image: url(:/app/images/soundButton5.png);}"
			"QPushButton:pressed{border-image: url(:/app/images/soundButton6.png);}");
	}

	//����ģʽ��ť
	modeButton = new QPushButton(this);
	modeButton->setObjectName(tr("modeButton"));
	if (ClientBaseOperating::initPlayMode() == "cyclePlay")	//ѭ������
	{
		modeButton->setStyleSheet("QPushButton:!hover{border-image: url(:/app/images/cyclePlay.png);}"
			"QPushButton:hover{border-image: url(:/app/images/cyclePlay2.png);}"
			"QPushButton:pressed{border-image: url(:/app/images/cyclePlay3.png);}");
		modeButton->setToolTip(tr("ѭ������"));
		this->setMode(cyclePlay);							//����ѭ������ģʽ
	}
	else if (ClientBaseOperating::initPlayMode() == "randomPlay")	//�������
	{
		modeButton->setStyleSheet("QPushButton:!hover{border-image: url(:/app/images/randomPlay.png);}"
			"QPushButton:hover{border-image: url(:/app/images/randomPlay2.png);}"
			"QPushButton:pressed{border-image: url(:/app/images/randomPlay3.png);}");
		modeButton->setToolTip(tr("�������"));
		this->setMode(randomPlay);							//�����������ģʽ
	}


	//������ʾʱ���ǩ
	timeLabel = new QLabel(tr("00:00/00:00"), this);
	timeLabel->setToolTip(tr("��ǰʱ��/��ʱ��"));
	timeLabel->setObjectName(tr("timeLabel"));
	timeLabel->setAlignment(Qt::AlignCenter);//���ö����ʽ
	//QSizePolicy��������ˮƽ�ʹ�ֱ�޸Ĵ�С���Ե�һ������
	timeLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);//ˮƽ�����ϳߴ����չ����ֱ�����ѹ̶�

	//��ʾ����ҳ��
	showButton = new QPushButton(this);
	showButton->setObjectName(tr("showButton"));
	showButton->setToolTip(tr("���������"));
	showButton->setStyleSheet("QPushButton:!hover{border-image: url(:/app/images/showButton.png);}"
		"QPushButton:hover{border-image: url(:/app/images/showButton2.png);}"
		"QPushButton:pressed{border-image: url(:/app/images/showButton3.png);}");

	//����
	playButton = new QPushButton(this);
	playButton->setToolTip(tr("����"));
	playButton->setObjectName(tr("playButton"));
	playButton->setStyleSheet("QPushButton:!hover{border-image: url(:/app/images/playButton.png);}"
		"QPushButton:hover{border-image: url(:/app/images/playButton2.png);}"
		"QPushButton:pressed{border-image: url(:/app/images/playButton3.png);}");

	//ֹͣ
	endButton = new QPushButton(this);
	endButton->setToolTip(tr("ֹͣ"));
	endButton->setObjectName(tr("endButton"));

	//��һ��
	preButton = new QPushButton(this);
	preButton->setToolTip(tr("��һ��"));
	preButton->setObjectName(tr("preButton"));

	//��һ��
	nextButton = new QPushButton(this);
	nextButton->setToolTip(tr("��һ��"));
	nextButton->setObjectName(tr("nextButton"));

	//���ļ�
	openFileButton = new QPushButton(this);
	openFileButton->setToolTip(tr("���ļ�"));
	openFileButton->setObjectName(tr("openFileButton"));

	//�򿪲����б�
	musicListButton = new QPushButton(this);
	musicListButton->setToolTip(tr("�򿪲����б�"));
	musicListButton->setObjectName(tr("musicListButton"));

	//������
	lrcButton = new QPushButton(this);
	lrcButton->setToolTip(tr("������"));
	lrcButton->setObjectName(tr("lrcButton"));

	//�������˳�
	closeAction = new QAction(this);
	closeAction->setIcon(QIcon(":/app/images/quitAction.png"));
	closeAction->setText(tr("�˳�"));

	//��������С��
	minAction = new QAction(this);
	minAction->setIcon(QIcon(":/app/images/minAction.png"));
	minAction->setText(tr("��С��"));

	//���̽����˳�����
	quitAction = new QAction(this);
	quitAction->setIcon(QIcon(":/app/images/quitAction.png"));
	quitAction->setText(tr("�˳�"));

	//��ͣ(����)
	pauseAction = new QAction(this);
	pauseAction->setIcon(QIcon(":/app/images/trayPlay.png"));
	pauseAction->setText(tr("����"));

	//ֹͣ
	stopAction = new QAction(this);
	stopAction->setIcon(QIcon(":/app/images/trayStop.png"));
	stopAction->setText(tr("ֹͣ"));

	//��һ��
	lastAction = new QAction(this);
	lastAction->setIcon(QIcon(":/app/images/trayLast.png"));
	lastAction->setText(tr("��һ��"));

	//��һ��
	nextAction = new QAction(this);
	nextAction->setIcon(QIcon(":/app/images/trayNext.png"));
	nextAction->setText(tr("��һ��"));

	//�������
	unlockLrcAction = new QAction(this);
	unlockLrcAction->setIcon(QIcon(":/app/images/unlock.png"));
	unlockLrcAction->setText(tr("�������"));

	//�������ļ�
	openFileAction = new QAction(this);
	openFileAction->setIcon(QIcon(":/app/images/addMusic.png"));
	openFileAction->setText(tr("�������ļ�"));

	//�򿪲����б�
	openMusicListAction = new QAction(this);
	openMusicListAction->setIcon(QIcon(":/app/images/openMusicList.png"));
	openMusicListAction->setText(tr("�򿪲����б�"));

	//���������
	openSearchPanelAnimation = new QAction(this);
	openSearchPanelAnimation->setIcon(QIcon(":/app/images/openMusicDownload.png"));
	openSearchPanelAnimation->setText(tr("���������"));

	//��ʾ������
	openLrcAction = new QAction(this);
	openLrcAction->setIcon(QIcon(":/app/images/lrc.png"));
	openLrcAction->setText(tr("��ʾ������"));

	//ѡ��
	configAction = new QAction(this);
	configAction->setIcon(QIcon(":/app/images/configButton.png"));
	configAction->setText(tr("����"));

	//����Qt
	aboutQtAction = new QAction(this);
	aboutQtAction->setIcon(QIcon(":/app/images/Qt.png"));
	aboutQtAction->setText(tr("����Qt"));

	//����
	aboutAction = new QAction(this);
	aboutAction->setIcon(QIcon(":/app/images/CZPlayer2.png"));
	aboutAction->setText(tr("����CZPlayer"));

	//ѭ������
	cycleAction = new QAction(this);
	cycleAction->setIcon(QIcon(":/app/images/cyclePlay.png"));

	//�������
	randomAction = new QAction(this);
	randomAction->setIcon(QIcon(":/app/images/randomPlay.png"));

	//����ϵͳ����ͼ��
	trayIcon = new QSystemTrayIcon(QIcon(":/app/images/CZPlayer.png"), this);
	trayIcon->show();
	trayIcon->showMessage(tr("CZPlayer"), tr("��ӭ��½http://www.qtcn.org/bbs/read-htm-tid-55824.html"));
	trayIcon->setToolTip(tr("CZPlayer"));

	//�����˵���ϵͳ����ͼ���һ����ֵĲ˵�
	trayMenu = new QMenu();
	trayMenu->addAction(quitAction);
	trayMenu->addAction(unlockLrcAction);
	trayMenu->addSeparator();
	trayMenu->addAction(pauseAction);
	trayMenu->addAction(stopAction);
	trayMenu->addSeparator();
	trayMenu->addAction(lastAction);
	trayMenu->addAction(nextAction);
	trayIcon->setContextMenu(trayMenu);//��ϵͳ��������Ӳ˵�
	trayIcon->show();

	//С���沥����
	minMusicWidget = new MinMusicWidget(this);
	//�����б�
	playList = new MusicList(this);
	musicListWidget = new MusicListWidget(playList, &m_mapMusicRows, this);
	//���
	lrc = new MusicLrc();
	//�������
	searchPanel = new SearchPanel(this);
	//���ý���
	configDialog = new ConfigDialog(this);
	//�������
	sharedDialog = new SharedDialog(this);
	//����CZPlayer
	aboutPanel = new AboutPanel;
	//ȫ���ȼ�����
	globalHotKey = new GlobalHotKey(this);

	//���ø������
	ClientBaseOperating::initLrcPropery();
	//����ȫ���ȼ�
	ClientBaseOperating::initHotKeyPropery();

	//�����豸�����߳�
	hotpugWatcherThread = new HotplugWatcherThread;
	//hotpugWatcherThread->start();//���߳��ڲ��Ѿ�start

	//����һ����ʱ��
	timer = new QTimer(this);
	timer->start(30);
	shortCutIimer = new QTimer(this);
	shortCutIimer->start(500);
	upDateTimer = new QTimer(this);
	upDateTimer->start(1000);

	//�����ݿ��ж�����Ϣ���������б���
	//�������ظ����б��߳�
	LoadMusicThread *loadMusicThread = new LoadMusicThread;
	connect(loadMusicThread, SIGNAL(sig_LoadMusicListFinished()), this, SLOT(slot_LoadMusicListFinished()));
	connect(loadMusicThread, SIGNAL(sig_LoadMusicListError()), this, SLOT(slot_LoadMusicListError()));
	loadMusicThread->setParam(&m_mapMusicRows, &m_mapMusicFilePath, playList);
	loadMusicThread->start();

	//���������С��λ��
	albumImage->setGeometry(25, 23, 110, 103);
	m_spectrograph->setGeometry(155, 47, 190, 78);
	InfoLabel->setGeometry(153, 69, 240, 39);
	timeLabel->setGeometry(295, 23, 82, 18);
	currentMusicLabel->setGeometry(170, 7, 500, 11);
	showButton->setGeometry(365, 55, 17, 17);
	modeButton->setGeometry(365, 80, 18, 15);

	preButton->setGeometry(33, 164, 25, 25);
	playButton->setGeometry(83, 164, 25, 25);
	endButton->setGeometry(133, 164, 25, 25);
	nextButton->setGeometry(183, 164, 25, 25);
	
	seekSlider->setGeometry(25, 129, 361, 30);
	openFileButton->setGeometry(152, 21, 49, 23);
	musicListButton->setGeometry(201, 20, 35, 24);
	lrcButton->setGeometry(236, 21, 44, 23);
	volumeButton->setGeometry(249, 168, 16, 16);
	volumeSlider->setGeometry(277, 161, 108, 30);
}

//�ź����
void Widget::connectUI()
{
	//ע������
	qRegisterMetaType<vector<float>>("vector<float>");
	qRegisterMetaType<vector<int>>("vector<int>");

	connect(openFileButton, SIGNAL(clicked()), this, SLOT(slot_OpenFile()));									//�������ļ�
	connect(musicListButton, SIGNAL(clicked()), this, SLOT(slot_OpenMusicList()));								//�������б����
	connect(lrcButton, SIGNAL(clicked()), this, SLOT(slot_OpenMusicLrc()));										//�򿪸��
	connect(showButton, SIGNAL(clicked()), this, SLOT(slot_OpenSearchPanel()));									//�򿪸������ؽ���
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(slot_TrayIconActivated(QSystemTrayIcon::ActivationReason)));									//����ͼ�걻�������д���
	connect(playList, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(slot_TableDoubleClicked(int, int)));		//������˫�������б�
	connect(playList, SIGNAL(sig_PlayListClean()), this, SLOT(slot_ClearSources()));							//���ý��Դ
	connect(playList, SIGNAL(sig_RowSelected(int, QString)), this, SLOT(slot_DelSelectSource(int, QString)));	//ɾ��ѡ�е�Դ
	connect(playList, SIGNAL(sig_SharedCurrentMusic(QString)), this, SLOT(slot_SharedCurrentMusic(QString)));	//����ø���
	connect(lrc, SIGNAL(sig_LrcLocked()), this, SLOT(slot_ShowLrcLocked()));									//��ʾ����Ѿ�������Ϣ
	connect(lrc, SIGNAL(sig_LrcUnlocked()), this, SLOT(slot_ShowLrcUnlocked()));								//��ʾ����Ѿ�������Ϣ
	connect(unlockLrcAction, SIGNAL(triggered()), this, SLOT(slot_UnlockLrc()));								//�������
	connect(playButton, SIGNAL(clicked()), this, SLOT(slot_SetPlayPaused()));									//����/��ͣ
	connect(endButton, SIGNAL(clicked()), this, SLOT(slot_SetStop()));											//ֹͣ
	connect(preButton, SIGNAL(clicked()), this, SLOT(slot_SetPre()));											//��һ��
	connect(nextButton, SIGNAL(clicked()), this, SLOT(slot_SetNext()));											//��һ��
	connect(minAction, SIGNAL(triggered()), this, SLOT(slot_MinToTray()));										//��С��
	connect(closeAction, SIGNAL(triggered()), this, SLOT(slot_CloseAllProgress()));								//�˳�����
	connect(quitAction, SIGNAL(triggered()), this, SLOT(slot_CloseAllProgress()));								//�˳�����
	connect(pauseAction, SIGNAL(triggered()), this, SLOT(slot_SetPlayPaused()));								//��ͣ(����)
	connect(stopAction, SIGNAL(triggered()), this, SLOT(slot_SetStop()));										//ֹͣ
	connect(lastAction, SIGNAL(triggered()), this, SLOT(slot_SetPre()));										//��һ��
	connect(nextAction, SIGNAL(triggered()), this, SLOT(slot_SetNext()));										//��һ��
	connect(timer, SIGNAL(timeout()), this, SLOT(slot_TimeOut()));												//����һ����ʱ��
	connect(shortCutIimer, SIGNAL(timeout()), this, SLOT(slot_ShortCutsetDisable()));							//����ȫ���ȼ�����/����
	connect(upDateTimer, SIGNAL(timeout()), this, SLOT(slot_UpdateTime()));										//����ʱ��͸��
	connect(openFileAction, SIGNAL(triggered()), this, SLOT(slot_OpenFile()));									//�������ļ�
	connect(openMusicListAction, SIGNAL(triggered()), this, SLOT(slot_OpenMusicList()));						//�����ֲ����б�
	connect(openSearchPanelAnimation, SIGNAL(triggered()), this, SLOT(slot_OpenSearchPanel()));					//�����������б�
	connect(openLrcAction, SIGNAL(triggered()), this, SLOT(slot_OpenMusicLrc()));								//��ʾ������
	connect(configAction, SIGNAL(triggered()), this, SLOT(slot_ConfigCZPlayer()));								//����
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(slot_AboutCZPlayer()));								//����CZPlayer
	connect(aboutQtAction, SIGNAL(triggered()), this, SLOT(slot_AboutQt()));									//����Qt
	connect(minMusicWidget ,SIGNAL(sig_ShowMusicPanel()), this, SLOT(slot_ShowMusicPanel()));					//��ʾ������
	connect(this, SIGNAL(sig_ShowMainWidget()), minMusicWidget, SLOT(slot_OpenMainWidget()));					//���͵����������ʾ������
	connect(volumeButton, SIGNAL(clicked()), this, SLOT(slot_SetMute()));										//���þ���
    connect(volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(slot_SetVolume(int)));							//��������
	connect(modeButton, SIGNAL(clicked()), this, SLOT(slot_ShowModeMenu()));									//��ʾģʽ�˵�
	connect(cycleAction, SIGNAL(triggered()), this, SLOT(slot_CyclePlay()));									//ѭ������
	connect(randomAction, SIGNAL(triggered()), this, SLOT(slot_RandomPlay()));									//�������
	connect(this, SIGNAL(sig_spectrumChanged(vector<float>)),m_spectrograph, SLOT(slot_spectrumChanged(vector<float>)));	//Ƶ�׸ı�
}

//����һ����ʱ��
void Widget::slot_TimeOut()
{
    if (lrc->text().isEmpty())
    {
        lrc->stopLrcMask();
    }

    //��������б�Ϊ�գ��򲥷Ű�ť������
	if (playList->rowCount() == 0)
	{
		playButton->setEnabled(false);
		endButton->setEnabled(false);
		preButton->setEnabled(false);
		nextButton->setEnabled(false);

		minMusicWidget->minPlayButton->setEnabled(false);
		minMusicWidget->minPauseButton->setEnabled(false);
		minMusicWidget->minStopButton->setEnabled(false);
		minMusicWidget->minNextButton->setEnabled(false);
		minMusicWidget->minLastButton->setEnabled(false);

		pauseAction->setIcon(QIcon(":/app/images/trayPlay.png"));
		pauseAction->setText(tr("����"));
		pauseAction->setEnabled(false);
		stopAction->setEnabled(false);
		lastAction->setEnabled(false);
		nextAction->setEnabled(false);
	}
	else
	{
		playButton->setEnabled(true);
		endButton->setEnabled(true);
		preButton->setEnabled(true);
		nextButton->setEnabled(true);

		minMusicWidget->minPlayButton->setEnabled(true);
		minMusicWidget->minPauseButton->setEnabled(true);
		minMusicWidget->minStopButton->setEnabled(true);
		minMusicWidget->minNextButton->setEnabled(true);
		minMusicWidget->minLastButton->setEnabled(true);

		pauseAction->setEnabled(true);
		stopAction->setEnabled(true);
		lastAction->setEnabled(true);
		nextAction->setEnabled(true);
	}

	if (nPos == -500)
	{
		nPos = 400;
	}
	currentMusicLabel->setGeometry(nPos, 7, 500, 11);
	nPos = nPos - 1;
}

//ѡ������
void Widget::slot_ConfigOptions()
{
    if (configDialog->isHidden())
    {
        this->slot_ConfigCZPlayer();
    }
}

//��ʾ/����������
void Widget::slot_ShowHideMainWidget()
{
    this->slot_MinToTray();
    if (this->isHidden())
    {
        this->slot_TrayIconActivated(QSystemTrayIcon::Trigger);
    }
}

//����ģʽ/��������
void Widget::slot_MinMainWidget()
{
    this->slot_OpenMinWidget();
    if (this->isHidden())
    {
        emit sig_ShowMainWidget();//��ʾ������
    }
}

//���ļ�
void Widget::slot_OpenFile()
{
    //����ͬʱ�򿪶����Ƶ�ļ�
#ifdef _WIN32_WINNT
    QString WINPATH = CZPlayerConfig::getValue("MUSICDIR_WIN").toString();
    QStringList list = QFileDialog::getOpenFileNames(this, tr("�������ļ�"),
                                                     WINPATH, tr("music Files(*.mp3 *.wma *.wav *.ogg)"));
#else
    QString X11PATH =  QDir::homePath() + CZPlayerConfig::getValue("MUSICDIR_X11").toString();
    QStringList list = QFileDialog::getOpenFileNames(this, tr("�������ļ�"),
                                                     X11PATH, tr("music Files(*.mp3 *.wma *.wav *.ogg)"));
#endif

    if (list.isEmpty())
    {
        return;
    }
	
	//�������ظ����б��߳�
	LoadMusicThread *loadMusicThread = new LoadMusicThread;
	connect(loadMusicThread, SIGNAL(sig_LoadMusicListFinished()), this, SLOT(slot_LoadMusicListFinished()));
	connect(loadMusicThread, SIGNAL(sig_LoadMusicListError()), this, SLOT(slot_LoadMusicListError()));
	loadMusicThread->setParam(list, &m_mapMusicRows, &m_mapMusicFilePath, playList);
	loadMusicThread->start();
}

//��Ӳ����б�
void Widget::slot_AddMusicList()
{
	//ֻ�ܴ�һ�������б��ļ�
#ifdef _WIN32_WINNT
	QString WINPATH = CZPlayerConfig::getValue("MUSICDIR_WIN").toString();
	QString fileName = QFileDialog::getOpenFileName(this, tr("���벥���б�"),
		WINPATH, tr("music List(*.m3u *.pls)"));
#else
	QString X11PATH =  QDir::homePath() + CZPlayerConfig::getValue("MUSICDIR_X11").toString();
	QString fileName = QFileDialog::getOpenFileName(this, tr("���벥���б�"),
		WINPATH, tr("music List(*.m3u *.pls)"));
#endif

	if (fileName != "")
	{
		//�������ظ����б��߳�
		LoadMusicThread *loadMusicThread = new LoadMusicThread;
		connect(loadMusicThread, SIGNAL(sig_LoadMusicListFinished()), this, SLOT(slot_LoadMusicListFinished()));
		connect(loadMusicThread, SIGNAL(sig_LoadMusicListError()), this, SLOT(slot_LoadMusicListError()));
		loadMusicThread->setParam(fileName, &m_mapMusicRows, &m_mapMusicFilePath, playList);
		loadMusicThread->start();
	}
}

//����С����
void Widget::slot_OpenMinWidget()
{
    if (minMusicWidget->isHidden())
    {
        QPropertyAnimation *minWidgetdAnimation = new QPropertyAnimation(minMusicWidget, "windowOpacity");
        minWidgetdAnimation->setDuration(500);
        minWidgetdAnimation->setStartValue(0);
        minWidgetdAnimation->setEndValue(1);
        minWidgetdAnimation->start();
        minMusicWidget->show();

        QPropertyAnimation *mainWidgetdAnimation = new QPropertyAnimation(this, "windowOpacity");
        mainWidgetdAnimation->setDuration(500);
        mainWidgetdAnimation->setStartValue(1);
        mainWidgetdAnimation->setEndValue(0);
        mainWidgetdAnimation->start();
        connect(mainWidgetdAnimation, SIGNAL(finished()), this, SLOT(slot_HideMainWidget()));

        QPropertyAnimation *searchPanelAnimation = new QPropertyAnimation(searchPanel, "windowOpacity");
        searchPanelAnimation->setDuration(500);
        searchPanelAnimation->setStartValue(1);
        searchPanelAnimation->setEndValue(0);
        searchPanelAnimation->start();
        connect(searchPanelAnimation, SIGNAL(finished()), this, SLOT(slot_HideSearchPanel()));

        QPropertyAnimation *musicListAnimation = new QPropertyAnimation(musicListWidget, "windowOpacity");
        musicListAnimation->setDuration(500);
        musicListAnimation->setStartValue(1);
        musicListAnimation->setEndValue(0);
        musicListAnimation->start();
        connect(musicListAnimation, SIGNAL(finished()), this, SLOT(slot_HideMusicList()));
    }
}

//�򿪲����б�
void Widget::slot_OpenMusicList()
{
    if (musicListWidget->isHidden())
    {
        musicListWidget->move(frameGeometry().bottomLeft());//��ʾ����������·�
        QPropertyAnimation *musicListAnimation = new QPropertyAnimation(musicListWidget, "windowOpacity");
        musicListAnimation->setDuration(500);
        musicListAnimation->setStartValue(0);
        musicListAnimation->setEndValue(1);
        musicListAnimation->start();
        musicListWidget->show();
        openMusicListAction->setText(tr("�رղ����б�"));
        musicListShowFlag = 1;
    }
    else
    {
        QPropertyAnimation *musicListAnimation = new QPropertyAnimation(musicListWidget, "windowOpacity");
        musicListAnimation->setDuration(500);
        musicListAnimation->setStartValue(1);
        musicListAnimation->setEndValue(0);
        musicListAnimation->start();
        connect(musicListAnimation, SIGNAL(finished()), this, SLOT(slot_HideMusicList()));
        openMusicListAction->setText(tr("�򿪲����б�"));
        musicListShowFlag = 0;
    }
}

//�򿪸��
void Widget::slot_OpenMusicLrc()
{
    if (lrc->isHidden())
    {
        //lrc->show();
        lrc->showNormal();
        openLrcAction->setText(tr("�ر�������"));
        minMusicWidget->minMusicLrcAction->setText(tr("�ر�������"));
    }
    else
    {
        lrc->hide();
        openLrcAction->setText(tr("��ʾ������"));
        minMusicWidget->minMusicLrcAction->setText(tr("��ʾ������"));
    }
}

//�����ظ�������
void Widget::slot_OpenSearchPanel()
{
    if (searchPanel->isHidden())
    {
        searchPanel->move(frameGeometry().topRight());//��ʾ����������ҷ�
        QPropertyAnimation *searchPanelAnimation = new QPropertyAnimation(searchPanel, "windowOpacity");
        searchPanelAnimation->setDuration(500);
        searchPanelAnimation->setStartValue(0);
        searchPanelAnimation->setEndValue(1);
        searchPanelAnimation->start();
        searchPanel->show();
        openSearchPanelAnimation->setText(tr("�ر��������"));
        showButton->setToolTip(tr("�ر��������"));
        showButton->setStyleSheet("QPushButton:!hover{border-image: url(:/app/images/hideButton.png);}"
                                   "QPushButton:hover{border-image: url(:/app/images/hideButton2.png);}"
                                   "QPushButton:pressed{border-image: url(:/app/images/hideButton3.png);}");
        musicDownloadShowFlag = 1;
    }
    else
    {
        QPropertyAnimation *searchPanelAnimation = new QPropertyAnimation(searchPanel, "windowOpacity");
        searchPanelAnimation->setDuration(500);
        searchPanelAnimation->setStartValue(1);
        searchPanelAnimation->setEndValue(0);
        searchPanelAnimation->start();
        connect(searchPanelAnimation, SIGNAL(finished()), this, SLOT(slot_HideSearchPanel()));
        openSearchPanelAnimation->setText(tr("���������"));
        showButton->setToolTip(tr("���������"));
        showButton->setStyleSheet("QPushButton:!hover{border-image: url(:/app/images/showButton.png);}"
                                   "QPushButton:hover{border-image: url(:/app/images/showButton2.png);}"
                                   "QPushButton:pressed{border-image: url(:/app/images/showButton3.png);}");
        musicDownloadShowFlag = 0;
    }
}

//������������
void Widget::slot_HideSearchPanel()
{
    searchPanel->hide();
}

//�������ֲ������б�
void Widget::slot_HideMusicList()
{
    musicListWidget->hide();
}

//����������
void Widget::slot_HideMainWidget()
{
    this->hide();
}

//��ʾ������
void Widget::slot_ShowMusicPanel()
{
    if (musicDownloadShowFlag == 1)
    {
        this->slot_OpenSearchPanel();
    }
    if (musicListShowFlag == 1)
    {
        this->slot_OpenMusicList();
    }
}

//����
void Widget::slot_ConfigCZPlayer()
{
    ConfigDialog::contentsWidget->setCurrentRow(0);//ѡ�е�һҳ
	ClientBaseOperating::initConfigDialog();//��ʼ�����öԻ���
    configDialog->exec();//ģ̬��ʾ�Ի���
}

//����Qt
void Widget::slot_AboutQt()
{
    QMessageBox::aboutQt(this, "����Qt");
}

//����CZPlayer
void Widget::slot_AboutCZPlayer()
{
	AboutPanel::tabWidget->setCurrentIndex(0);
	aboutPanel->exec();//ģ̬��ʾ�Ի���
}

//����LRC��ʣ���stateChanged()������State::Play��(������ǰ���Ÿ�ĸ��)
//��slot_Finished()�����е����˸ú���(��ʼ������һ�׸�ĸ��)
void Widget::resolveLrc()
{
	lrcMap.clear();

	//����ר��ͼƬ
	if (m_strCurrentMusicName.contains(" - "))//�������ַ�������ʽΪ��������� - �ƼҾԣ�
	{
		//��ר��ͼƬ
		QStringList ablumList = m_strCurrentMusicName.split(" - ");
		QString name = ablumList.at(1);//������
		QString artist = ablumList.at(0);//����

#ifdef _WIN32_WINNT
		QString albumImagesName = CZPlayerConfig::getValue("ALBUMDIR_WIN").toString();
#else
		QString albumImagesName = QDir::homePath() + CZPlayerConfig::getValue("ALBUMDIR_X11").toString();
#endif

		QString albumFileName = albumImagesName + "/" + m_strCurrentMusicName + ".jpg";
		QFile fileAlbum(albumFileName);
		if (fileAlbum.open(QIODevice::ReadOnly))//���ļ��ɹ�
		{
			//�������س���û��������
			if (fileAlbum.size() <  4500)
			{
				//����ר��ͼƬ
				AlbumThread *albumThread = new AlbumThread(m_strCurrentMusicName, name, artist, albumImage);
				connect(albumThread, SIGNAL(sig_AlbumDownloadFinished(QString, QString)), this, SLOT(slot_AlbumDownloadFinished(QString, QString)));
				albumThread->start();//����һ���߳�
			}
			else
			{
				//albumImage->setPixmap(QPixmap(albumFileName));
				//��ͼƬת����label��С����ʾ
				QImage image(albumFileName);
				QImage newImg = image.scaled(albumImage->width(), albumImage->height());
				albumImage->setPixmap(QPixmap::fromImage(newImg));
			}
		}
		else
		{
			//����ר��ͼƬ
			AlbumThread *albumThread = new AlbumThread(m_strCurrentMusicName, name, artist, albumImage);
			connect(albumThread, SIGNAL(sig_AlbumDownloadFinished(QString, QString)), this, SLOT(slot_AlbumDownloadFinished(QString, QString)));
			albumThread->start();//����һ���߳�
		}
	}
	else
	{
		albumImage->setPixmap(QPixmap(tr(":/app/images/albumImage.png")));
	}

#ifdef _WIN32_WINNT
	QString lrcDirName = CZPlayerConfig::getValue("LRCDIR_WIN").toString();
#else
	QString lrcDirName = QDir::homePath() + CZPlayerConfig::getValue("LRCDIR_X11").toString();
#endif
	QString lrcFileName = lrcDirName + "/" + m_strCurrentMusicName + ".lrc";
	m_lrcFileName = lrcFileName;

	//�򿪸��
	QFile file(lrcFileName);
	if (!file.open(QIODevice::ReadOnly))//���ظ����ļ�
	{
		lrc->setText(m_strCurrentMusicName + tr("----�������ظ���ļ�!"));

		if (m_strCurrentMusicName.contains(" - "))
		{
			QStringList list = m_strCurrentMusicName.split(" - ");
			QString musicName = list.at(1);//������
			QString musicArtist = list.at(0);//����

			//���ظ��
			LrcThread *lrcThread = new LrcThread(musicName, musicArtist, m_lrcFileName, lrc);
			lrcThread->start();//����һ���߳�
			connect(lrcThread, SIGNAL(sig_ResolveLrc()), this, SLOT(slot_ResolveLrc()));
			return;
		}
	}

	/*�������*/

	QString allText = QString(file.readAll());
	file.close();

	//����ʰ��з�Ϊ����б�
	QStringList lrcLines = allText.split("\n");

	//�����ʱ���ǩ�ĸ�ʽ[00:05.54]
	//������ʽ\d{2}��ʾƥ����������
	QRegExp rx("\\[\\d{2}:\\d{2}\\.\\d{2}\\]");

	foreach (QString oneLine, lrcLines)
	{
		QString temp = oneLine;
		temp.replace(rx, "");//�ÿ��ַ����滻������ʽ����ƥ��ĵط�,�����ͻ���˸���ı�

		// Ȼ�����λ�ȡ��ǰ���е�����ʱ���ǩ�����ֱ������ı�����QMap��
		//indexIn()Ϊ���ص�һ��ƥ���λ�ã��������Ϊ-1�����ʾû��ƥ��ɹ�
		//���������pos����Ӧ�ö�Ӧ���Ǹ���ļ�

		int pos = rx.indexIn(oneLine, 0);
		while (pos != -1)//��ʾƥ��ɹ�
		{
			QString cap = rx.cap(0);//���ص�0�����ʽƥ�������

			//��ʱ���ǩת��Ϊʱ����ֵ���Ժ���Ϊ��λ
			QRegExp regexp;

			//��÷���
			regexp.setPattern("\\d{2}(?=:)");
			regexp.indexIn(cap);
			int minute = regexp.cap(0).toInt();

			//�����
			regexp.setPattern("\\d{2}(?=\\.)");
			regexp.indexIn(cap);
			int second = regexp.cap(0).toInt();

			//��ú���
			regexp.setPattern("\\d{2}(?=\\])");
			regexp.indexIn(cap);
			int milliseSecond = regexp.cap(0).toInt();

			//�����ʱ��
			long long totalTime = minute * 60000 + second * 1000 + milliseSecond * 10;

			//��ÿһ�е�ʱ��͸�ʲ��뵽lrcMap��
			lrcMap.insert(totalTime, temp);
			pos += rx.matchedLength();
			pos = rx.indexIn(oneLine, pos);//ƥ��ȫ��
		}
	}

	//���lrcMapΪ��
	if (lrcMap.isEmpty())
	{
		lrc->setText(m_strCurrentMusicName + tr("----����ļ����ݴ���!"));
		return;
	}
}

//���ý������
void Widget::slot_ResolveLrc()
{
	this->resolveLrc();//������ǰ���ظ��
}

//����/��ͣ
void Widget::slot_SetPlayPaused()
{
	if (m_state == Widget::Play)							//��ǰ״̬Ϊ����״̬
	{
		this->pause();										//��ͣ
		this->setState(Pause);								//���ò���״̬
	}
	else if (m_state == Widget::Pause)						//��ǰ״̬Ϊ��ͣ״̬
	{
		this->play();										//����
		this->setState(Play);								//���ò���״̬
	}
	else if (m_state == Widget::Stop)						//��ǰ״̬Ϊֹͣ״̬
	{
		this->reset();										//����
		QFile file(m_strCurrentFilePath);
		if (file.exists())
		{
			this->play(m_strCurrentFilePath);
			this->setState(Play);							//����״̬
		}
		else
		{
			QMessageBox::information(NULL, tr("��Ϣ"), tr("��Ч����·����"), QMessageBox::Yes);
			return;
		}
	}
	else if (m_state == Widget::NoState)				//��ǰ״̬Ϊ��״̬
	{
		QTableWidgetItem *item = playList->item(0, 1);
		map<QString, QString>::iterator iter2 = m_mapMusicFilePath.find(item->text());
		if (iter2 != m_mapMusicFilePath.end())
		{
			this->reset();									//����
			nTime = 0;				

			QFile file(iter2->second);
			if (file.exists())
			{
				this->play(iter2->second, item->text(), 0, playList->item(0, 2)->text());
			}
			else
			{
				QMessageBox::information(NULL, tr("��Ϣ"), tr("��Ч����·����"), QMessageBox::Yes);
				return;
			}
		}
	}
	this->stateChanged();								//״̬�ı䣬���½���
}

//����
void Widget::slot_SetPlay()
{
	if (m_state == Widget::Pause)						//��ǰ״̬Ϊ��ͣ״̬
	{
		this->play();									//����
		this->setState(Play);							//���ò���״̬
	}
	else if (m_state == Widget::Stop)					//��ǰ״̬Ϊֹͣ״̬
	{
		this->reset();									//����
		QFile file(m_strCurrentFilePath);
		if (file.exists())
		{
			this->play(m_strCurrentFilePath);			//����
			this->setState(Play);						//����״̬
		}
		else
		{
			QMessageBox::information(NULL, tr("��Ϣ"), tr("��Ч����·����"), QMessageBox::Yes);
			return;
		}
	}
	else if (m_state == Widget::NoState)				//��ǰ״̬Ϊ��״̬
	{
		QTableWidgetItem *item = playList->item(0, 1);
		map<QString, QString>::iterator iter2 = m_mapMusicFilePath.find(item->text());
		if (iter2 != m_mapMusicFilePath.end())
		{
			this->reset();									//����
			nTime = 0;

			QFile file(iter2->second);
			if (file.exists())
			{
				this->play(iter2->second, item->text(), 0, playList->item(0, 2)->text());
			}
			else
			{
				QMessageBox::information(NULL, tr("��Ϣ"), tr("��Ч����·����"), QMessageBox::Yes);
				return;
			}
		}
	}
	this->stateChanged();								//״̬�ı䣬���½���
}

//��ͣ
void Widget::slot_SetPause()
{
	if (m_state == Widget::Play)						//��ǰ״̬Ϊ����״̬
	{
		this->pause();									//��ͣ����
		this->setState(Pause);							//���ò���״̬
	}
	this->stateChanged();								//״̬�ı䣬���½���
}

//ֹͣ����
void Widget::slot_SetStop()
{
	if (m_state == Widget::Play || m_state == Widget::Pause)
	{
		this->reset();
		this->setState(Stop);
		this->stateChanged();							//״̬�ı䣬���½���
	}
}

//������һ��
void Widget::slot_SetPre()
{
	//ѭ������ģʽ
	if (m_mode == Widget::cyclePlay)
	{
		if (m_nCurrentMusicRow - 1 != -1)
		{
			QTableWidgetItem *item = playList->item(m_nCurrentMusicRow - 1, 1);
			map<QString, QString>::iterator iter2 = m_mapMusicFilePath.find(item->text());
			if (iter2 != m_mapMusicFilePath.end())
			{
				this->reset();									//����
				nTime = 0;

				QFile file(iter2->second);
				if (file.exists())
				{
					this->play(iter2->second, item->text(), m_nCurrentMusicRow - 1, playList->item(m_nCurrentMusicRow - 1, 2)->text());
				}
				else
				{
					QMessageBox::information(NULL, tr("��Ϣ"), tr("��Ч����·����"), QMessageBox::Yes);
					return;
				}
			}
		}
		else	//�������һ��
		{
			QTableWidgetItem *item = playList->item(playList->rowCount() - 1, 1);
			map<QString, QString>::iterator iter2 = m_mapMusicFilePath.find(item->text());
			if (iter2 != m_mapMusicFilePath.end())
			{
				this->reset();									//����
				nTime = 0;

				QFile file(iter2->second);
				if (file.exists())
				{
					this->play(iter2->second, item->text(), playList->rowCount() - 1, playList->item(playList->rowCount() - 1, 2)->text());
				}
				else
				{
					QMessageBox::information(NULL, tr("��Ϣ"), tr("��Ч����·����"), QMessageBox::Yes);
					return;
				}
			}
		}
	}
	//�������
	else if (m_mode == Widget::randomPlay)
	{
		int nRow = qrand() % playList->rowCount();				//����������ŵ�����
		QTableWidgetItem *item = playList->item(nRow, 1);
		map<QString, QString>::iterator iter2 = m_mapMusicFilePath.find(item->text());
		if (iter2 != m_mapMusicFilePath.end())
		{
			this->reset();									//����
			nTime = 0;				

			QFile file(iter2->second);
			if (file.exists())
			{
				this->play(iter2->second, item->text(), nRow, playList->item(nRow, 2)->text());
			}
			else
			{
				QMessageBox::information(NULL, tr("��Ϣ"), tr("��Ч����·����"), QMessageBox::Yes);
				return;
			}
		}
	}
}

//������һ��
void Widget::slot_SetNext()
{
	//ѭ������ģʽ
	if (m_mode == Widget::cyclePlay)
	{
		if (m_nCurrentMusicRow + 1 != playList->rowCount())
		{
			QTableWidgetItem *item = playList->item(m_nCurrentMusicRow + 1, 1);
			map<QString, QString>::iterator iter2 = m_mapMusicFilePath.find(item->text());
			if (iter2 != m_mapMusicFilePath.end())
			{
				this->reset();									//����
				nTime = 0;				

				QFile file(iter2->second);
				if (file.exists())
				{
					this->play(iter2->second, item->text(), m_nCurrentMusicRow + 1, playList->item(m_nCurrentMusicRow + 1, 2)->text());
				}
				else
				{
					QMessageBox::information(NULL, tr("��Ϣ"), tr("��Ч����·����"), QMessageBox::Yes);
					return;
				}
			}
		}
		else	//����ѭ���б�
		{
			QTableWidgetItem *item = playList->item(0, 1);
			map<QString, QString>::iterator iter2 = m_mapMusicFilePath.find(item->text());
			if (iter2 != m_mapMusicFilePath.end())
			{
				this->reset();									//����
				nTime = 0;				

				QFile file(iter2->second);
				if (file.exists())
				{
					this->play(iter2->second, item->text(), 0, playList->item(0, 2)->text());
				}
				else
				{
					QMessageBox::information(NULL, tr("��Ϣ"), tr("��Ч����·����"), QMessageBox::Yes);
					return;
				}
			}
		}
	}
	//�������
	else if (m_mode == Widget::randomPlay)
	{
		int nRow = qrand() % playList->rowCount();				//����������ŵ�����
		QTableWidgetItem *item = playList->item(nRow, 1);
		map<QString, QString>::iterator iter2 = m_mapMusicFilePath.find(item->text());
		if (iter2 != m_mapMusicFilePath.end())
		{
			this->reset();									//����
			nTime = 0;			

			QFile file(iter2->second);
			if (file.exists())
			{
				this->play(iter2->second, item->text(), nRow, playList->item(nRow, 2)->text());
			}
			else
			{
				QMessageBox::information(NULL, tr("��Ϣ"), tr("��Ч����·����"), QMessageBox::Yes);
				return;
			}
		}
	}
}

//������˫�����ֲ����б�ѡ����
void Widget::slot_TableDoubleClicked(int row, int /*column*/)
{
	QTableWidgetItem *item = playList->item(row, 1);
	map<QString, QString>::iterator iter = m_mapMusicFilePath.find(item->text());
	if (iter != m_mapMusicFilePath.end())
	{
		this->reset();									//����
		nTime = 0;

		QFile file(iter->second);
		if (file.exists())
		{
			this->play(iter->second, item->text(), row, playList->item(row, 2)->text());
		}
		else
		{
			QMessageBox::information(NULL, tr("��Ϣ"), tr("��Ч����·����"), QMessageBox::Yes);
			return;
		}
	}
}

//ѡ�е�ǰ����ý��Դ����
void Widget::rowChanged()
{
	playList->selectRow(m_nCurrentMusicRow);
}

//���ý��Դ����ɾ�����ݿ�ȫ�����ݡ�map�Լ�ȫ��ר��
void Widget::slot_ClearSources()
{
	//ֹͣ��ǰ����
	this->slot_SetStop();

	if (!DBModule::clearDB())//������ݿ�
	{
		QMessageBox::information(NULL, tr("��Ϣ"), tr("������ݿ�ʧ�ܣ�"), QMessageBox::Yes);
	}

    albumImage->setPixmap(QPixmap(tr(":/app/images/albumImage.png")));
	currentMusicLabel->setText(tr("Welecome to CZPlayer!"));
	m_mapMusicRows.clear();
	m_mapMusicFilePath.clear();

	//ɾ��ר���ļ��������ȫ��ר��
#ifdef _WIN32_WINNT
	QString albumImagesName = CZPlayerConfig::getValue("ALBUMDIR_WIN").toString();
#else
	QString albumImagesName = QDir::homePath() + CZPlayerConfig::getValue("ALBUMDIR_X11").toString();
#endif

	QDir dir(albumImagesName);
	QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot | QDir::NoSymLinks);
	foreach (QFileInfo fileInfo, fileInfoList)
	{
		if (fileInfo.isFile())
		{
			QFile file(fileInfo.absoluteFilePath());
			file.remove();
		}
	}
}

//ɾ�����ݿ���Ӧ������
void Widget::slot_DelSelectSource(int row, QString musicName)
{
    //���ɾ���ĵ�ǰ��Ϊ���ڲ��ŵ���һ�У���ֹͣ�����ֵĲ���
	if (row == m_nCurrentMusicRow)
	{
		this->slot_SetStop();
	}
	if (!DBModule::delLine(musicName))//ɾ��һ��
	{
		QMessageBox::information(NULL, tr("��Ϣ"), tr("ɾ���ø���ʧ�ܣ�"), QMessageBox::Yes);
		return;
	}
    //����map
    this->updateMap(musicName);
}

//����map
void Widget::updateMap(QString musicName)
{
	qDebug() << musicName;

    m_mapMusicRows.clear();
    for (int index = 0; index < playList->rowCount(); ++index)
    {
		//playList->item(index, 1)->text()Ϊ������
        m_mapMusicRows.insert(make_pair(playList->item(index, 1)->text(), index));
    }

	//ɾ���ø�����Ӧ��·��
	map<QString, QString>::iterator iter = m_mapMusicFilePath.find(musicName);
	if (iter != m_mapMusicFilePath.end())
	{
		m_mapMusicFilePath.erase(iter);
	}

	//ɾ��ר���ļ������������Ӧ��ר��
#ifdef _WIN32_WINNT
	QString albumImagesName = CZPlayerConfig::getValue("ALBUMDIR_WIN").toString();
#else
	QString albumImagesName = QDir::homePath() + CZPlayerConfig::getValue("ALBUMDIR_X11").toString();
#endif

	QString albumFileName = albumImagesName + "/" + musicName + ".jpg";
	qDebug() << albumFileName;
	remove(albumFileName.toStdString().c_str());
}

//��ý�岥�ſ����ʱ���ᷢ��aboutToFininsh()�ź�
//void Widget::slot_AboutMusicFinish()
//{
//    int index = sources.indexOf(mediaObject->currentSource()) + 1;
//    if (sources.size() > index)
//    {
//        mediaObject->setCurrentSource(sources.at(index));
//        mediaObject->play();
//        lrc->stopLrcMask();//ֹͣ�������
//        resolveLrc(sources.at(index).fileName());//������һ�׸�ĸ��
//    }
//    else
//    {
//        mediaObject->enqueue(sources.at(0));
//        mediaObject->play();
//        lrc->stopLrcMask();//ֹͣ�������
//        resolveLrc(sources.at(0).fileName());//������һ�׸�ĸ��
//        QString fileName = QFileInfo(sources.at(0).fileName()).baseName();
//    }
//}

//����״̬�ı�(���½���)
void Widget::stateChanged()
{
	//����״̬Ϊ����״̬ʱ������һЩ�ؼ���״̬
	if (m_state == Widget::Play)
	{
		playButton->setStyleSheet("QPushButton:!hover{border-image: url(:/app/images/stopButton.png);}"
			"QPushButton:hover{border-image: url(:/app/images/stopButton2.png);}"
			"QPushButton:pressed{border-image: url(:/app/images/stopButton3.png);}");
		playButton->setToolTip(tr("��ͣ"));
		pauseAction->setIcon(QIcon(":/app/images/trayPause.png"));
		pauseAction->setText(tr("��ͣ"));
		InfoLabel->setVisible(false);
		m_spectrograph->setVisible(true);
		trayIcon->setToolTip(tr("���ڲ���:%1").arg(m_strCurrentMusicName));
		currentMusicLabel->setText(tr("���ڲ��ţ�") + m_strCurrentMusicName);
		this->resolveLrc();//�������ڲ��ŵĸ����ĸ��
	}
	else if (m_state == Widget::Pause)	//��״̬Ϊ��ͣ״̬
	{
		playButton->setStyleSheet("QPushButton:!hover{border-image: url(:/app/images/playButton.png);}"
			"QPushButton:hover{border-image: url(:/app/images/playButton2.png);}"
			"QPushButton:pressed{border-image: url(:/app/images/playButton3.png);}");
		playButton->setToolTip(tr("����"));
		pauseAction->setIcon(QIcon(":/app/images/trayPlay.png"));
		pauseAction->setText(tr("����"));
		InfoLabel->setText(false);
		m_spectrograph->setVisible(true);
		trayIcon->setToolTip(tr("CZPlayer"));
		currentMusicLabel->setText(tr("��ͣ���ţ�") + m_strCurrentMusicName);
		lrc->stopLrcMask();
		lrc->setText(m_strCurrentMusicName);
	}
	else if (m_state == Widget::Stop)	//��״̬Ϊֹͣ����
	{
		playButton->setStyleSheet("QPushButton:!hover{border-image: url(:/app/images/playButton.png);}"
			"QPushButton:hover{border-image: url(:/app/images/playButton2.png);}"
			"QPushButton:pressed{border-image: url(:/app/images/playButton3.png);}");
		playButton->setToolTip(tr("����"));
		pauseAction->setIcon(QIcon(":/app/images/trayPlay.png"));
		pauseAction->setText(tr("����"));
		m_spectrograph->setVisible(false);
		InfoLabel->setVisible(true);
		trayIcon->setToolTip(tr("CZPlayer"));
		InfoLabel->setText(tr("Welcome to CZPlayer!"));
		currentMusicLabel->setText(tr("ֹͣ���ţ�") + m_strCurrentMusicName);
		lrc->stopLrcMask();
		lrc->setText(tr("CZPlayer"));
	}
	this->slot_UpdateTime();	//����ʱ��͸��
}

//����ʱ��͸��
void Widget::slot_UpdateTime()
{
	//����״̬Ϊ����״̬ʱ������һЩ�ؼ���״̬
	if (m_state == Widget::Play)
	{
		QTime qTime;
		int ntotalTime = qTime.secsTo(QTime::fromString(m_strCurrentMusicTime, "mm:ss"));
		//���¹�����
		seekSlider->setRange(0, ntotalTime);
		seekSlider->setValue(nTime);

		//����ʱ��
		 QTime currentTime(0, nTime / 60, nTime % 60);
		 timeLabel->setText(currentTime.toString("mm:ss") + "/" + m_strCurrentMusicTime);
		 if (currentTime.toString("mm:ss") != m_strCurrentMusicTime)
		 {
			++nTime;
		 }
	}
	else if (m_state == Widget::Pause)	//��״̬Ϊ��ͣ״̬
	{
		//���¹�����
		QTime qTime;
		int ntotalTime = qTime.secsTo(QTime::fromString(m_strCurrentMusicTime, "mm:ss"));
		seekSlider->setRange(0, ntotalTime);
		seekSlider->setValue(nTime);

		//����ʱ��
		QTime currentTime(0, nTime / 60, nTime % 60);
		timeLabel->setText(currentTime.toString("mm:ss") + "/" + m_strCurrentMusicTime);
	}
	else if (m_state == Widget::Stop)	//��״̬Ϊֹͣ����
	{
		seekSlider->setValue(0);
		timeLabel->setText(tr("00:00/00:00"));
		nTime = 0;
	}

	//��ȡ��ǰʱ���Ӧ�ĸ��
	if (!lrcMap.isEmpty())
	{
		//��ȡ��ǰʱ���ڸ���е�ǰ������ʱ���
		long long previousTime = 0;
		long long laterTime = 0;

		//keys()��������lrcMap�б�
		foreach (long long value, lrcMap.keys())
		{
			if (nTime >= value / 1000)
			{
				previousTime = value;
			}
			else
			{
				laterTime = value;
				break;
			}
		}

		//��ȡ��ǰʱ������Ӧ�ĸ������
		QString currentLrc = lrcMap.value(previousTime);

		// ������µ�һ�и�ʣ���ô���¿�ʼ��ʾ�������
		if (currentLrc != lrc->text())
		{
			lrc->setText(currentLrc);
			long long intervalTime = laterTime - previousTime;//ʱ����
			lrc->startLrcMask(intervalTime);//�����������
		}
	}
	else
	{
		//lrc->setText(m_strCurrentMusicName + tr("----δ�ҵ�����ļ�!"));
		lrc->setText("Welcome to CZPlayer!");
	}
}

//��С��������
void Widget::slot_MinToTray()
{
    if (musicListWidget->isVisible())
    {
        QPropertyAnimation *musicListAnimation = new QPropertyAnimation(musicListWidget, "windowOpacity");
        musicListAnimation->setDuration(500);
        musicListAnimation->setStartValue(1);
        musicListAnimation->setEndValue(0);
        musicListAnimation->start();
        connect(musicListAnimation, SIGNAL(finished()), this, SLOT(slot_HideMusicList()));
    }
    if (searchPanel->isVisible())
    {
        QPropertyAnimation *searchPanelAnimation = new QPropertyAnimation(searchPanel, "windowOpacity");
        searchPanelAnimation->setDuration(500);
        searchPanelAnimation->setStartValue(1);
        searchPanelAnimation->setEndValue(0);
        searchPanelAnimation->start();
        connect(searchPanelAnimation, SIGNAL(finished()), this, SLOT(slot_HideSearchPanel()));
    }
    if (this->isVisible())
    {
        QPropertyAnimation *mainWidgetAnimation = new QPropertyAnimation(this, "windowOpacity");
        mainWidgetAnimation->setDuration(500);
        mainWidgetAnimation->setStartValue(1);
        mainWidgetAnimation->setEndValue(0);
        mainWidgetAnimation->start();
        connect(mainWidgetAnimation, SIGNAL(finished()), this, SLOT(slot_HideMainWidget()));
        trayIcon->showMessage(tr("CZPlayer"), tr("���������»ص�������"));
    }
}

//�ر����г���
void Widget::slot_CloseAllProgress()
{
    QPropertyAnimation *mainWidgetAnimation = new QPropertyAnimation(this, "windowOpacity");
    mainWidgetAnimation->setDuration(500);
    mainWidgetAnimation->setStartValue(1);
    mainWidgetAnimation->setEndValue(0);
    mainWidgetAnimation->start();
    connect(mainWidgetAnimation, SIGNAL(finished()), this, SLOT(close()));

    QPropertyAnimation *musicListAnimation = new QPropertyAnimation(musicListWidget, "windowOpacity");
    musicListAnimation->setDuration(500);
    musicListAnimation->setStartValue(1);
    musicListAnimation->setEndValue(0);
    musicListAnimation->start();
    connect(musicListAnimation, SIGNAL(finished()), this, SLOT(close()));

    QPropertyAnimation *searchPanelAnimation = new QPropertyAnimation(searchPanel, "windowOpacity");
    searchPanelAnimation->setDuration(500);
    searchPanelAnimation->setStartValue(1);
    searchPanelAnimation->setEndValue(0);
    searchPanelAnimation->start();
    connect(searchPanelAnimation, SIGNAL(finished()), this, SLOT(close()));

    QPropertyAnimation *lrcAnimation = new QPropertyAnimation(lrc, "windowOpacity");
    lrcAnimation->setDuration(500);
    lrcAnimation->setStartValue(1);
    lrcAnimation->setEndValue(0);
    lrcAnimation->start();
    connect(lrcAnimation, SIGNAL(finished()), this, SLOT(close()));

    minMusicWidget->close();
}

//ϵͳ����ͼ�걻����
void Widget::slot_TrayIconActivated(QSystemTrayIcon::ActivationReason activationReason)
{
    if (activationReason == QSystemTrayIcon::Trigger)
    {
        QPropertyAnimation *mainWidgetAnimation = new QPropertyAnimation(this, "windowOpacity");
        mainWidgetAnimation->setDuration(500);
        mainWidgetAnimation->setStartValue(0);
        mainWidgetAnimation->setEndValue(1);
        mainWidgetAnimation->start();
        this->show();//��ʾ������
        this->slot_ShowMusicPanel();
    }
}

//��С��
void Widget::slot_ShowMinSize()
{
    this->showMinimized();
    musicListWidget->showMinimized();
    searchPanel->showMinimized();
}

//��ʾ����Ѿ�������Ϣ
void Widget::slot_ShowLrcLocked()
{
    trayIcon->showMessage(tr("��ʾ"), tr("����������Ѿ�����,���������ͼ���Ҽ����Խ���"));
}

//��ʾ����Ѿ�������Ϣ
void Widget::slot_ShowLrcUnlocked()
{
    trayIcon->showMessage(tr("��ʾ"), tr("����������Ѿ�����"));
}

//�������
void Widget::slot_UnlockLrc()
{
    lrc->unlockLrc();//�������
}

//����ȫ���ȼ�����/����
void Widget::slot_ShortCutsetDisable()
{
	if (minMusicWidget->isHidden())
	{
		//���������ȼ�
		GlobalHotKey::showHideMainWidgetDisabled(false);
		GlobalHotKey::showDownloadDisabled(false);
		GlobalHotKey::showMusicListDisabled(false);
	}
	else
	{
		//���������ȼ�
		GlobalHotKey::showHideMainWidgetDisabled(true);
		GlobalHotKey::showDownloadDisabled(true);
		GlobalHotKey::showMusicListDisabled(true);
	}
}

//����������Ҽ�����¼�
void Widget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(closeAction);
    menu.addAction(minAction);
    menu.addAction(QIcon(":/app/images/minMainWidget.png"), tr("����ģʽ"), this, SLOT(slot_OpenMinWidget()));
    menu.addSeparator();
    menu.addAction(openFileAction);
    menu.addAction(openMusicListAction);
    menu.addAction(openSearchPanelAnimation);
    menu.addAction(openLrcAction);
    menu.addSeparator();
    menu.addAction(configAction);
	menu.addAction(aboutAction);
    menu.addAction(aboutQtAction);
    menu.exec(event->globalPos());//globalPos()Ϊ��ǰ����λ������
    //menu.exec(QCursor::pos());
    event->accept();
}

//�Ͻ��¼�
void Widget::dragEnterEvent(QDragEnterEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    foreach (QUrl url, urls)
    {
        QString filePath = url.toLocalFile();

		QFileInfo fileInfo(filePath);
		if (fileInfo.isFile())	//���Ͻ��ļ�
		{
			QString strSuffixName = filePath.right(3);
			if (strSuffixName == "mp3" || strSuffixName == "MP3" 
				|| strSuffixName == "wma" || strSuffixName == "WMA" 
				|| strSuffixName == "wav" || strSuffixName == "WAV" 
				|| strSuffixName == "ogg" || strSuffixName == "OGG"
				|| strSuffixName == "m3u" || strSuffixName == "M3U" 
				|| strSuffixName == "pls" || strSuffixName == "PLS")
			{
				event->accept();
			}
			else
			{
				event->ignore();
			}
		}
		else if (fileInfo.isDir())	//���Ͻ��ļ���
		{
			event->accept();
		}
    }
}

//�����¼�
void Widget::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
    {
        return;
    }

	//�������ظ����б��߳�
	LoadMusicThread *loadMusicThread = new LoadMusicThread;
	connect(loadMusicThread, SIGNAL(sig_LoadMusicListFinished()), this, SLOT(slot_LoadMusicListFinished()));
	connect(loadMusicThread, SIGNAL(sig_LoadMusicListError()), this, SLOT(slot_LoadMusicListError()));
	loadMusicThread->setParam(urls, &m_mapMusicRows, &m_mapMusicFilePath, playList);
	loadMusicThread->start();

    event->accept();
}

//��дpaintEvent,��ӱ���ͼƬ
void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap backgroundImage;
    backgroundImage.load(":/app/images/mainBg2.png");

    //��ͨ��pix�ķ������ͼƬ�Ĺ��˵�͸���Ĳ��ֵõ���ͼƬ����ΪloginPanel�Ĳ�����߿�
    this->setMask(backgroundImage.mask());
    painter.drawPixmap(0, 0, 400, 212, backgroundImage);

    event->accept();
}

//��дmousePressEvent��mouseMoveEventʵ�ִ�����ƶ�
void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

//��дmousePressEvent��mouseMoveEventʵ�ִ�����ƶ�
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        this->move(event->globalPos() - dragPosition);//�ƶ�����
        musicListWidget->move(frameGeometry().bottomLeft());//�ƶ������ʱ�����ֲ����б�����ƶ�
        searchPanel->move(frameGeometry().topRight());//�ƶ������ʱ���������ؽ�������ƶ�
        event->accept();
    }
}

//���ò���״̬
void Widget::setState( State state )
{
	m_state = state;
}

//���ò���ģʽ
void Widget::setMode( Mode mode )
{
	m_mode = mode;
}

//��������
void Widget::slot_SetVolume(int value)
{
	if (CZPlayerConfig::getValue("SETMUTE").toString() == "true")
	{
		volumeButton->setToolTip(tr("����"));
		volumeButton->setStyleSheet("QPushButton:!hover{border-image: url(:/app/images/soundButton.png);}"
			"QPushButton:hover{border-image: url(:/app/images/soundButton2.png);}"
			"QPushButton:pressed{border-image: url(:/app/images/soundButton3.png);}");
	}
	SoundControl::SetVolume(0, value);
	CZPlayerConfig::setValue("VOLUMEVALUE", QString::number(value));
	volumeSlider->setToolTip(tr("��ǰ������%1%").arg(value));
}

//���þ���
void Widget::slot_SetMute()
{
	if (CZPlayerConfig::getValue("SETMUTE").toString() == "false")
	{
		SoundControl::SetVolume(0, 0);	//����
		volumeButton->setToolTip(tr("�ָ�����"));
		volumeButton->setStyleSheet("QPushButton:!hover{border-image: url(:/app/images/soundButton4.png);}"
			"QPushButton:hover{border-image: url(:/app/images/soundButton5.png);}"
			"QPushButton:pressed{border-image: url(:/app/images/soundButton6.png);}");
		CZPlayerConfig::setValue("SETMUTE", "true");
	}
	else
	{
		SoundControl::SetVolume(0, volumeSlider->value());	//�ָ�
		volumeButton->setToolTip(tr("����"));
		volumeButton->setStyleSheet("QPushButton:!hover{border-image: url(:/app/images/soundButton.png);}"
			"QPushButton:hover{border-image: url(:/app/images/soundButton2.png);}"
			"QPushButton:pressed{border-image: url(:/app/images/soundButton3.png);}");
		CZPlayerConfig::setValue("SETMUTE", "false");
	}
}

//��ʾģʽ�˵�
void Widget::slot_ShowModeMenu()
{
	if (ClientBaseOperating::initPlayMode() == "cyclePlay")	//ѭ������
	{
		cycleAction->setText(tr("ѭ������ ��"));
		randomAction->setText(tr("�������"));
	}
	else if (ClientBaseOperating::initPlayMode() == "randomPlay")	//�������
	{
		cycleAction->setText(tr("ѭ������"));
		randomAction->setText(tr("������� ��"));
	}

	QMenu menu;
	menu.addAction(cycleAction);
	menu.addAction(randomAction);
	menu.exec(QCursor::pos());
}

//ѭ������
void Widget::slot_CyclePlay()
{
	CZPlayerConfig::setValue("PLAYMODE", "cyclePlay");
	cycleAction->setText(tr("ѭ������ ��"));
	randomAction->setText(tr("�������"));
	modeButton->setStyleSheet("QPushButton:!hover{border-image: url(:/app/images/cyclePlay.png);}"
		"QPushButton:hover{border-image: url(:/app/images/cyclePlay2.png);}"
		"QPushButton:pressed{border-image: url(:/app/images/cyclePlay3.png);}");
	modeButton->setToolTip(tr("ѭ������"));
	this->setMode(cyclePlay);					//����ѭ������ģʽ
}

//�������
void Widget::slot_RandomPlay()
{
	CZPlayerConfig::setValue("PLAYMODE", "randomPlay");
	cycleAction->setText(tr("ѭ������"));
	randomAction->setText(tr("������� ��"));
	modeButton->setStyleSheet("QPushButton:!hover{border-image: url(:/app/images/randomPlay.png);}"
		"QPushButton:hover{border-image: url(:/app/images/randomPlay2.png);}"
		"QPushButton:pressed{border-image: url(:/app/images/randomPlay3.png);}");
	modeButton->setToolTip(tr("�������"));
	this->setMode(randomPlay);					//�����������ģʽ
}

//ר����������ź�
void Widget::slot_AlbumDownloadFinished( QString strMusicName, QString strAlbumUrl )
{
	qDebug() << "strMusicName = " << strMusicName;
	qDebug() << "strAlbumUrl = " << strAlbumUrl;
	DBModule::updateAlbumUrlPath(strMusicName, strAlbumUrl);
}

//����ø���
void Widget::slot_SharedCurrentMusic( QString strMusicName )
{
	QString strAlbumUrlPath;
	DBModule::readAlbumUrlPathFromDB(strMusicName, strAlbumUrlPath);
	qDebug() << strAlbumUrlPath;

	if (sharedDialog->isHidden())
	{
#ifdef _WIN32_WINNT
		QString albumImagesName = CZPlayerConfig::getValue("ALBUMDIR_WIN").toString();
#else
		QString albumImagesName = QDir::homePath() + CZPlayerConfig::getValue("ALBUMDIR_X11").toString();
#endif

		QString albumImagePath = albumImagesName + "/" + strMusicName + ".jpg";
		QFile file(albumImagePath);
		if (file.exists())
		{
			sharedDialog->initSharedDialog(albumImagePath, strMusicName, strAlbumUrlPath);			//��ʼ������Ի���
		}
		else
		{
			sharedDialog->initSharedDialog(":/app/images/albumImage.png", strMusicName, strAlbumUrlPath);
		}

		sharedDialog->exec();
	}
}

//���ظ����б�
void Widget::loadMusicList( const QString &filePath )
{
	//���
	QTableWidgetItem *rowItem = new QTableWidgetItem;
	rowItem->setTextAlignment(Qt::AlignCenter);
	rowItem->setFont(QFont("΢���ź�", 10));

	//����
	QString fileName = QFileInfo(filePath).baseName();
	QTableWidgetItem *fileNameItem = new QTableWidgetItem(fileName);
	fileNameItem->setTextAlignment(Qt::AlignCenter);
	fileNameItem->setFont(QFont("΢���ź�", 10));

	//ʱ��
	WCHAR wFilePath[MAX_PATH];
	astr_ustr(const_cast<char*>(filePath.toStdString().c_str()), wFilePath);
	unsigned long nMusicTime = getMusicTime(wFilePath);
	QTime time(0, nMusicTime / 60, nMusicTime % 60);
	QTableWidgetItem *timeItem = new QTableWidgetItem(time.toString("mm:ss"));
	timeItem->setTextAlignment(Qt::AlignCenter);
	timeItem->setFont(QFont("΢���ź�", 10));

	//���벥���б�
	int currentRows = playList->rowCount();//�����б��е�����
	playList->insertRow(currentRows);//�Ӳ����б��еĵ�ǰ�в���
	rowItem->setText(QString::number(currentRows + 1));
	playList->setItem(currentRows, 0, rowItem);
	playList->setItem(currentRows, 1, fileNameItem);
	playList->setItem(currentRows, 2, timeItem);

	//����ʱ���м��
	if (currentRows + 1 <= 12)
	{
		playList->horizontalHeader()->resizeSection(3, 80);
	}
	else
	{
		playList->horizontalHeader()->resizeSection(3, 65);
	}

	//��������Ϣ����vector
	m_mapMusicRows.insert(make_pair(fileName, currentRows));
	m_mapMusicFilePath.insert(make_pair(fileName, filePath));

	//�������ݿ�
	DBModule::insertLine(fileName, time.toString("mm:ss"), filePath);
}

//�����겥���б�
void Widget::slot_LoadMusicListFinished()
{
	qDebug() << "�����겥���б�";
	if (isStarupLoadMusic = true)		//�Ƿ�������ʱ���ز����б�
	{
		//���������ļ���ȡ�Զ����ű�־Ϊtrue,�����б�Ϊ�ղ��Ҳ��Ǵ���������������ʱ��ִ�в��Ź���
		if (CZPlayerConfig::getValue("STARTPLAY").toString() == "true" && playList->rowCount() != 0 && m_argvMusicPath == "")
		{
			this->slot_SetPlayPaused();
		}

		//����Ǵ���������������,�򲥷Ÿø���(����Ϊ����)
		if (m_argvMusicPath != "")
		{
			//��map�����Ҹø���
			map<QString, int>::iterator iter = m_mapMusicRows.find(QFileInfo(m_argvMusicPath).baseName());
			if (iter != m_mapMusicRows.end())
			{
				//ģ��˫�����в�������,�ڶ�������������
				this->slot_TableDoubleClicked(iter->second, 1);
			}
			else	//��ǰ�����б�û�����׸���Ѹø������벥���б��Ҳ���
			{
				this->loadMusicList(m_argvMusicPath);
				this->slot_TableDoubleClicked(playList->rowCount() - 1, 1);
			}
		}
	}
	isStarupLoadMusic = false;
}

//���ز����б����
void Widget::slot_LoadMusicListError()
{
	QMessageBox::information(NULL, tr("��Ϣ"), tr("������Ч��m3u��pls�ļ���"), QMessageBox::Yes);
	return;
}

//��Ӹ����ļ���
void Widget::slot_AddMusicDir()
{
#ifdef _WIN32_WINNT
	QString WINPATH = CZPlayerConfig::getValue("MUSICDIR_WIN").toString();
	QString directory = QFileDialog::getExistingDirectory(this, tr("��ѡ���ļ���"), WINPATH);
#else
	QString X11PATH =  QDir::homePath() + CZPlayerConfig::getValue("MUSICDIR_X11").toString();
	QString directory = QFileDialog::getExistingDirectory(this, tr("��ѡ���ļ���"), QDir::currentPath());
#endif

	if (!directory.isEmpty())
	{
		//�������ظ����б��߳�
		LoadMusicThread *loadMusicThread = new LoadMusicThread;
		connect(loadMusicThread, SIGNAL(sig_LoadMusicListFinished()), this, SLOT(slot_LoadMusicListFinished()));
		connect(loadMusicThread, SIGNAL(sig_LoadMusicListError()), this, SLOT(slot_LoadMusicListError()));
		loadMusicThread->setParam(directory, &m_mapMusicRows, &m_mapMusicFilePath, playList);
		loadMusicThread->start();
	}
}

//�ı乤��Ŀ¼
void Widget::changeWorkDir()
{
	//����������������ʱ��Ҫ�ı乤��Ŀ¼
	//if (m_argvMusicPath != "")
	//{
		WCHAR filePath[MAX_PATH];	//Ҫ�������е��ļ�������·��
		//�õ���ǰִ���ļ���ȫ·��
		HMODULE hModule = GetModuleHandle(NULL);
		GetModuleFileName(hModule, filePath, sizeof(filePath));
		filePath[wcsrchr(filePath, '\\') - filePath] = '\0';//wcsrchr:��ÿ���ַ������ֵ�λ��
		SetCurrentDirectory(filePath);						//�ı乤��Ŀ¼(����Ŀ¼Ϊexe����Ŀ¼)
	//}
}

//Ƶ�׸ı�
void Widget::slot_spectrumChanged( vector<float> vecFrequency )
{
	emit sig_spectrumChanged(vecFrequency);
}

//�������
void Widget::slot_Finished()
{
	qDebug() << "������ɣ�";
	//this->slot_SetNext();	//������һ��
	nextButton->click();//ѡ�в�����һ�׸�����ť
}

//����
void Widget::reset()
{
	m_spectrograph->reset();	//����Ƶ�׻�������

	//���ò�������
	if(basicPlayer != NULL)
	{
		CCriticalSection cs;
		cs.Enter();
		basicPlayer->Stop();
		//����Sleep������ԭ����basicPlayer->Stop()���������̵߳���ֹ���첽�ģ�
		//����Ҫ���߳���ֹ���delete basicPlayer
		Sleep(500);	
		cs.Leave();

		if (basicPlayer != NULL) 
		{ 
			delete basicPlayer; 
			basicPlayer = NULL;
		}
	}
}

//����
void Widget::play( const QString &strCurrentFilePath, const QString &strCurrentMusicName, const int &nCurrentMusicRow, const QString &strCurrentMusicTime )
{
	albumImage->setPixmap(QPixmap(tr("")));
	albumImage->setMovie(movie);
	movie->start();

	this->play(strCurrentFilePath);						//����
	this->setState(Play);								//����״̬
	m_strCurrentMusicName = strCurrentMusicName;		//��ǰ���ŵĸ�����
	m_strCurrentFilePath = strCurrentFilePath;			//��ǰ���ŵĸ���·��
	m_nCurrentMusicRow = nCurrentMusicRow;				//��ǰ���ŵĸ����к�
	m_strCurrentMusicTime = strCurrentMusicTime;		//��ǰ���ŵĸ���ʱ��
	this->stateChanged();								//״̬�ı䣬���½���
	this->rowChanged();									//ѡ�в���ý����
	playList->setCurrentMusicRow(m_nCurrentMusicRow);	//���õ�ǰ���Ÿ����к�
}

//����
void Widget::play( const QString &strCurrentFilePath )
{
	WCHAR wCurrentFilePath[MAX_PATH];
	astr_ustr(const_cast<char*>(strCurrentFilePath.toStdString().c_str()), wCurrentFilePath);
	//��CBasicPlayer���캯��������Ҫ��ɲ����̡߳�Ƶ�׷������Լ��ļ���д���Ĺ���
	basicPlayer = new CBasicPlayer(wCurrentFilePath);
	c_connect(basicPlayer, sig_spectrumChanged, this, &Widget::slot_spectrumChanged);
	c_connect(basicPlayer, sig_Finished, this, &Widget::slot_Finished);
	//��GetValid����������Ҫ�Ƕ������ļ����д��Ѿ���ȡ�����ļ�����
	if(!basicPlayer->GetInput()->GetValid())
	{
		return;
	}
	//�����̴߳������Լ��Ѿ���ȡ���������ļ������ݾͿ��Կ�ʼ������
	basicPlayer->Start();
}

//����
void Widget::play()
{
	CCriticalSection cs;
	cs.Enter();
	basicPlayer->Play();								//����
	cs.Leave();
}

//��ͣ
void Widget::pause()
{
	CCriticalSection cs;
	cs.Enter();
	basicPlayer->Pause();								//��ͣ
	cs.Leave();
}