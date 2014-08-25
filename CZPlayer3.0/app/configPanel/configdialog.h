#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QtCore>
#include <QtGui>
#include "generalsettingspage.h"
#include "downloadsettingspage.h"
#include "lrcsettingspage.h"
#include "hotkeyssettingspage.h"
#include "UpdatePage.h"

//���öԻ���
class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();

public:
    static QListWidget *contentsWidget;

//signals:
//    void changeLrcTransparent(int value);//�ı���͸����

private slots:
    void slot_ChangePage(QListWidgetItem *current, QListWidgetItem *previous);
    void slot_OkFunc();								//ȷ��
    void slot_AppFunc();							//Ӧ��

private:
    void createIcons();
	void generalSettingAppFunc();					//��������ҳӦ��
    void downloadAppFunc();							//����ҳӦ��
    void lrcAppFunc();								//���ҳӦ��
    void hotKeyAppFunc();							//�ȼ�ҳӦ��
    void mkdirMusicDir();
    void mkdirAlbumDir();
    void mkdirLrcDir();

private:
    QStackedWidget *pagesWidget;

    QPushButton *okButton;							//ȷ����ť
    QPushButton *cancelButton;						//ȡ����ť
    QPushButton *appButton;							//Ӧ�ð�ť

    QWidget *m_parent;

    GeneralSettingsPage *generalSettingsPage;		//��������
    DownloadSettingsPage *downloadSettingsPage;		//��������
    LrcSettingsPage *lrcSettingsPage;				//�������
    HotKeysSettingsPage *hotKeysSettingsPage;		//�ȼ�����
    //ProxySettingsPage *proxySettingsPage;			//��������
	UpdatePage *updatePage;							//����ҳ��
};

#endif // CONFIGDIALOG_H
