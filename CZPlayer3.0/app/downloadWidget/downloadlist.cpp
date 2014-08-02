#include "downloadlist.h"

DownloadList::DownloadList(QWidget *widget) : parent(widget)
{
    delRow = 0;
    this->setRowCount(0);//��ʼ��������Ϊ0
    this->setColumnCount(5);//��ʼ��������Ϊ5

    QStringList headList;
    headList << tr("��������") << tr("״̬") << tr("��С") << tr("�ٶ�") << tr("����");
    this->setHorizontalHeaderLabels(headList);//����ͷ��Ϣ

    this->setSelectionMode(QAbstractItemView::SingleSelection);//����ֻ��ѡ�е���
    this->setSelectionBehavior(QAbstractItemView::SelectRows);//����ѡ��ϰ��Ϊѡ����
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);//�������ݲ��ɱ༭
    this->horizontalHeader()->setResizeMode(QHeaderView::Stretch);//���������Զ�����������������
    this->setFrameStyle(QFrame::NoFrame);//ȥ���߿�
    this->setShowGrid(false);//���ò���ʾ����
    this->setItemDelegate(new NoFocusDelegate());//ȥ��ѡ�е�Ԫ�������
    this->horizontalHeader()->setHighlightSections(false);//�����ʱ���Ա�ͷ�й�������ȡ���㣩
    this->verticalHeader()->setVisible(false); //���ô�ֱͷ���ɼ�
    this->horizontalHeader()->setClickable(false);//����Ӧ��굥��

    //����header��ʽ
    this->horizontalHeader()->setStyleSheet("QHeaderView{background-color: gray;/*background:transparent;*/color:white; font:14px; font-family:'΢���ź�';"
        "border-left:transparent;border:transparent;border-top:transparent;border-bottom:transparent;}");
    this->setStyleSheet("QTableCornerButton::section{background:gray};");//���ñ��ֱ�Ƿ�����ʽ
    this->verticalHeader()->setStyleSheet("QHeaderView::section{background-color: gray; color:white; font:14px; font-family:'΢���ź�';}");

    connect(this, SIGNAL(cellClicked(int,int)), this, SLOT(slot_TableClicked(int)));
}

//��������
DownloadList::~DownloadList()
{
}

//���ѡ�е���
void DownloadList::slot_TableClicked(int index)
{
    row = index;
}

//ɾ����ǰ����
void DownloadList::slot_DelCurrentMusic()
{
//     if (row < this->rowCount())
//     {
//         QString musicStatus =  this->item(row, 1)->text();
//         QString musicTitle = this->item(row, 0)->text();
//
//         for (map<int, int>::iterator iter = m_listMap.begin(); iter != m_listMap.end(); ++iter)
//         {
//             if (iter->second == row)
//             {
//                 delRow = iter->first;
//                 break;
//             }
//         }
//
//         if (musicStatus == "��������")
//         {
//             this->removeRow(row);
//             qDebug() << "delRow = " << delRow;
//             emit sig_DelCurrentRow(delRow);
//         }
//         else if (musicStatus == "���")
//         {
//             this->removeRow(row);
//         }
//         else if (musicStatus == "�ж�")
//         {
//             this->removeRow(row);
//             emit sig_DelCurrentRow(delRow);
//         }
//         emit sig_DelCurrentMusicMapItem(musicTitle);
//     }
}

//��ͣ����
void DownloadList::slot_PauseMusicDownload()
{
    if (row < this->rowCount())
    {
        QString musicStatus =  this->item(row, 1)->text();
        if (musicStatus == "��������")
        {
            emit sig_PauseDownload(row);
        }
    }
}

//��ʼ����
void DownloadList::slot_StartMusicDownload()
{

}

//�õ�listmap
void DownloadList::setMusicListMap(map<int, int> listMap)
{
    m_listMap = listMap;
}

//��Ӧ����Ҽ�����¼�
void DownloadList::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
//     menu.addAction(QIcon(":/app/images/slot_DelCurrentMusic.png"), tr("ɾ������"), this, SLOT(slot_DelCurrentMusic()));
//     menu.addSeparator();
    menu.addAction(QIcon(":/app/images/sig_PauseDownload.png"), tr("��ͣ����"), this, SLOT(slot_PauseMusicDownload()));
    menu.addAction(QIcon(":/app/images/startDownload.png"), tr("��ʼ����"), this, SLOT(slot_StartMusicDownload()));
    menu.exec(event->globalPos());//globalPos()Ϊ��ǰ����λ������
    event->accept();
}











