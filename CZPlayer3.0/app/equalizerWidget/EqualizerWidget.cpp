#include "EqualizerWidget.h"


EqualizerWidget::EqualizerWidget(QWidget *parent) : m_parent(parent)
{
	//���ô��ڻ�������
	this->resize(400, 150);//���ô����С
	this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);//ȥ������߿�
	this->setAttribute(Qt::WA_TranslucentBackground);//���ñ���͸��
	this->setWindowIcon(QIcon(":/app/images/CZPlayer.png"));//����logo
	this->setWindowTitle(tr("������"));
}

EqualizerWidget::~EqualizerWidget(void)
{
}

//��дpaintEvent,��ӱ���ͼƬ
void EqualizerWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QPixmap backgroundImage;
	backgroundImage.load(":/app/images/equalizerBg.png");

	//��ͨ��pix�ķ������ͼƬ�Ĺ��˵�͸���Ĳ��ֵõ���ͼƬ����ΪloginPanel�Ĳ�����߿�
	this->setMask(backgroundImage.mask());
	painter.drawPixmap(0, 0, 400, 150, backgroundImage);
	event->accept();
}
