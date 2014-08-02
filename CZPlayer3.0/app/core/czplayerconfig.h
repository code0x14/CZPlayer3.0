#ifndef CZPLAYERCONFIG_H
#define CZPLAYERCONFIG_H

#include "head.h"
#include <QVariant>
#include <QSettings>

class CZPlayerConfig : public QObject
{
    Q_OBJECT

public:
    CZPlayerConfig(QObject *parent = 0);
    ~CZPlayerConfig();

    //��ȡ�����ļ�
    static QVariant getValue(const QString &key);
    //д�����ļ�
    static void setValue(const QString &key, const QVariant &value);

private:
    static QSettings *m_CZPlayerSetting;
    QObject *m_parent;
};

#endif // CZPLAYERCONFIG_H
