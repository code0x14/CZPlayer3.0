#include "downloadsettingspage.h"
#include "static.h"

QLineEdit* DownloadSettingsPage::musicEdit = 0;
QLineEdit* DownloadSettingsPage::albumEdit = 0;
QLineEdit* DownloadSettingsPage::lrcFileEdit = 0;

DownloadSettingsPage::DownloadSettingsPage(QWidget *parent) : m_parent(parent)
{
    QGroupBox *dirSettingGB = new QGroupBox(tr("Ŀ¼����"));

    musicDir = new QLabel(this);
    musicDir->setObjectName(tr("downloadDir"));
    musicDir->setText(tr("����Ŀ¼��"));

    musicEdit = new QLineEdit(this);
    musicEdit->setObjectName(tr("downloadEdit"));

    scanButton = new QPushButton(this);
    scanButton->setObjectName(tr("scanButton"));
    scanButton->setText(tr("���"));

    albumDir = new QLabel(this);
    albumDir->setObjectName(tr("albumDir"));
    albumDir->setText(tr("ר��Ŀ¼��"));

    albumEdit = new QLineEdit(this);
    albumEdit->setObjectName(tr("albumEdit"));

    scanButton2 = new QPushButton(this);
    scanButton2->setObjectName(tr("scanButton2"));
    scanButton2->setText(tr("���"));

    lrcFileLabel = new QLabel(this);
    lrcFileLabel->setObjectName(tr("lrcFileLabel"));
    lrcFileLabel->setText(tr("���Ŀ¼��"));

    lrcFileEdit = new QLineEdit(this);
    lrcFileEdit->setObjectName(tr("lrcFileEdit"));

    scanButton3 = new QPushButton(this);
    scanButton3->setObjectName(tr("scanButton3"));
    scanButton3->setText(tr("���"));

    QHBoxLayout *musicDirLayout = new QHBoxLayout;
    musicDirLayout->addWidget(musicDir);
    musicDirLayout->addWidget(musicEdit);
    musicDirLayout->addWidget(scanButton);

    QHBoxLayout *albumDirLayout = new QHBoxLayout;
    albumDirLayout->addWidget(albumDir);
    albumDirLayout->addWidget(albumEdit);
    albumDirLayout->addWidget(scanButton2);

    QHBoxLayout *lrcDirLayout = new QHBoxLayout;
    lrcDirLayout->addWidget(lrcFileLabel);
    lrcDirLayout->addWidget(lrcFileEdit);
    lrcDirLayout->addWidget(scanButton3);

    QVBoxLayout *dirSettingLayout = new QVBoxLayout;
    dirSettingLayout->addLayout(musicDirLayout);
    dirSettingLayout->addLayout(albumDirLayout);
    dirSettingLayout->addLayout(lrcDirLayout);
    dirSettingGB->setLayout(dirSettingLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(dirSettingGB);
    mainLayout->addStretch();
    this->setLayout(mainLayout);

    connect(scanButton, SIGNAL(clicked()), this, SLOT(slot_ChangeDir()));
    connect(scanButton2, SIGNAL(clicked()), this, SLOT(slot_ChangeDir()));
    connect(scanButton3, SIGNAL(clicked()), this, SLOT(slot_ChangeDir()));
}

DownloadSettingsPage::~DownloadSettingsPage()
{
}

//�õ�����Ŀ¼
QString DownloadSettingsPage::getMusicDir()
{
    return musicEdit->text();
}

//��������Ŀ¼
void DownloadSettingsPage::setMusicDir(const QString &musicDir)
{
    musicEdit->setText(musicDir);
}

//�õ�ר��Ŀ¼
QString DownloadSettingsPage::getAlbumDir()
{
    return albumEdit->text();
}

//�õ����Ŀ¼
QString DownloadSettingsPage::getLrcDir()
{
    return lrcFileEdit->text();
}

//���ø��Ŀ¼
void DownloadSettingsPage::setLrcDir(const QString &lrcDir)
{
    lrcFileEdit->setText(lrcDir);
}

//����ר��Ŀ¼
void DownloadSettingsPage::setAlbumDir(const QString &albumDir)
{
    albumEdit->setText(albumDir);
}

//��������Ŀ¼
void DownloadSettingsPage::slot_ChangeDir()
{
    QPushButton *button = dynamic_cast<QPushButton *>(sender());
#ifdef _WIN32_WINNT
    QString directory = QFileDialog::getExistingDirectory(this, tr("��Ŀ¼"),
                                                    "E:\\",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
#else
    QString homePath = QDir::homePath();
    QString directory = QFileDialog::getExistingDirectory(this, tr("��Ŀ¼"),
                                                    homePath,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
#endif
    if (button == scanButton)
    {
        if (!directory.isEmpty())
        {
            musicEdit->setText(directory);
        }
    }
    else if (button == scanButton2)
    {
        if (!directory.isEmpty())
        {
            albumEdit->setText(directory);
        }
    }
    else if (button == scanButton3)
    {
        if (!directory.isEmpty())
        {
            lrcFileEdit->setText(directory);
        }
    }
}










