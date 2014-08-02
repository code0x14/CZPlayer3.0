#include "musicdownload.h"

MusicDownload::MusicDownload(QWidget *widget) : parent(widget)
{
    //���ô��ڻ�������
    this->resize(400, 212);//���ô����С
    this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);//ȥ������߿�,������������ʾͼ��
    this->setAttribute(Qt::WA_TranslucentBackground);//���ñ���͸��
    this->setWindowIcon(QIcon(":/app/images/CZPlayer.png"));//����logo
    this->setWindowTitle(tr("��������"));

    //����
    musicEditLabel = new QLabel(this);
    musicEditLabel->setObjectName(tr("musicEditLabel"));
    musicEditLabel->setText(tr("����:"));

    //���������
    musicEdit = new QLineEdit(this);
    musicEdit->setObjectName(tr("musicEdit"));
    //musicEdit->setMaxLength(0);//����������󳤶�

    //������
    artistEditLabel = new QLabel(this);
    artistEditLabel->setObjectName(tr("artistEditLabel"));
    artistEditLabel->setText(tr("����:"));

    //�����������
    artistEdit = new QLineEdit(this);
    artistEdit->setObjectName(tr("artistEdit"));

    //����
    downloadButton = new QPushButton(this);
    downloadButton->setObjectName(tr("downloadButton"));
    downloadButton->setToolTip(tr("���ظ���"));

    //�����б�
    downloadList = new DownloadList(this);
    downloadList->setObjectName(tr("downloadList"));

    //��������¼�������
    artistEdit->installEventFilter(this);
    musicEdit->installEventFilter(this);

    QSpacerItem *topItem = new QSpacerItem(380, 40, QSizePolicy::Fixed, QSizePolicy::Fixed);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addItem(topItem);
    mainLayout->addWidget(downloadList);
    this->setLayout(mainLayout);

    musicEditLabel->setGeometry(15, 20, 40, 16);
    musicEdit->setGeometry(55, 12, 120, 31);
    artistEditLabel->setGeometry(195, 20, 40, 16);
    artistEdit->setGeometry(236, 12, 80, 31);
    downloadButton->setGeometry(335, 13, 55, 33);

    connect(downloadButton, SIGNAL(clicked()), this, SLOT(slot_DownloadMusic()));
}

MusicDownload::~MusicDownload()
{
}

//��дpaintEvent,��ӱ���ͼƬ
void MusicDownload::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap backgroundImage;
    backgroundImage.load(":/app/images/mainBg.png");

    //��ͨ��pix�ķ������ͼƬ�Ĺ��˵�͸���Ĳ��ֵõ���ͼƬ����ΪloginPanel�Ĳ�����߿�
    this->setMask(backgroundImage.mask());
    painter.drawPixmap(0, 0, 400, 212, backgroundImage);
    event->accept();
}

//���ذ�ť�ص�
void MusicDownload::slot_DownloadMusic()
{
    QString musicName = musicEdit->text();//������
    QString musicArtist = artistEdit->text();//����

    if (musicName == "" || musicArtist == "")
    {
		QMessageBox::information(NULL, tr("��Ϣ"), tr("�����������������Ϊ�գ�"), QMessageBox::Yes);
        return;
    }

    //QString musicTitle = musicArtist + "-" + musicName;
    //map<QString, int>::iterator musicIter = musicMap.find(musicTitle);
    //if (musicIter != musicMap.end())//�ҵ�����ͬ������
    //{
    //    return;
    //}

    //static int times = 0;
    //++times;
    //musicMap.insert(make_pair(musicTitle, times));

    DownloadThread *downloadThread = new DownloadThread(musicName, musicArtist, downloadList);
    downloadThread->start();//����һ���߳�
//    static int index = 0;
//    threadMap.insert(make_pair(index, downloadThread));
//    ++index;

    connect(downloadThread, SIGNAL(sig_DelCurrentMusicMapItem(QString)), this, SLOT(slot_DelCurrentMapItem(QString)));//ɾ����ǰ����mapIter
    connect(downloadList, SIGNAL(sig_DelCurrentMusicMapItem(QString)), this, SLOT(slot_DelCurrentMapItem(QString)));//ɾ����ǰ����mapIter
    //connect(downloadList, SIGNAL(delCurrentRow(int)), this, SLOT(delCurrentThread(int)));//ɾ����ǰ�߳�
}

//ɾ����ǰ����mapIter
void MusicDownload::slot_DelCurrentMapItem(QString musicTitle)
{
    map<QString, int>::iterator musicMapIter = musicMap.find(musicTitle);
    if (musicMapIter != musicMap.end())
    {
        musicMap.erase(musicMapIter);
    }
}

////ɾ����ǰ�߳�
//void MusicDownload::delCurrentThread(int row)
//{
//    qDebug() << "row = " << row;
//    map<int, DownloadThread*>::iterator threadMapIter = threadMap.find(row);
//    if (threadMapIter != threadMap.end())
//    {
//        //threadMap.erase(threadMapIter);
////        DownloadThread *thread = threadMapIter->second;
////        thread->deleteLater();
//        qDebug() << "true";
//    }
//}

//�¼�������
bool MusicDownload::eventFilter(QObject *target, QEvent *event)
{
    if (target == artistEdit || target == musicEdit)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Return)
            {
                downloadButton->click();//ѡ�����ذ�ť
                return true;
            }
        }
    }

    return QWidget::eventFilter(target, event);
}








