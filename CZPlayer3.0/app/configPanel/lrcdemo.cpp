#include "lrcdemo.h"

QLinearGradient LrcDemo::linearGradient;//��ʵ����Խ���
QLinearGradient LrcDemo::maskLinearGradient;//��ʵ��������Խ���
QFont LrcDemo::font;
qreal LrcDemo::lrcMaskWidth = 0;
int LrcDemo::shadow;

LrcDemo::LrcDemo()
{
    this->setWindowFlags(Qt::FramelessWindowHint);//ȥ������߿�
    this->setAttribute(Qt::WA_TranslucentBackground);//���ñ���͸��
    this->resize(500, 60);
    this->setText(tr("CZPlayer,����������"));
    this->setAlignment(Qt::AlignCenter);

    //��ʵ����Խ������
    linearGradient.setStart(0, 10);//�����������
//    linearGradient.setFinalStop(0, 40);//�����յ�����
//    linearGradient.setColorAt(0.1, QColor(14, 179, 255));
//    linearGradient.setColorAt(0.5, QColor(114, 232, 255));
//    linearGradient.setColorAt(0.9, QColor(14, 179, 255));

    //���ֵ����Խ������
    maskLinearGradient.setStart(0, 10);
    maskLinearGradient.setFinalStop(0, 40);
//    maskLinearGradient.setColorAt(0.1, QColor(222, 54, 4));
//    maskLinearGradient.setColorAt(0.5, QColor(255, 72, 16));
//    maskLinearGradient.setColorAt(0.9, QColor(222, 54, 4));

    //lrcMaskWidth += 13 * 30 / 2;
    this->update();
}

LrcDemo::~LrcDemo()
{
}

void LrcDemo::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setFont(font);

    //�Ȼ��Ƶײ����֣���Ϊ��Ӱ��������ʹ��ʾЧ�������������Ҹ����ʸ�
    painter.setPen(QColor(0, 0, 0, shadow));
    painter.drawText(1, 1, 500, 60, Qt::AlignLeft, text());//�����

    //����������ƽ�������
    painter.setPen(QPen(linearGradient, 0));
    painter.drawText(0, 0, 500, 60, Qt::AlignLeft, text());

    //���ø������
    painter.setPen(QPen(maskLinearGradient, 0));
    painter.drawText(0, 0, lrcMaskWidth, 60, Qt::AlignLeft, text());
    event->accept();
}

//��������
void LrcDemo::setFontYS(const QString &value)
{
    font.setFamily(value);
}

//���������С
void LrcDemo::setFontSize(const QString &value)
{
    font.setPointSize(value.toInt());
}

//������������
void LrcDemo::setFontType(const QString &value)
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
void LrcDemo::setLinearGradient(const QString &value)
{
    QStringList shangList = value.split(" ");
    int shangR = shangList.at(0).toInt();
    int shangG = shangList.at(1).toInt();
    int shangB = shangList.at(2).toInt();
    linearGradient.setColorAt(0.1, QColor(shangR, shangG, shangB));
}

void LrcDemo::setLinearGradient2(const QString &value)
{
    QStringList zhongList = value.split(" ");
    int zhongR = zhongList.at(0).toInt();
    int zhongG = zhongList.at(1).toInt();
    int zhongB = zhongList.at(2).toInt();
    linearGradient.setColorAt(0.5, QColor(zhongR, zhongG, zhongB));
}

void LrcDemo::setLinearGradient3(const QString &value)
{
    QStringList xiaList = value.split(" ");
    int xiaR = xiaList.at(0).toInt();
    int xiaG = xiaList.at(1).toInt();
    int xiaB = xiaList.at(2).toInt();
    linearGradient.setColorAt(0.9, QColor(xiaR, xiaG, xiaB));
}

//���ø���Ѳ�����ɫ
void LrcDemo::setMaskLinearGradient(const QString &value)
{
    QStringList shangList = value.split(" ");
    int shangR = shangList.at(0).toInt();
    int shangG = shangList.at(1).toInt();
    int shangB = shangList.at(2).toInt();
    maskLinearGradient.setColorAt(0.1, QColor(shangR, shangG, shangB));
}

void LrcDemo::setMaskLinearGradient2(const QString &value)
{
    QStringList zhongList = value.split(" ");
    int zhongR = zhongList.at(0).toInt();
    int zhongG = zhongList.at(1).toInt();
    int zhongB = zhongList.at(2).toInt();
    maskLinearGradient.setColorAt(0.5, QColor(zhongR, zhongG, zhongB));
}

void LrcDemo::setMaskLinearGradient3(const QString &value)
{
    QStringList xiaList = value.split(" ");
    int xiaR = xiaList.at(0).toInt();
    int xiaG = xiaList.at(1).toInt();
    int xiaB = xiaList.at(2).toInt();
    maskLinearGradient.setColorAt(0.9, QColor(xiaR, xiaG, xiaB));
}

//����mask����
void LrcDemo::setLrcMaskWidth(const int &value)
{
    lrcMaskWidth = value;
}

//�ı��ʻҶ�
void LrcDemo::setLrcDemoShadow(const int &value)
{
    shadow = value;
}


























