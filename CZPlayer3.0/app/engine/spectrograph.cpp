#include "spectrograph.h"

Spectrograph::Spectrograph(QWidget *parent) : QLabel(parent) 
	,m_lowFreq(0.0) , m_highFreq(0.0)
{
	//����Ƶ��label�Ĵ�С
	this->resize(190, 78);
	m_vecFrequency.clear();
}

Spectrograph::~Spectrograph()
{
}

//����Ƶ�ײ���
void Spectrograph::setParams(double lowFreq, double highFreq)
{
    Q_ASSERT(highFreq > lowFreq);
    m_lowFreq = lowFreq;		//Ƶ���½�
    m_highFreq = highFreq;		//Ƶ���Ͻ�
}

//����Ƶ��
void Spectrograph::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

	//QPixmap backgroundImage;
	//backgroundImage.load(":/app/images/screen.png");
	////��ͨ��pix�ķ������ͼƬ�Ĺ��˵�͸���Ĳ��ֵõ���ͼƬ����ΪloginPanel�Ĳ�����߿�
	//this->setMask(backgroundImage.mask());
	//painter.drawPixmap(0, 0, 190, 78, backgroundImage);

	const int numBars = m_vecFrequency.size();

	QColor barColor(5, 184, 204);		//Ƶ��bar��ɫ

    barColor = barColor.lighter();
    barColor.setAlphaF(0.75);		//����alphaͨ��

    //����Ƶ��
    if (numBars)
	{
        //�����ȵ����Ϳհ�
		const int widgetWidth = this->width();										//Ƶ��widget���
        const int barPlusGapWidth = widgetWidth / numBars;							//ÿһ��Ƶ�׼ӿհ׼�϶�Ŀ��
        const int barWidth = 0.8 * barPlusGapWidth;									//ÿһ��Ƶ��bar�Ŀ��
        const int gapWidth = barPlusGapWidth - barWidth;							//ÿһ���հ׼�϶���
        const int paddingWidth = widgetWidth - numBars * (barWidth + gapWidth);		//��Ե���
        const int leftPaddingWidth = (paddingWidth + gapWidth) / 2;					//���Ե���
        const int barHeight = this->height() - 2 * gapWidth;						//ÿһ��Ƶ��bar�ĸ߶�

		//����ÿһ��Ƶ��bar
        for (int i = 0; i < numBars; ++i)
		{
            const double value = m_vecFrequency[i];		//vlaue��ֵ��0��1֮��
            Q_ASSERT(value >= 0.0 && value <= 1.0);

            QRect bar = rect();
			//����Ƶ��bar��λ�úʹ�С
            bar.setLeft(rect().left() + leftPaddingWidth + (i * (gapWidth + barWidth)));
            bar.setWidth(barWidth);
            bar.setTop(rect().top() + gapWidth + (1.0 - value) * barHeight);
            bar.setBottom(rect().bottom() - gapWidth);

            QColor color = barColor;
			
			//������ɫ����
			//QLinearGradient linearGradient(bar.topLeft(), bar.bottomRight());
			//linearGradient.setColorAt(0.1, QColor(247, 104, 9));
			//linearGradient.setColorAt(1.0, QColor(238, 17, 128)); 
			//painter.fillRect(bar, QBrush(linearGradient));

            painter.fillRect(bar, color);
        }
    }
	event->accept();
}

//Ƶ�׸ı�
void Spectrograph::slot_spectrumChanged( vector<float> vecFrequency )
{
	m_vecFrequency = vecFrequency;
	update();						//ˢ��Ƶ��
}

//����Ƶ��
void Spectrograph::reset()
{
	m_vecFrequency.clear();
	update();
}

