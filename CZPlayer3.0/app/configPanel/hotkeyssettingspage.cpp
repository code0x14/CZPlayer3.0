#include "hotkeyssettingspage.h"
#include "defaultScheme.h"

HotKeysTableWidget* HotKeysSettingsPage::hotKeysTableWidget = 0;

QTableWidgetItem* HotKeysSettingsPage::settingsHotKey = 0;
QTableWidgetItem* HotKeysSettingsPage::minToTrayHotKey = 0;
QTableWidgetItem* HotKeysSettingsPage::openMinWidgetHotKey = 0;
QTableWidgetItem* HotKeysSettingsPage::openMusicFileHotKey = 0;
QTableWidgetItem* HotKeysSettingsPage::openDownloadWidgetHotKey = 0;
QTableWidgetItem* HotKeysSettingsPage::openListWidgetHotKey = 0;
QTableWidgetItem* HotKeysSettingsPage::openLrcHotKey = 0;
QTableWidgetItem* HotKeysSettingsPage::pauseHotKey = 0;
QTableWidgetItem* HotKeysSettingsPage::lastHotKey = 0;
QTableWidgetItem* HotKeysSettingsPage::nextHotKey = 0;

HotKeysSettingsPage::HotKeysSettingsPage(QWidget *parent) : m_parent(parent)
{
    QGroupBox *hotKeysGB = new QGroupBox(tr("�ȼ�����"));

    hotKeysTableWidget = new HotKeysTableWidget(this);

    //ѡ������
    QTableWidgetItem *settingsAction = new QTableWidgetItem(tr("ѡ������"));
    settingsAction->setTextAlignment(Qt::AlignCenter);
    settingsHotKey = new QTableWidgetItem;
    settingsHotKey->setTextAlignment(Qt::AlignCenter);
    hotKeysTableWidget->insertRow(0);//�Ӳ����б��еĵ�ǰ�в���
    hotKeysTableWidget->setItem(0, 0, settingsAction);
    hotKeysTableWidget->setItem(0, 1, settingsHotKey);

	//QRegExp regExp("[^#]+$");	//�������룣��
	//hotKeysTableWidget->setValidator(new QRegExpValidator(regExp));

    //��ʾ/����������
    QTableWidgetItem *minToTrayAction = new QTableWidgetItem(tr("��ʾ/����������"));
    minToTrayAction->setTextAlignment(Qt::AlignCenter);
    minToTrayHotKey = new QTableWidgetItem;
    minToTrayHotKey->setTextAlignment(Qt::AlignCenter);
    hotKeysTableWidget->insertRow(1);//�Ӳ����б��еĵ�ǰ�в���
    hotKeysTableWidget->setItem(1, 0, minToTrayAction);
    hotKeysTableWidget->setItem(1, 1, minToTrayHotKey);

    //����ģʽ/��������
    QTableWidgetItem *openMinWidgetAction = new QTableWidgetItem(tr("����ģʽ/��������"));
    openMinWidgetAction->setTextAlignment(Qt::AlignCenter);
    openMinWidgetHotKey = new QTableWidgetItem;
    openMinWidgetHotKey->setTextAlignment(Qt::AlignCenter);
    hotKeysTableWidget->insertRow(2);//�Ӳ����б��еĵ�ǰ�в���
    hotKeysTableWidget->setItem(2, 0, openMinWidgetAction);
    hotKeysTableWidget->setItem(2, 1, openMinWidgetHotKey);

    //�������ļ�
    QTableWidgetItem *openMusicFileAction = new QTableWidgetItem(tr("�������ļ�"));
    openMusicFileAction->setTextAlignment(Qt::AlignCenter);
    openMusicFileHotKey = new QTableWidgetItem;
    openMusicFileHotKey->setTextAlignment(Qt::AlignCenter);
    hotKeysTableWidget->insertRow(3);//�Ӳ����б��еĵ�ǰ�в���
    hotKeysTableWidget->setItem(3, 0, openMusicFileAction);
    hotKeysTableWidget->setItem(3, 1, openMusicFileHotKey);

    //��ʾ/���������б�
    QTableWidgetItem *openDownloadWidgetAction = new QTableWidgetItem(tr("��ʾ/���������б�"));
    openDownloadWidgetAction->setTextAlignment(Qt::AlignCenter);
    openDownloadWidgetHotKey = new QTableWidgetItem;
    openDownloadWidgetHotKey->setTextAlignment(Qt::AlignCenter);
    hotKeysTableWidget->insertRow(4);//�Ӳ����б��еĵ�ǰ�в���
    hotKeysTableWidget->setItem(4, 0, openDownloadWidgetAction);
    hotKeysTableWidget->setItem(4, 1, openDownloadWidgetHotKey);

    //��ʾ/���ظ����б�
    QTableWidgetItem *openListWidgetAction = new QTableWidgetItem(tr("��ʾ/���ظ����б�"));
    openListWidgetAction->setTextAlignment(Qt::AlignCenter);
    openListWidgetHotKey = new QTableWidgetItem;
    openListWidgetHotKey->setTextAlignment(Qt::AlignCenter);
    hotKeysTableWidget->insertRow(5);//�Ӳ����б��еĵ�ǰ�в���
    hotKeysTableWidget->setItem(5, 0, openListWidgetAction);
    hotKeysTableWidget->setItem(5, 1, openListWidgetHotKey);

    //��ʾ/����������
    QTableWidgetItem *openLrcAction = new QTableWidgetItem(tr("��ʾ/����������"));
    openLrcAction->setTextAlignment(Qt::AlignCenter);
    openLrcHotKey = new QTableWidgetItem;
	openLrcHotKey->setTextAlignment(Qt::AlignCenter);
    hotKeysTableWidget->insertRow(6);//�Ӳ����б��еĵ�ǰ�в���
    hotKeysTableWidget->setItem(6, 0, openLrcAction);
    hotKeysTableWidget->setItem(6, 1, openLrcHotKey);

    //����/��ͣ
    QTableWidgetItem *pauseAction = new QTableWidgetItem(tr("����/��ͣ"));
    pauseAction->setTextAlignment(Qt::AlignCenter);
    pauseHotKey = new QTableWidgetItem;
    pauseHotKey->setTextAlignment(Qt::AlignCenter);
    hotKeysTableWidget->insertRow(7);//�Ӳ����б��еĵ�ǰ�в���
    hotKeysTableWidget->setItem(7, 0, pauseAction);
    hotKeysTableWidget->setItem(7, 1, pauseHotKey);

    //��һ��
    QTableWidgetItem *lastAction = new QTableWidgetItem(tr("��һ��"));
    lastAction->setTextAlignment(Qt::AlignCenter);
    lastHotKey = new QTableWidgetItem;
    lastHotKey->setTextAlignment(Qt::AlignCenter);
    hotKeysTableWidget->insertRow(8);//�Ӳ����б��еĵ�ǰ�в���
    hotKeysTableWidget->setItem(8, 0, lastAction);
    hotKeysTableWidget->setItem(8, 1, lastHotKey);

    //��һ��
    QTableWidgetItem *nextAction = new QTableWidgetItem(tr("��һ��"));
    nextAction->setTextAlignment(Qt::AlignCenter);
    nextHotKey = new QTableWidgetItem;
    nextHotKey->setTextAlignment(Qt::AlignCenter);
    hotKeysTableWidget->insertRow(9);//�Ӳ����б��еĵ�ǰ�в���
    hotKeysTableWidget->setItem(9, 0, nextAction);
    hotKeysTableWidget->setItem(9, 1, nextHotKey);

    editButton = new QPushButton(this);
    editButton->setObjectName(tr("editButton"));
    editButton->setText(tr("�༭"));

    backSettingButton = new QPushButton(this);
    backSettingButton->setObjectName(tr("backSettingButton"));
    backSettingButton->setText(tr("�ָ�Ĭ��"));

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(editButton);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(backSettingButton);

    QVBoxLayout *hotKeysLayout = new QVBoxLayout;
    hotKeysLayout->addWidget(hotKeysTableWidget);
    hotKeysLayout->addLayout(buttonsLayout);
    hotKeysGB->setLayout(hotKeysLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(hotKeysGB);
    this->setLayout(mainLayout);

    //���ö����в��ɱ༭��ȫ���ȼ��пɱ༭
	for (int i = 0; i < hotKeysTableWidget->rowCount(); ++i)
	{
		QTableWidgetItem *item = hotKeysTableWidget->item(i, 0);
		item->setFlags(Qt::NoItemFlags);
	}

    connect(backSettingButton, SIGNAL(clicked()), this, SLOT(slot_BackSetting()));
	connect(editButton, SIGNAL(clicked()), this, SLOT(slot_EditHotKeys()));
}

//�ָ�Ĭ�Ϸ���
void HotKeysSettingsPage::slot_BackSetting()
{
    this->setSettingsHotKey(configOptions);
    this->setMinToTrayHotKey(showHideMainWidget);
    this->setOpenMinWidgetHotKey(minMainWidget);
    this->setOpenMusicFileHotKey(openMusicfile);
    this->setOpenDownloadWidgetHotKey(showDownload);
    this->setOpenListWidgetHotKey(showMusicList);
    this->setOpenLrcHotKey(showLrc);
    this->setPauseHotKey(playPause);
    this->setLastHotKey(last);
    this->setNextHotKey(nextSc);
}

//�༭�ȼ�
void HotKeysSettingsPage::slot_EditHotKeys()
{
	qDebug() << "hello";
}

HotKeysSettingsPage::~HotKeysSettingsPage()
{
}

QString HotKeysSettingsPage::getSettingsHotKey()
{
    return settingsHotKey->text();
}

void HotKeysSettingsPage::setSettingsHotKey(const QString &value)
{
    settingsHotKey->setText(value);
}

QString HotKeysSettingsPage::getMinToTrayHotKey()
{
    return minToTrayHotKey->text();
}

void HotKeysSettingsPage::setMinToTrayHotKey(const QString &value)
{
    minToTrayHotKey->setText(value);
}

QString HotKeysSettingsPage::getOpenMinWidgetHotKey()
{
    return openMinWidgetHotKey->text();
}

void HotKeysSettingsPage::setOpenMinWidgetHotKey(const QString &value)
{
    openMinWidgetHotKey->setText(value);
}

QString HotKeysSettingsPage::getOpenMusicFileHotKey()
{
    return openMusicFileHotKey->text();
}

void HotKeysSettingsPage::setOpenMusicFileHotKey(const QString &value)
{
    openMusicFileHotKey->setText(value);
}

QString HotKeysSettingsPage::getOpenDownloadWidgetHotKey()
{
    return openDownloadWidgetHotKey->text();
}

void HotKeysSettingsPage::setOpenDownloadWidgetHotKey(const QString &value)
{
    openDownloadWidgetHotKey->setText(value);
}

QString HotKeysSettingsPage::getOpenListWidgetHotKey()
{
    return openListWidgetHotKey->text();
}

void HotKeysSettingsPage::setOpenListWidgetHotKey(const QString &value)
{
    openListWidgetHotKey->setText(value);
}

QString HotKeysSettingsPage::getOpenLrcHotKey()
{
    return openLrcHotKey->text();
}

void HotKeysSettingsPage::setOpenLrcHotKey(const QString &value)
{
    openLrcHotKey->setText(value);
}

QString HotKeysSettingsPage::getPauseHotKey()
{
    return pauseHotKey->text();
}

void HotKeysSettingsPage::setPauseHotKey(const QString &value)
{
    pauseHotKey->setText(value);
}

QString HotKeysSettingsPage::getLastHotKey()
{
    return lastHotKey->text();
}

void HotKeysSettingsPage::setLastHotKey(const QString &value)
{
    lastHotKey->setText(value);
}

QString HotKeysSettingsPage::getNextHotKey()
{
    return nextHotKey->text();
}

void HotKeysSettingsPage::setNextHotKey(const QString &value)
{
    nextHotKey->setText(value);
}





























