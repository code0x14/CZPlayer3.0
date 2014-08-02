#include "musiclrc.h"

QLinearGradient MusicLrc::linearGradient;//��ʵ����Խ���
QLinearGradient MusicLrc::maskLinearGradient;//��ʵ��������Խ���
QFont MusicLrc::font;
int MusicLrc::shadow;

MusicLrc::MusicLrc()
{
    flag = 0;//û���������

    //ȥ������߿��������ʾ����Ļ��ǰ��,��Ҫ��showNormalһ����
    this->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint
                          | Qt::FramelessWindowHint);//ȥ������߿�,������������ʾͼ��
    this->setAttribute(Qt::WA_TranslucentBackground);//���ñ���͸��
    this->setWindowIcon(QIcon(":/app/images/CZPlayer.png"));//����logo
    this->resize(1000, 60);

    //���ø��λ��
    double width = QApplication::desktop()->width();
    double height = QApplication::desktop()->height();
    this->move(QPoint(width / 2 - 800 / 2, height - 90));

    //��ʵ����Խ������
    linearGradient.setStart(0, 10);//�����������
    linearGradient.setFinalStop(0, 40);//�����յ�����
    //��һ�������յ����꣬��������������������ԣ����ձ������м���
//    linearGradient.setColorAt(0.1, QColor(14, 179, 255));
//    linearGradient.setColorAt(0.5, QColor(114, 232, 255));
//    linearGradient.setColorAt(0.9, QColor(14, 179, 255));

    //���ֵ����Խ������
    maskLinearGradient.setStart(0, 10);
    maskLinearGradient.setFinalStop(0, 40);
//    maskLinearGradient.setColorAt(0.1, QColor(222, 54, 4));
//    maskLinearGradient.setColorAt(0.5, QColor(255, 72, 16));
//    maskLinearGradient.setColorAt(0.9, QColor(222, 54, 4));

//    //��������
//    font.setFamily("΢���ź�");
//    //font.setBold(true);
//    font.setPointSize(30);

    //���ö�ʱ��
    timer = new QTimer(this);

    lrcMaskWidth = 0;
    lrcMaskWidthInterval = 0;

    connect(timer, SIGNAL(timeout()), this, SLOT(slot_TimeOut()));
}

MusicLrc::~MusicLrc()
{
	if (timer) {timer->stop(); delete timer; timer = 0;}
}

//����������֣���Ҫָ����ǰ��ʿ�ʼ�����֮���ʱ����
void MusicLrc::startLrcMask(qint64 intervalTime)
{
    //��������ÿ��30�������һ�����ֵĿ�ȣ���Ϊ�������̫ƽ��
    //������cpuռ���ʣ������ʱ����̫���򶯻�Ч���Ͳ�������
    qreal count = intervalTime / 30;

    //��ȡ����ÿ����Ҫ���ӵĿ�ȣ������800�ǲ����Ĺ̶����
    lrcMaskWidthInterval = 1000 / count;
    lrcMaskWidth = 0;
    timer->start(30);
}

//ֹͣ�������
void MusicLrc::stopLrcMask()
{
    timer->stop();
    lrcMaskWidth = 0;
    this->update();
}

void MusicLrc::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setFont(font);

    //�Ȼ��Ƶײ����֣���Ϊ��Ӱ��������ʹ��ʾЧ�������������Ҹ����ʸ�
    painter.setPen(QColor(0, 0, 0, shadow));
    painter.drawText(1, 1, 1000, 60, Qt::AlignLeft, text());//�����

    //����������ƽ�������
    painter.setPen(QPen(linearGradient, 0));
    painter.drawText(0, 0, 1000, 60, Qt::AlignLeft, text());

    //���ø������
    painter.setPen(QPen(maskLinearGradient, 0));
    painter.drawText(0, 0, lrcMaskWidth, 60, Qt::AlignLeft, text());
    event->accept();
}

//��дmousePressEvent��mouseMoveEventʵ�ָ�ʵ��ƶ�
void MusicLrc::mousePressEvent(QMouseEvent *event)
{
    if (flag == 0)//û�п����������
    {
        if (event->button() == Qt::LeftButton)
        {
            dragPosition = event->globalPos() - frameGeometry().topLeft();
            event->accept();
        }
    }
}

//��дmousePressEvent��mouseMoveEventʵ�ָ�ʵ��ƶ�
void MusicLrc::mouseMoveEvent(QMouseEvent *event)
{
    if (flag == 0)//û�п����������
    {
        if (event->buttons() == Qt::LeftButton)
        {
            setCursor(Qt::PointingHandCursor);//��ʾ����
            move(event->globalPos() - dragPosition);//�ƶ�����
            event->accept();
        }
    }
}

//�һ��¼�
void MusicLrc::contextMenuEvent(QContextMenuEvent *event)
{
    if (flag == 0)//û�п����������
    {
        QMenu menu;
        menu.addAction(QIcon(":/app/images/lrc.png"), tr("���ظ��"), this, SLOT(hide()));
        menu.addAction(QIcon(":images/lock.png"), tr("�������"), this, SLOT(slot_LockLrc()));
        menu.exec(event->globalPos());//globalPos()Ϊ��ǰ����λ������
    }
    event->accept();
}

//�������
void MusicLrc::slot_LockLrc()
{
    flag = 1;//�����������
    emit sig_LrcLocked();//�������Ѿ������ź�
}

//�������
void MusicLrc::unlockLrc()
{
    flag = 0;
    emit sig_LrcUnlocked();//�������Ѿ������ź�
}

//��������
void MusicLrc::setFontYS(const QString &value)
{
    font.setFamily(value);
}

//���������С
void MusicLrc::setFontSize(const QString &value)
{
    font.setPointSize(value.toInt());
}

//������������
void MusicLrc::setFontType(const QString &value)
{
    if (value == "����")
    {
        font.setBold(false);
        font.setItalic(false);
    }
    else if (value == "����")
    {
        font.setBold(true);
        font.setItalic(false);
    }
    else if (value == "��б")
    {
        font.setBold(false);
        font.setItalic(true);
    }
    else if (value == "���� ��б")
    {
        font.setBold(true);
        font.setItalic(true);
    }
}

//���ø��δ������ɫ
void MusicLrc::setLinearGradient(const QString &shangValue, const QString &zhongValue, const QString &xiaValue)
{
    QStringList shangList = shangValue.split(" ");
    int shangR = shangList.at(0).toInt();
    int shangG = shangList.at(1).toInt();
    int shangB = shangList.at(2).toInt();
    QStringList zhongList = zhongValue.split(" ");
    int zhongR = zhongList.at(0).toInt();
    int zhongG = zhongList.at(1).toInt();
    int zhongB = zhongList.at(2).toInt();
    QStringList xiaList = xiaValue.split(" ");
    int xiaR = xiaList.at(0).toInt();
    int xiaG = xiaList.at(1).toInt();
    int xiaB = xiaList.at(2).toInt();

    linearGradient.setColorAt(0.1, QColor(shangR, shangG, shangB));
    linearGradient.setColorAt(0.5, QColor(zhongR, zhongG, zhongB));
    linearGradient.setColorAt(0.9, QColor(xiaR, xiaG, xiaB));
}

//���ø���Ѳ�����ɫ
void MusicLrc::setMaskLinearGradient(const QString &shangValue, const QString &zhongValue, const QString &xiaValue)
{
    QStringList shangList = shangValue.split(" ");
    int shangR = shangList.at(0).toInt();
    int shangG = shangList.at(1).toInt();
    int shangB = shangList.at(2).toInt();
    QStringList zhongList = zhongValue.split(" ");
    int zhongR = zhongList.at(0).toInt();
    int zhongG = zhongList.at(1).toInt();
    int zhongB = zhongList.at(2).toInt();
    QStringList xiaList = xiaValue.split(" ");
    int xiaR = xiaList.at(0).toInt();
    int xiaG = xiaList.at(1).toInt();
    int xiaB = xiaList.at(2).toInt();

    maskLinearGradient.setColorAt(0.1, QColor(shangR, shangG, shangB));
    maskLinearGradient.setColorAt(0.5, QColor(zhongR, zhongG, zhongB));
    maskLinearGradient.setColorAt(0.9, QColor(xiaR, xiaG, xiaB));
}

//���ø����ӰЧ��
void MusicLrc::setLrcShadow(const int &value)
{
    shadow = value;
}

//��ʱ��
void MusicLrc::slot_TimeOut()
{
    //ÿ��һ�ι̶���ʱ�����ֵĳ��Ⱦ�����һ��
    lrcMaskWidth += lrcMaskWidthInterval;
    this->update();//���¸����ʾ�����ǲ��������ػ棬���ǰ���һ��Paint�¼�����������ѭ��ʱ��ϵͳ���ػ�
}


























