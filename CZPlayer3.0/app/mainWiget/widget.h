#ifndef WIDGET_H
#define WIDGET_H

#include "stdafx.h"
#include "musiclist.h"
#include "musiclrc.h"
#include "musicdownload.h"
#include "SearchPanel.h"
#include "albumthread.h"
#include "lrcthread.h"
#include "HotplugWatcherThread.h"
#include "LoadMusicThread.h"
#include "minmusicwidget.h"
#include "musiclistwidget.h"
#include "configdialog.h"
#include "SharedDialog.h"
#include "AboutPanel.h"
#include "globalhotkey.h"
#include "head.h"
#include "spectrograph.h"
#include "Utils.h"
#include "BasicPlayer.h"
#include "signal_slot.h"

//������������
class Widget : public QWidget
{
	Q_OBJECT

public:
	Widget(char *argvMusicPath);
	~Widget();

public:
	int getMusicListShow() const;
	void setMusicListShow(int value);

protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void contextMenuEvent(QContextMenuEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);

signals:
	void sig_ShowMainWidget();																//��ʾ������
	void sig_spectrumChanged(vector<float> vecFrequency);									//Ƶ�׸ı�

private slots:
	void slot_UpdateTime();																	//����ʱ��͸��
	void slot_OpenMinWidget();																//�����������
	void slot_OpenFile();																	//�������ļ�
	void slot_AddMusicList();																//��Ӳ����б�
	void slot_AddMusicDir();																//��Ӹ����ļ���
	void slot_OpenMusicList();																//�����ֲ����б�
	void slot_OpenMusicLrc();																//�򿪸��
	void slot_OpenSearchPanel();															//�����ظ�������
	void slot_HideSearchPanel();															//������������
	void slot_HideMusicList();																//�������ֲ������б�
	void slot_HideMainWidget();																//����������
	void slot_ShowMusicPanel();																//��ʾ������
	void slot_ConfigCZPlayer();																//����
	void slot_AboutCZPlayer();																//����CZPlayer
	void slot_AboutQt();																	//����Qt
	void slot_SetPlayPaused();																//����/��ͣ
	void slot_SetPlay();																	//����
	void slot_SetPause();																	//��ͣ
	void slot_SetStop();																	//ֹͣ
	void slot_SetPre();																		//��һ��
	void slot_SetNext();																	//��һ��
	void slot_TableDoubleClicked(int row, int /*column*/);									//˫��ý���ѡ�в��Ÿ�����
	void slot_ClearSources();																//���Դ
	void slot_DelSelectSource(int row, QString musicName);									//ɾ��ѡ��Դ�����ݿ���Ӧ����
	void slot_TrayIconActivated(QSystemTrayIcon::ActivationReason activationReason);		//ϵͳ���̼����
	void slot_ShowMinSize();																//��С��
	void slot_MinToTray();																	//��С��������
	void slot_ShowLrcLocked();																//��ʾ����Ѿ�������Ϣ
	void slot_ShowLrcUnlocked();															//��ʾ����Ѿ�������Ϣ
	void slot_UnlockLrc();																	//�������
	void slot_CloseAllProgress();															//�˳�����
	void slot_TimeOut();																	//��ʱ��
	void slot_ResolveLrc();																	//�������
	void slot_ShortCutsetDisable();															//����ȫ���ȼ�����/����
	void slot_ConfigOptions();																//ѡ������
	void slot_ShowHideMainWidget();															//��ʾ/����������
	void slot_MinMainWidget();																//����ģʽ/��������
	void slot_SetVolume(int value);															//��������
	void slot_SetMute();																	//���þ���
	void slot_ShowModeMenu();																//��ʾģʽ�˵�
	void slot_CyclePlay();																	//ѭ������
	void slot_RandomPlay();																	//�������
	void slot_AlbumDownloadFinished(QString strMusicName, QString strAlbumUrl);				//ר����������ź�
	void slot_SharedCurrentMusic(QString strMusicName);										//����ø���
	void slot_LoadMusicListFinished();														//�����겥���б�
	void slot_LoadMusicListError();															//���ز����б����

private c_slots:
	void slot_spectrumChanged(vector<float> vecFrequency);									//Ƶ�׸ı�
	void slot_Finished();																	//�������

private:
	//����״̬
	enum State
	{
		NoState,			//��״̬
		Play,				//����
		Pause,				//��ͣ
		Stop				//ֹͣ
	};

	//����ģʽ
	enum Mode
	{
		cyclePlay,			//ѭ������
		randomPlay			//�������
	};

private:
	void createUI();				//��������
	void connectUI();				//�ź����

	void resolveLrc();																		//�������
	void updateMap(QString musicName);														//����map
	void stateChanged();																	//����״̬�ı�(���½���)
	void rowChanged();																		//ѡ�е�ǰ����ý��Դ����
	void loadMusicList(const QString &filePath);											//���ظ����б�
	void play(const QString &strCurrentFilePath, const QString &strCurrentMusicName, 
		const int &nCurrentMusicRow, const QString &strCurrentMusicTime);					//����

	void setState(State state);																//���ò���״̬
	void setMode(Mode mode);																//���ò���ģʽ
	void reset();																			//����
	void play();																			//����
	void play(const QString &strCurrentFilePath);											//����
	void pause();																			//��ͣ
	void changeWorkDir();																	//�ı乤��Ŀ¼

private:
	//��ʾ������0���رգ�1��������
	int musicListShowFlag;
	int musicDownloadShowFlag;

	QPoint dragPosition;
	QTimer *timer;
	QTimer *shortCutIimer;
	QTimer *upDateTimer;

	MinMusicWidget *minMusicWidget;							//���㲥�Ž���
	MusicList *playList;									//�����б�
	MusicListWidget *musicListWidget;
	MusicLrc *lrc;											//���
	//MusicDownload *musicDownload;							//��������
	SearchPanel *searchPanel;								//�����������
	ConfigDialog *configDialog;								//���ý���
	SharedDialog *sharedDialog;								//�������
	AboutPanel *aboutPanel;									//����CZPlayer
	GlobalHotKey *globalHotKey;								//ȫ���ȼ�
	HotplugWatcherThread *hotpugWatcherThread;				//�豸����

	QMap<qint64, QString> lrcMap;
	map<QString, int> m_mapMusicRows;						//key����������value���к�
	map<QString, QString> m_mapMusicFilePath;				//key����������value������·��

	State m_state;											//����״̬
	Mode m_mode;											//����ģʽ

	CBasicPlayer* basicPlayer;								//��������
	Spectrograph *m_spectrograph;							//Ƶ�׻�������

	QSlider *seekSlider;									//���ſ�����
	QSlider *volumeSlider;									//����������

	QLabel *albumImage;
	QLabel *timeLabel;
	QLabel *InfoLabel;
	QLabel *currentMusicLabel;

	QAction *closeAction;
	QAction *pauseAction;
	QAction *stopAction;
	QAction *lastAction;
	QAction *nextAction;
	QAction *minAction;
	QAction *quitAction;
	QAction *unlockLrcAction;
	QAction *openFileAction;
	QAction *openMusicListAction;
	QAction *openSearchPanelAnimation;
	QAction *openLrcAction;
	QAction *configAction;
	QAction *aboutAction;
	QAction *aboutQtAction;
	QAction *cycleAction;
	QAction *randomAction;

	QMenu *trayMenu;										//ϵͳ���̲˵�
	QSystemTrayIcon *trayIcon;								//ϵͳ����ͼ��
	QMovie *movie;

	QPushButton *playButton;
	QPushButton *endButton;
	QPushButton *preButton;
	QPushButton *nextButton;
	QPushButton *openFileButton;
	QPushButton *musicListButton;
	QPushButton *lrcButton;
	QPushButton *showButton;
	QPushButton *volumeButton;
	QPushButton *modeButton;

	QString m_musicName;
	QString m_musicArtist;
	QString m_lrcFileName;
	QString m_title;
	QString m_artist;
	int nPos;

	QString m_strCurrentFilePath;		//��ǰ���Ÿ�����·��
	QString m_strCurrentMusicName;		//��ǰ���Ÿ���������
	QString m_strCurrentMusicTime;		//��ǰ���Ÿ�����ʱ��(mm:ss)
	int m_nCurrentMusicRow;				//��ǰ���Ÿ������к�
	int nTime;							//��ǰ���Ÿ�����ʱ��(��)

	bool isStarupLoadMusic;				//�Ƿ�������ʱ���ز����б�
	QString m_argvMusicPath;			//�����������Ĳ���
};

#endif // WIDGET_H
