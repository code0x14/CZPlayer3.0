#ifndef MUSICLRC_H
#define MUSICLRC_H

#include "head.h"

class MusicLrc : public QLabel
{
    Q_OBJECT

public:
    MusicLrc();
    ~MusicLrc();

public:
    void startLrcMask(qint64 intervalTime);
    void stopLrcMask();
    void unlockLrc();

    static void setFontYS(QString const &value);
    static void setFontSize(QString const &value);
    static void setFontType(QString const &value);
    static void setLinearGradient(QString const &shangValue, QString const &zhongValue, QString const &xiaValue);
    static void setMaskLinearGradient(QString const &shangValue, QString const &zhongValue, QString const &xiaValue);
    static void setLrcShadow(int const &value);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

signals:
    void sig_LrcLocked();//����Ѿ������ź�
    void sig_LrcUnlocked();//����Ѿ������ź�

private slots:
    void slot_TimeOut();
    void slot_LockLrc();//�������

private:
    static QLinearGradient linearGradient;//��ʵ����Խ���
    static QLinearGradient maskLinearGradient;//��ʵ��������Խ���
    static QFont font;
    QTimer *timer;
    qreal lrcMaskWidth;//qreal = float �����ͣ�qrand = rand �����
    qreal lrcMaskWidthInterval;//�������ֿ�ȼ��
    QPoint dragPosition;
    qint16 flag;//���������־

    static int shadow;
    static QPainter painter;
};

#endif // MUSICLRC_H


































