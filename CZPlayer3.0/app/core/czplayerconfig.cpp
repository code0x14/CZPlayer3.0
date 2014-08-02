#include "czplayerconfig.h"
#include "static.h"

QSettings* CZPlayerConfig::m_CZPlayerSetting = 0;

CZPlayerConfig::CZPlayerConfig(QObject *parent) : m_parent(parent)
{
}

CZPlayerConfig::~CZPlayerConfig()
{
}

//��ȡ�����ļ�
QVariant CZPlayerConfig::getValue(const QString &key)
{
    QString configPath = CZPlayerConfigName;
    if (!m_CZPlayerSetting)
    {
        m_CZPlayerSetting = new QSettings(configPath, QSettings::IniFormat);
        m_CZPlayerSetting->setIniCodec("UTF-8");
    }
    return m_CZPlayerSetting->value(key);
}

//д�����ļ�
void CZPlayerConfig::setValue(const QString &key, const QVariant &value)
{
    QString configPath = CZPlayerConfigName;
    if (!m_CZPlayerSetting)
    {
        m_CZPlayerSetting = new QSettings(configPath, QSettings::IniFormat);
        m_CZPlayerSetting->setIniCodec("UTF-8");
    }
    m_CZPlayerSetting->setValue(key, value);
}















