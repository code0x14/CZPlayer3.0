#pragma once

#include "head.h"
#include "SearchList.h"
#include "SearchThread.h"

//�������
class SearchPanel : public QWidget
{
	Q_OBJECT

public:
	SearchPanel(QWidget *widget = 0);
	~SearchPanel(void);

protected:
	void paintEvent(QPaintEvent *event);
	bool eventFilter(QObject *target, QEvent *event);

private slots:
	void slot_SearchMusic();					//��������
	void slot_LineEditFocus();					//�����۽�
	void slot_ScrollBarEvent(int value);		//�������¼�
	void slot_SearchNum(int nNum);				//�ѵ�����ظ�������
	void slot_LoadFinished();					//�������
	void slot_CheckBoxStateChanged(int state);	//��ѡ��״̬�ı�
	void slot_SearchTimeout();					//������ʱ

private:
	QWidget *m_parent;
	SearchThread *searchThread;
	QLineEdit *searchLineEdit;
	QPushButton *searchButton;
	QLabel *searchInfoLabel;
	QLabel *searchInfoLabel2;
	SearchList *searchList;
	QTimer *timer;
	QCheckBox *allChecked;
	QPushButton *collectButton;
	QPushButton *downButton;
	QScrollBar *scrollBar;
	int m_nMusicNum;
	int m_nStartPage;				//�Ѿ�����ҳ������
	QString m_strSearchKey;
	bool m_bLoadStared;				//�Ƿ��Ѿ���ʼ����

	QLabel *navigationBarLabel;
	QPushButton *searchMusicButton;			//����
	QPushButton *broadcastingStationButton;	//��̨
	QPushButton *listButton;				//��
	QPushButton *downLoadListButton;		//�����б�
	QPushButton *favoritesListButton;		//�ղ��б�
	QPushButton *lyricsPhotoBarButton;		//���д��
};


