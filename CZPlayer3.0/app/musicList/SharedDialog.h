#pragma once

#include <Windows.h>
#include <QtCore>
#include <QtGui>

//��������Ի���
class SharedDialog : public QDialog
{
	Q_OBJECT

public:
	SharedDialog(QWidget *parent = 0);
	~SharedDialog(void);

public:
	void initSharedDialog(QString albumImagePath, QString strMusicName, QString strAlbumUrlPath);		//��ʼ������Ի���

private slots:
	void slot_CheckBoxStateChanged(int state);									//��ѡ��״̬�ı�
	void slot_SharedMusic();													//�������

private:
	QWidget *m_parent;
	QString m_strAlbumUrlPath;
	QString m_strMusicName;

	QLabel *albumLabel;
	QTextEdit *textEdit;
	QLabel *sharedLabel;

	QCheckBox *xinlangweiboCheckBox;	//����΢��
	QLabel *xinlangweiboLabel;

	QCheckBox *tengxunweiboCheckBox;	//��Ѷ΢��
	QLabel *tengxunweiboLabel;

	QCheckBox *qqkongjianCheckBox;		//QQ�ռ�
	QLabel *qqkongjianLabel;

	QCheckBox *renrenCheckBox;			//������
	QLabel *renrenLabel;

	QCheckBox *kaixinCheckBox;			//������
	QLabel *kaixinLabel;

	QCheckBox *doubanCheckBox;			//������
	QLabel *doubanLabel;

	QCheckBox *souhuweiboCheckBox;		//�Ѻ�΢��
	QLabel *souhuweiboLabel;

	QCheckBox *qqhaoyouCheckBox;		//QQ����
	QLabel *qqhaoyouLabel;

	QPushButton *shareButton;			//����ť
	QPushButton *cancelButton;			//ȡ����ť

private:
	void openBrowser(QString &qStrUrl);						//�������
};

