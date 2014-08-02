#include "globalhotkey.h"

QxtGlobalShortcut* GlobalHotKey::configOptions = 0;
QxtGlobalShortcut* GlobalHotKey::showHideMainWidget = 0;
QxtGlobalShortcut* GlobalHotKey::minMainWidget = 0;
QxtGlobalShortcut* GlobalHotKey::openMusicfile = 0;
QxtGlobalShortcut* GlobalHotKey::showDownload = 0;
QxtGlobalShortcut* GlobalHotKey::showMusicList = 0;
QxtGlobalShortcut* GlobalHotKey::showLrc = 0;
QxtGlobalShortcut* GlobalHotKey::playPause = 0;
QxtGlobalShortcut* GlobalHotKey::last = 0;
QxtGlobalShortcut* GlobalHotKey::nextSc = 0;

GlobalHotKey::GlobalHotKey(QWidget *parent)
{
    //ѡ������
    configOptions = new QxtGlobalShortcut(this);

    //��ʾ/����������
    showHideMainWidget = new QxtGlobalShortcut(this);

    //����ģʽ/��������
    minMainWidget = new QxtGlobalShortcut(this);

    //�������ļ�
    openMusicfile = new QxtGlobalShortcut(this);

    //��ʾ/���������б�
    showDownload = new QxtGlobalShortcut(this);

    //��ʾ/���ظ����б�
    showMusicList = new QxtGlobalShortcut(this);

    //��ʾ/����������
    showLrc = new QxtGlobalShortcut(this);

    //����/��ͣ
    playPause = new QxtGlobalShortcut(this);

    //��һ��
    last = new QxtGlobalShortcut(this);

    //��һ��
    nextSc = new QxtGlobalShortcut(this);

    //�ź����
	connect(configOptions, SIGNAL(activated()), parent, SLOT(slot_ConfigOptions()));
	connect(showHideMainWidget, SIGNAL(activated()), parent, SLOT(slot_ShowHideMainWidget()));
	connect(minMainWidget, SIGNAL(activated()), parent, SLOT(slot_MinMainWidget()));
	connect(openMusicfile, SIGNAL(activated()), parent, SLOT(slot_OpenFile()));
	connect(showDownload, SIGNAL(activated()), parent, SLOT(slot_OpenSearchPanel()));
	connect(showMusicList, SIGNAL(activated()), parent, SLOT(slot_OpenMusicList()));
	connect(showLrc, SIGNAL(activated()), parent, SLOT(slot_OpenMusicLrc()));
	connect(playPause, SIGNAL(activated()), parent, SLOT(slot_SetPlayPaused()));
	connect(last, SIGNAL(activated()), parent, SLOT(slot_SetPre()));
	connect(nextSc, SIGNAL(activated()), parent, SLOT(slot_SetNext()));
}

GlobalHotKey::~GlobalHotKey()
{
}

void GlobalHotKey::setConfigOptions(const QString &value)
{
    configOptions->setShortcut(QKeySequence(value));
}

void GlobalHotKey::setShowHideMainWidget(const QString &value)
{
    showHideMainWidget->setShortcut(QKeySequence(value));
}

void GlobalHotKey::setMinMainWidget(const QString &value)
{
    minMainWidget->setShortcut(QKeySequence(value));
}

void GlobalHotKey::setOpenMusicfile(const QString &value)
{
    openMusicfile->setShortcut(QKeySequence(value));
}

void GlobalHotKey::setShowDownload(const QString &value)
{
    showDownload->setShortcut(QKeySequence(value));
}

void GlobalHotKey::setShowMusicList(const QString &value)
{
    showMusicList->setShortcut(QKeySequence(value));
}

void GlobalHotKey::setShowLrc(const QString &value)
{
    showLrc->setShortcut(QKeySequence(value));
}

void GlobalHotKey::setPlayPause(const QString &value)
{
    playPause->setShortcut(QKeySequence(value));
}

void GlobalHotKey::setLast(const QString &value)
{
    last->setShortcut(QKeySequence(value));
}

void GlobalHotKey::setNext(const QString &value)
{
    nextSc->setShortcut(QKeySequence(value));
}

void GlobalHotKey::configOptionsDisabled( const bool &value )
{
	configOptions->setDisabled(value);
}

void GlobalHotKey::showHideMainWidgetDisabled( const bool &value )
{
	showHideMainWidget->setDisabled(value);
}

void GlobalHotKey::minMainWidgetDisabled( const bool &value )
{
	minMainWidget->setDisabled(value);
}

void GlobalHotKey::openMusicfileDisabled( const bool &value )
{
	openMusicfile->setDisabled(value);
}

void GlobalHotKey::showDownloadDisabled( const bool &value )
{
	showDownload->setDisabled(value);
}

void GlobalHotKey::showMusicListDisabled( const bool &value )
{
	showMusicList->setDisabled(value);
}

void GlobalHotKey::showLrcDisabled( const bool &value )
{
	showLrc->setDisabled(value);
}

void GlobalHotKey::playPauseDisabled( const bool &value )
{
	playPause->setDisabled(value);
}

void GlobalHotKey::lastDisabled( const bool &value )
{
	last->setDisabled(value);
}

void GlobalHotKey::nextDisabled( const bool &value )
{
	nextSc->setDisabled(value);
}
























