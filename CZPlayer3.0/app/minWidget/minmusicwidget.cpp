#include "minmusicwidget.h"

MinMusicWidget::MinMusicWidget(QWidget *parent) : m_parent(parent)
{
    //���ô��ڻ�������
    this->resize(200, 159);//���ô����С
    this->setWindowFlags(Qt::FramelessWindowHint);//ȥ������߿�
    this->setAttribute(Qt::WA_TranslucentBackground);//���ñ���͸��
    this->setWindowIcon(QIcon(":/app/images/CZPlayer.png"));//����logo
    this->setAcceptDrops(true);//�����Ϸ��ļ�
    this->setWindowTitle(tr("CZPlayer"));

    //����
    minPlayButton = new QPushButton(this);
    minPlayButton->setObjectName(tr("minPlayButton"));
    minPlayButton->setToolTip(tr("����"));

    //��ͣ
    minPauseButton = new QPushButton(this);
    minPauseButton->setObjectName(tr("minPauseButton"));
    minPauseButton->setToolTip(tr("��ͣ"));

    //ֹͣ
    minStopButton = new QPushButton(this);
    minStopButton->setObjectName(tr("minStopButton"));
    minStopButton->setToolTip(tr("ֹͣ"));

    //��һ��
    minNextButton = new QPushButton(this);
    minNextButton->setObjectName(tr("minNextButton"));
    minNextButton->setToolTip(tr("��һ��"));

    //��һ��
    minLastButton = new QPushButton(this);
    minLastButton->setObjectName(tr("minLastButton"));
    minLastButton->setToolTip(tr("��һ��"));

    //���ļ�
    minOpenFileButton = new QPushButton(this);
    minOpenFileButton->setObjectName(tr("minOpenFileButton"));
    minOpenFileButton->setToolTip(tr("���ļ�"));

    //������
    minMusicLrcAction = new QAction(this);
    minMusicLrcAction->setIcon(QIcon(":/app/images/lrc.png"));
    minMusicLrcAction->setText(tr("��ʾ������"));

    //���ø��������λ�úʹ�С
    minPlayButton->setGeometry(2, 33, 37, 37);
    minPauseButton->setGeometry(10, 78, 40, 37);
    minStopButton->setGeometry(45, 109, 41, 37);
    minNextButton->setGeometry(91, 114, 40, 37);
    minLastButton->setGeometry(134, 94, 40, 39);
    minOpenFileButton->setGeometry(161, 55, 39, 39);

    connect(minPlayButton, SIGNAL(clicked()), parent, SLOT(slot_SetPlay()));				//����
    connect(minPauseButton, SIGNAL(clicked()), parent, SLOT(slot_SetPause()));				//��ͣ
    connect(minStopButton, SIGNAL(clicked()), parent, SLOT(slot_SetStop()));				//ֹͣ
    connect(minNextButton, SIGNAL(clicked()), parent, SLOT(slot_SetNext()));				//��һ��
    connect(minLastButton, SIGNAL(clicked()), parent, SLOT(slot_SetPre()));					//��һ��
    connect(minOpenFileButton, SIGNAL(clicked()), parent, SLOT(slot_OpenFile()));			//���ļ�
    connect(minMusicLrcAction, SIGNAL(triggered()), parent, SLOT(slot_OpenMusicLrc()));		//��ʾ���
}

MinMusicWidget::~MinMusicWidget()
{
}

//��������
void MinMusicWidget::slot_OpenMainWidget()
{
    if (m_parent->isHidden())
    {
        QPropertyAnimation *mainWidgetAnimation = new QPropertyAnimation(m_parent, "windowOpacity");
        mainWidgetAnimation->setDuration(500);
        mainWidgetAnimation->setStartValue(0);
        mainWidgetAnimation->setEndValue(1);
        mainWidgetAnimation->start();
        m_parent->show();
        emit sig_ShowMusicPanel();

        QPropertyAnimation *minWidgetAnimation = new QPropertyAnimation(this, "windowOpacity");
        minWidgetAnimation->setDuration(500);
        minWidgetAnimation->setStartValue(1);
        minWidgetAnimation->setEndValue(0);
        minWidgetAnimation->start();
        connect(minWidgetAnimation, SIGNAL(finished()), this, SLOT(slot_HideMinWidget()));
    }
}

//����С����
void MinMusicWidget::slot_HideMinWidget()
{
    this->hide();
}

//����������Ҽ�����¼�
void MinMusicWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(QIcon(":/app/images/minMainWidget.png"), tr("����������"), this, SLOT(slot_OpenMainWidget()));
    menu.addAction(minMusicLrcAction);
    menu.exec(event->globalPos());//globalPos()Ϊ��ǰ����λ������
    //menu.exec(QCursor::pos());
    event->accept();
}

//��дpaintEvent,��ӱ���ͼƬ
void MinMusicWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap backgroundImage;
    backgroundImage.load(":/app/images/minMainBg2.png");

    //��ͨ��pix�ķ������ͼƬ�Ĺ��˵�͸���Ĳ��ֵõ���ͼƬ����ΪloginPanel�Ĳ�����߿�
    this->setMask(backgroundImage.mask());
    painter.drawPixmap(0, 0, 200, 159, backgroundImage);
    event->accept();
}

//��дmousePressEvent��mouseMoveEventʵ�ִ�����ƶ�
void MinMusicWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

//��дmousePressEvent��mouseMoveEventʵ�ִ�����ƶ�
void MinMusicWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        this->move(event->globalPos() - dragPosition);//�ƶ�����
        event->accept();
    }
}
