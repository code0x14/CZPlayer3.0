#include "configdialog.h"
#include "static.h"
#include "czplayerconfig.h"
#include "musiclrc.h"
#include "globalhotkey.h"
#include "AutoStart.h"
#include "FileRelation.h"
#include <tchar.h>

QListWidget* ConfigDialog::contentsWidget = 0;

ConfigDialog::ConfigDialog(QWidget *parent) : m_parent(parent)
{
    //���ô��ڻ�������
    this->resize(665, 482);//���ô����С
    this->setMinimumSize(665, 482);
	this->setMaximumSize(665, 482);
    this->setWindowTitle(tr("CZPlayer ����"));

    //����ѡ��
    contentsWidget = new QListWidget;
    contentsWidget->setViewMode(QListView::ListMode);//IconMode
    contentsWidget->setIconSize(QSize(96, 84));
    contentsWidget->setMovement(QListView::Static);
    contentsWidget->setMaximumWidth(128);
    contentsWidget->setSpacing(12);
    this->createIcons();

    generalSettingsPage = new GeneralSettingsPage(this);
    downloadSettingsPage = new DownloadSettingsPage(this);
    lrcSettingsPage = new LrcSettingsPage(this);
    hotKeysSettingsPage = new HotKeysSettingsPage(this);
    //proxySettingsPage = new ProxySettingsPage(this);
	updatePage = new UpdatePage(this);

    //��ҳ��ʾ
    pagesWidget = new QStackedWidget;
    pagesWidget->addWidget(generalSettingsPage);		//��������
    pagesWidget->addWidget(downloadSettingsPage);		//��������
    pagesWidget->addWidget(lrcSettingsPage);			//�������
    pagesWidget->addWidget(hotKeysSettingsPage);		//�ȼ�����
    //pagesWidget->addWidget(proxySettingsPage);			//��������
	pagesWidget->addWidget(updatePage);					//����ҳ��
    contentsWidget->setCurrentRow(0);

    //ȷ����ť
    okButton = new QPushButton(this);
    okButton->setObjectName(tr("okButton"));
    okButton->setText(tr("ȷ��"));

    //ȡ����ť
    cancelButton = new QPushButton(this);
    cancelButton->setObjectName(tr("cancelButton"));
    cancelButton->setText(tr("ȡ��"));

    //Ӧ�ð�ť
    appButton = new QPushButton(this);
    appButton->setObjectName(tr("appButton"));
    appButton->setText(tr("Ӧ��"));

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(contentsWidget);
    horizontalLayout->addWidget(pagesWidget);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);
    buttonsLayout->addWidget(appButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    //mainLayout->addStretch();
    mainLayout->addLayout(buttonsLayout);
    this->setLayout(mainLayout);

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(hide()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(slot_OkFunc()));
    connect(appButton, SIGNAL(clicked()), this, SLOT(slot_AppFunc()));
}

ConfigDialog::~ConfigDialog()
{
}

//����ͼ��
void ConfigDialog::createIcons()
{
    //��������
    QListWidgetItem *generalSettingsItem = new QListWidgetItem(contentsWidget);
    generalSettingsItem->setIcon(QIcon(":/app/images/generalSettingsButton.png"));
    generalSettingsItem->setText(tr("��������"));
    generalSettingsItem->setTextAlignment(Qt::AlignCenter);
    generalSettingsItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    //��������
    QListWidgetItem *downloadSettingsItem = new QListWidgetItem(contentsWidget);
    downloadSettingsItem->setIcon(QIcon(":/app/images/downloadSettingsButton.png"));
    downloadSettingsItem->setText(tr("��������"));
    downloadSettingsItem->setTextAlignment(Qt::AlignCenter);
    downloadSettingsItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    //�������
    QListWidgetItem *lrcSettingsItem = new QListWidgetItem(contentsWidget);
    lrcSettingsItem->setIcon(QIcon(":/app/images/lrcSettingsButton.png"));
    lrcSettingsItem->setText(tr("�������"));
    lrcSettingsItem->setTextAlignment(Qt::AlignCenter);
    lrcSettingsItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    //�ȼ�����
    QListWidgetItem *hotKeysSettingsItem = new QListWidgetItem(contentsWidget);
    hotKeysSettingsItem->setIcon(QIcon(":/app/images/hotKeysSettingsButton.png"));
    hotKeysSettingsItem->setText(tr("�ȼ�����"));
    hotKeysSettingsItem->setTextAlignment(Qt::AlignCenter);
    hotKeysSettingsItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    //��������
    //QListWidgetItem *proxySettingsItem = new QListWidgetItem(contentsWidget);
    //proxySettingsItem->setIcon(QIcon(":/app/images/proxySettingsButton.png"));
    //proxySettingsItem->setText(tr("��������"));
    //proxySettingsItem->setTextAlignment(Qt::AlignCenter);
    //proxySettingsItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	//����ҳ��
	QListWidgetItem *updateItem = new QListWidgetItem(contentsWidget);
	updateItem->setIcon(QIcon(":/app/images/updateButton.png"));
	updateItem->setText(tr("�������"));
	updateItem->setTextAlignment(Qt::AlignCenter);
	updateItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(contentsWidget,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(slot_ChangePage(QListWidgetItem*,QListWidgetItem*)));
}

//�ı�ҳ��
void ConfigDialog::slot_ChangePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
    {
        current = previous;
    }
    pagesWidget->setCurrentIndex(contentsWidget->row(current));
}

//ȷ��
void ConfigDialog::slot_OkFunc()
{
    this->slot_AppFunc();
    this->hide();
}

//Ӧ��
void ConfigDialog::slot_AppFunc()
{
	this->generalSettingAppFunc();
    this->downloadAppFunc();
    this->lrcAppFunc();
    this->hotKeyAppFunc();
}

//��������ҳӦ��
void ConfigDialog::generalSettingAppFunc()
{
	if (GeneralSettingsPage::getAutoPlay() == Qt::Checked)
	{
		CZPlayerConfig::setValue("STARTPLAY", "true");
	}
	else
	{
		CZPlayerConfig::setValue("STARTPLAY", "false");
	}

	if (GeneralSettingsPage::getAutoStart() == Qt::Checked)
	{
		AutoStart::autoStart(_T("CZPlayer"));
	}
	else
	{
		AutoStart::cancelAutoStart(_T("CZPlayer"));
	}

	if (GeneralSettingsPage::getGreetings() == Qt::Checked)
	{
		CZPlayerConfig::setValue("GREETINGS", "true");
	}
	else
	{
		CZPlayerConfig::setValue("GREETINGS", "false");
	}

	if (GeneralSettingsPage::getDefaultPlayer() == Qt::Checked)
	{
		WCHAR filePath[MAX_PATH];	//�ļ�������·��
		WCHAR icoPath[MAX_PATH];	//ͼ��·��

		//�õ���ǰִ���ļ���ȫ·��
		HMODULE hModule = GetModuleHandle(NULL);
		GetModuleFileName(hModule, filePath, sizeof(filePath));
		wsprintf(icoPath, _T("%s,0"), filePath);
		
		FileRelation::registerFileRelation(_T(".mp3"), filePath, _T("CZPlayer3.0.MP3"), icoPath, _T("MP3�ļ�"));
		FileRelation::registerFileRelation(_T(".wma"), filePath, _T("CZPlayer3.0.WMA"), icoPath, _T("WMA�ļ�"));
		FileRelation::registerFileRelation(_T(".wav"), filePath, _T("CZPlayer3.0.WAV"), icoPath, _T("WAV�ļ�"));
		FileRelation::registerFileRelation(_T(".ogg"), filePath, _T("CZPlayer3.0.OGG"), icoPath, _T("OGG�ļ�"));
	}
	else
	{
		FileRelation::cancelFileRelation(_T(".mp3"), _T("CZPlayer3.0.MP3"));
		FileRelation::cancelFileRelation(_T(".wma"), _T("CZPlayer3.0.WMA"));
		FileRelation::cancelFileRelation(_T(".wav"), _T("CZPlayer3.0.WAV"));
		FileRelation::cancelFileRelation(_T(".ogg"), _T("CZPlayer3.0.OGG"));
	}
}

//����ҳӦ��
void ConfigDialog::downloadAppFunc()
{
    //�õ�����Ŀ¼
#ifdef _WIN32_WINNT
    CZPlayerConfig::setValue("MUSICDIR_WIN", DownloadSettingsPage::getMusicDir());
    CZPlayerConfig::setValue("ALBUMDIR_WIN", DownloadSettingsPage::getAlbumDir());
    CZPlayerConfig::setValue("LRCDIR_WIN", DownloadSettingsPage::getLrcDir());
#else
    CZPlayerConfig::setValue("MUSICDIR_X11", DownloadSettingsPage::getMusicDir());
    CZPlayerConfig::setValue("ALBUMDIR_X11", DownloadSettingsPage::getAlbumDir());
    CZPlayerConfig::setValue("LRCDIR_X11", DownloadSettingsPage::getLrcDir());
#endif
    this->mkdirMusicDir();
    this->mkdirAlbumDir();
    this->mkdirLrcDir();
}

//���ҳӦ��
void ConfigDialog::lrcAppFunc()
{
    //д�������ļ�
    CZPlayerConfig::setValue("FONT", LrcSettingsPage::getFontCombo());
    CZPlayerConfig::setValue("FONTSIZE", LrcSettingsPage::getFontSizeCombo());
    CZPlayerConfig::setValue("FONTTYPE", LrcSettingsPage::getFontTypeCombo());
    CZPlayerConfig::setValue("NOPLAYSHANG", LrcSettingsPage::getNoPlayEdit());
    CZPlayerConfig::setValue("NOPLAYZHONG", LrcSettingsPage::getNoPlayEdit2());
    CZPlayerConfig::setValue("NOPLAYXIA", LrcSettingsPage::getNoPlayEdit3());
    CZPlayerConfig::setValue("PLAYEDSHANG", LrcSettingsPage::getPlayedEdit());
    CZPlayerConfig::setValue("PLAYEDZHONG", LrcSettingsPage::getPlayedEdit2());
    CZPlayerConfig::setValue("PLAYEDXIA", LrcSettingsPage::getPlayedEdit3());
    CZPlayerConfig::setValue("YUSHECOLOR", LrcSettingsPage::getYuSheColor());
    CZPlayerConfig::setValue("SHADOW", LrcSettingsPage::getShadowSlider());

    //���ĸ������
    MusicLrc::setFontYS(CZPlayerConfig::getValue("FONT").toString());
    MusicLrc::setFontSize(CZPlayerConfig::getValue("FONTSIZE").toString());
    MusicLrc::setFontType(CZPlayerConfig::getValue("FONTTYPE").toString());
    MusicLrc::setLinearGradient(CZPlayerConfig::getValue("NOPLAYSHANG").toString(),
                                CZPlayerConfig::getValue("NOPLAYZHONG").toString(),
                                CZPlayerConfig::getValue("NOPLAYXIA").toString());
    MusicLrc::setMaskLinearGradient(CZPlayerConfig::getValue("PLAYEDSHANG").toString(),
                                    CZPlayerConfig::getValue("PLAYEDZHONG").toString(),
                                    CZPlayerConfig::getValue("PLAYEDXIA").toString());
    MusicLrc::setLrcShadow(CZPlayerConfig::getValue("SHADOW").toInt());
}

//�ȼ�ҳӦ��
void ConfigDialog::hotKeyAppFunc()
{
    //д�������ļ�
    CZPlayerConfig::setValue("CONFIGOPTIONS", HotKeysSettingsPage::getSettingsHotKey());
    CZPlayerConfig::setValue("SHOWHIDEMAINWIDGET", HotKeysSettingsPage::getMinToTrayHotKey());
    CZPlayerConfig::setValue("MINMAINWIDGET", HotKeysSettingsPage::getOpenMinWidgetHotKey());
    CZPlayerConfig::setValue("OPENMUSICFILE", HotKeysSettingsPage::getOpenMusicFileHotKey());
    CZPlayerConfig::setValue("SHOWHIDEDOWNLOAD", HotKeysSettingsPage::getOpenDownloadWidgetHotKey());
    CZPlayerConfig::setValue("SHOWHIDEMUSICLIST", HotKeysSettingsPage::getOpenListWidgetHotKey());
    CZPlayerConfig::setValue("SHOWHIDELRC", HotKeysSettingsPage::getOpenLrcHotKey());
    CZPlayerConfig::setValue("PLAYPAUSE", HotKeysSettingsPage::getPauseHotKey());
    CZPlayerConfig::setValue("LAST", HotKeysSettingsPage::getLastHotKey());
    CZPlayerConfig::setValue("NEXT", HotKeysSettingsPage::getNextHotKey());

    //�����ȼ�
    GlobalHotKey::setConfigOptions(CZPlayerConfig::getValue("CONFIGOPTIONS").toString());
    GlobalHotKey::setShowHideMainWidget(CZPlayerConfig::getValue("SHOWHIDEMAINWIDGET").toString());
    GlobalHotKey::setMinMainWidget(CZPlayerConfig::getValue("MINMAINWIDGET").toString());
    GlobalHotKey::setOpenMusicfile(CZPlayerConfig::getValue("OPENMUSICFILE").toString());
    GlobalHotKey::setShowDownload(CZPlayerConfig::getValue("SHOWHIDEDOWNLOAD").toString());
    GlobalHotKey::setShowMusicList(CZPlayerConfig::getValue("SHOWHIDEMUSICLIST").toString());
    GlobalHotKey::setShowLrc(CZPlayerConfig::getValue("SHOWHIDELRC").toString());
    GlobalHotKey::setPlayPause(CZPlayerConfig::getValue("PLAYPAUSE").toString());
    GlobalHotKey::setLast(CZPlayerConfig::getValue("LAST").toString());
    GlobalHotKey::setNext(CZPlayerConfig::getValue("NEXT").toString());
}

//����һ�����ظ���Ŀ¼
void ConfigDialog::mkdirMusicDir()
{
    QDir *CZPlayer = new QDir;
    //��ȡ�����ļ�//����һ���ļ������洢���صĸ���
#ifdef _WIN32_WINNT
    QString WINPATH = CZPlayerConfig::getValue("MUSICDIR_WIN").toString();
    bool exist = CZPlayer->exists(WINPATH);
    if (!exist)
    {
        CZPlayer->mkdir(WINPATH);
    }
#else
    QString X11PATH =  QDir::homePath() + CZPlayerConfig::getValue("MUSICDIR_X11").toString();
    bool exist = CZPlayer->exists(X11PATH);
    if (!exist)
    {
        CZPlayer->mkdir(X11PATH);
    }
#endif
}

//����һ������ר��Ŀ¼
void ConfigDialog::mkdirAlbumDir()
{
    QDir *CZPlayer = new QDir;
    //��ȡ�����ļ�//����һ���ļ������洢���ص�ר��
#ifdef _WIN32_WINNT
    QString WINPATH = CZPlayerConfig::getValue("ALBUMDIR_WIN").toString();
    bool exist = CZPlayer->exists(WINPATH);
    if (!exist)
    {
        CZPlayer->mkdir(WINPATH);
    }
#else
    QString X11PATH =  QDir::homePath() + CZPlayerConfig::getValue("ALBUMDIR_X11").toString();
    bool exist = CZPlayer->exists(X11PATH);
    if (!exist)
    {
        CZPlayer->mkdir(X11PATH);
    }
#endif
}

//����һ�����ظ��Ŀ¼
void ConfigDialog::mkdirLrcDir()
{
    QDir *CZPlayer = new QDir;
    //��ȡ�����ļ�//����һ���ļ������洢���صĸ��
#ifdef _WIN32_WINNT
    QString WINPATH = CZPlayerConfig::getValue("LRCDIR_WIN").toString();
    bool exist = CZPlayer->exists(WINPATH);
    if (!exist)
    {
        CZPlayer->mkdir(WINPATH);
    }
#else
    QString X11PATH =  QDir::homePath() + CZPlayerConfig::getValue("LRCDIR_X11").toString();
    bool exist = CZPlayer->exists(X11PATH);
    if (!exist)
    {
        CZPlayer->mkdir(X11PATH);
    }
#endif
}

















