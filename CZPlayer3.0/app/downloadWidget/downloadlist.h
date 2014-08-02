#ifndef DOWNLOADLIST_H
#define DOWNLOADLIST_H

#include "nofocusdelegate.h"
#include "head.h"

class DownloadList : public QTableWidget
{
    Q_OBJECT

public:
    DownloadList(QWidget *widget = 0);
    ~DownloadList();
    void setMusicListMap(map<int, int> listMap);

protected:
    void contextMenuEvent(QContextMenuEvent *event);

signals:
    void sig_DelCurrentRow(int row);
    void sig_DelCurrentMusicMapItem(QString musicTitle);//ɾ����ǰ����mapIter
    void sig_PauseDownload(int row);//��ͣ����

private slots:
    void slot_TableClicked(int index);//��ȡѡ����
    void slot_DelCurrentMusic();//ɾ����ǰ��
    void slot_PauseMusicDownload();//��ͣ����
    void slot_StartMusicDownload();//��ʼ����

private:
    //void setThreadMap()
    map<int, int> m_listMap;

    QWidget *parent;
    int row;
    int delRow;
};

#endif // DOWNLOADLIST_H
