#include "SearchList.h"
#include "nofocusdelegate.h"

SearchList::SearchList( QWidget *widget /*= 0*/ ) : QTableWidget(widget), m_parent(widget)
{
	this->setRowCount(0);//��ʼ��������Ϊ0
	this->setColumnCount(3);//��ʼ��������Ϊ3

	QStringList headList;
	headList << tr("����") << tr("����") << tr("ר��");
	this->setHorizontalHeaderLabels(headList);//����ͷ��Ϣ

	this->horizontalHeader()->setVisible(false);//����ˮƽͷ���ɼ�
	this->verticalHeader()->setVisible(false); //���ô�ֱͷ���ɼ�
	this->setSelectionMode(QAbstractItemView::SingleSelection);//����ֻ��ѡ�е���
	this->setSelectionBehavior(QAbstractItemView::SelectRows);//����ѡ��ϰ��Ϊѡ����
	this->setEditTriggers(QAbstractItemView::NoEditTriggers);//�������ݲ��ɱ༭
	this->horizontalHeader()->setResizeMode(QHeaderView::Stretch);//���������Զ�����������������
	this->setFrameStyle(QFrame::NoFrame);//ȥ���߿�
	this->setShowGrid(false);//���ò���ʾ����
	this->setItemDelegate(new NoFocusDelegate());//ȥ��ѡ�е�Ԫ�������
	this->horizontalHeader()->setHighlightSections(false);//�����ʱ���Ա�ͷ�й�������ȡ���㣩
	this->horizontalHeader()->setClickable(false);//����Ӧ��굥��

	//����header��ʽ
	this->horizontalHeader()->setStyleSheet("QHeaderView{background-color: gray;/*background:transparent;*/color:white; font:14px; font-family:'΢���ź�';"
		"border-left:transparent;border:transparent;border-top:transparent;border-bottom:transparent;}");
	this->setStyleSheet("QTableCornerButton::section{background:gray};");//���ñ��ֱ�Ƿ�����ʽ
	this->verticalHeader()->setStyleSheet("QHeaderView::section{background-color: gray; color:white; font:14px; font-family:'΢���ź�';}");
}

SearchList::~SearchList(void)
{
}
