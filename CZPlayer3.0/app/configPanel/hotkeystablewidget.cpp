#include "hotkeystablewidget.h"
#include "nofocusdelegate.h"

HotKeysTableWidget::HotKeysTableWidget(QWidget *parent) : m_parent(parent)
{
    //���ô��ڻ�������
    this->setRowCount(0);//��ʼ��������Ϊ0
    this->setColumnCount(2);//��ʼ��������Ϊ3

    QStringList headList;
    headList << tr("����") << tr("ȫ���ȼ�");
    this->setHorizontalHeaderLabels(headList);//����ͷ��Ϣ

//    this->horizontalHeader()->setVisible(false);
    this->verticalHeader()->setVisible(false);
    this->horizontalHeader()->setResizeMode(QHeaderView::Stretch);//���������Զ�����������������
//    this->horizontalHeader()->resizeSection(0, 30);
//    this->horizontalHeader()->resizeSection(1, 170);
//    this->horizontalHeader()->resizeSection(2, 100);
//    this->horizontalHeader()->resizeSection(3, 65);
    this->horizontalHeader()->setHighlightSections(false);//�����ʱ���Ա�ͷ�й�������ȡ���㣩
    this->horizontalHeader()->setClickable(false);//����Ӧ��굥��
    this->setSelectionMode(QAbstractItemView::SingleSelection);//����ֻ��ѡ�е���
    this->setSelectionBehavior(QAbstractItemView::SelectRows);//����ѡ��ϰ��Ϊѡ����
    //this->setEditTriggers(QAbstractItemView::NoEditTriggers);//�������ݲ��ɱ༭
    this->setShowGrid(false);//���ò���ʾ����
    this->setItemDelegate(new NoFocusDelegate());//ȥ��ѡ�е�Ԫ�������

    this->setStyleSheet("color: black;");
}

HotKeysTableWidget::~HotKeysTableWidget()
{
}
