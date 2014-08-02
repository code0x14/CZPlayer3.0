#ifndef SPECTROGRAPH_H
#define SPECTROGRAPH_H

#include <QtCore>
#include <QtGui>
#include <vector>
using namespace std;

QT_FORWARD_DECLARE_CLASS(QMouseEvent)

//Ƶ�׻���
class Spectrograph : public QLabel
{
    Q_OBJECT

public:
    Spectrograph(QWidget *parent = 0);
    ~Spectrograph();

public:
	//lowFreq:Ƶ���½�
	//highFreq:Ƶ���Ͻ�
    void setParams(double lowFreq, double highFreq);//���ò���

	void reset();													//����Ƶ��
	
protected:
    void paintEvent(QPaintEvent *event);

signals:
    void infoMessage(const QString &message, int intervalMs);	//��Ϣ�ź�

public slots:
	void slot_spectrumChanged(vector<float> vecFrequency);

private:
    double              m_lowFreq;								//Ƶ���½�
    double              m_highFreq;								//Ƶ���Ͻ�
	vector<float> m_vecFrequency;								//ͨ�����ٸ���Ҷ�任�������Ĳ���
};

#endif // SPECTROGRAPH_H
