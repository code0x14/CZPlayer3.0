#ifndef MUSICLIST_H
#define MUSICLIST_H

#include "head.h"

//�����б�
class MusicList : public QTableWidget
{
    Q_OBJECT

public:
    MusicList(QWidget *widget = 0);
    ~MusicList();

public:
	void setCurrentMusicRow(int row);	//���õ�ǰ�������ŵ��к�

protected:
    void contextMenuEvent(QContextMenuEvent *event);

signals:
    void sig_PlayListClean();
    void sig_RowSelected(int row, QString title);
	void sig_SharedCurrentMusic(QString strMusicName);

private slots:
    void slot_ClearAllMusicList();				//��ո���
    void slot_DelCurrentMusic();				//ɾ����ǰ����
    void slot_TableClicked(int index);			//��ȡѡ����
	void slot_TableRowEnter(int row, int /*col*/);	//��껬����ÿһ��
    void slot_LocateCurrentMusic();				//��λ����ǰ����
	void slot_SaveList();						//���浱ǰ�б�
	void slot_ShareCurrentMusic();				//����ǰ����

private:
    void updateMusicList();						//���²����б����
	void setRowColor(int row, QColor color);	//��������ɫ

private:
    QPoint dragPosition;
    QWidget *parent;
    int row;
	int m_nCurrentMusicRow;
	int m_nPerviousColorRow;
};

#endif // MUSICLIST_H
