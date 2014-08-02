#include "nofocusdelegate.h"
#include "musiclist.h"
#include "DBModule.h"
#include "MusicListSaveFormatEntity.h"

MusicList::MusicList(QWidget *widget) : QTableWidget(widget), parent(widget), m_nPerviousColorRow(0)
{
    //���ô��ڻ�������
    this->resize(380, 360);//���ô����С
    //this->setWindowFlags(Qt::FramelessWindowHint);//ȥ������߿�
    this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);//ȥ������߿�,������������ʾͼ��

    //this->setAttribute(Qt::WA_TranslucentBackground);//���ñ���͸��
    this->setWindowIcon(QIcon(":/app/images/CZPlayer.png"));//����logo
    this->setWindowTitle(tr("���ֲ����б�"));

    this->setRowCount(0);//��ʼ��������Ϊ0
    this->setColumnCount(3);//��ʼ��������Ϊ3

    QStringList headList;
    headList << tr("���") << tr("����") << tr("ʱ��");
    this->setHorizontalHeaderLabels(headList);//����ͷ��Ϣ

    this->horizontalHeader()->setVisible(false);
    this->verticalHeader()->setVisible(false);
    //this->horizontalHeader()->setResizeMode(QHeaderView::Stretch);//���������Զ�����������������
    this->horizontalHeader()->resizeSection(0, 30);
    this->horizontalHeader()->resizeSection(1, 270);
    this->horizontalHeader()->resizeSection(2, 65);
    this->horizontalHeader()->setHighlightSections(false);//�����ʱ���Ա�ͷ�й�������ȡ���㣩
    this->horizontalHeader()->setClickable(false);//����Ӧ��굥��
    this->setSelectionMode(QAbstractItemView::SingleSelection);//����ֻ��ѡ�е���
    this->setSelectionBehavior(QAbstractItemView::SelectRows);//����ѡ��ϰ��Ϊѡ����
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);//�������ݲ��ɱ༭
    this->setShowGrid(false);//���ò���ʾ����
    this->setItemDelegate(new NoFocusDelegate());//ȥ��ѡ�е�Ԫ�������
	this->setMouseTracking(true);//����������깦��

    //����header��ʽ
    this->horizontalHeader()->setStyleSheet("QHeaderView::section{background: gray; color:white; font-family:'΢���ź�'}");
    this->setStyleSheet("QTableCornerButton::section{background:gray};");//���ñ��ֱ�Ƿ�����ʽ
    this->verticalHeader()->setStyleSheet("QHeaderView::section{background: gray; color:white; font-family:'΢���ź�';}");

    connect(this, SIGNAL(cellClicked(int,int)), this, SLOT(slot_TableClicked(int)));
	connect(this, SIGNAL(cellEntered(int, int)), this, SLOT(slot_TableRowEnter(int, int)));
}

MusicList::~MusicList()
{
}

//������ֲ����б�
void MusicList::slot_ClearAllMusicList()
{
	if (this->rowCount())
	{
		if (QMessageBox::Yes == QMessageBox::information(NULL, tr("��Ϣ"),
			tr("�Ƿ�Ҫ��ղ����б�"), QMessageBox::Yes | QMessageBox::No))
		{
			while (this->rowCount())
			{
				removeRow(0);
			}
			emit sig_PlayListClean();//�����󣬷�����ճɹ��ź�
		}
	}
	else
	{
		QMessageBox::information(NULL, tr("��Ϣ"), tr("��ǰ�����б�Ϊ�գ�"), QMessageBox::Yes);
	}
}

//ɾ����ǰ����
void MusicList::slot_DelCurrentMusic()
{
    if (this->rowCount())
    {
        if (QMessageBox::Yes == QMessageBox::information(NULL, tr("��Ϣ"),
                tr("�Ƿ�ɾ���ø�����"), QMessageBox::Yes | QMessageBox::No))
        {
            if (row < this->rowCount())
            {
				QString strMusicName = this->item(row, 1)->text();//������
                this->removeRow(row);
				emit sig_RowSelected(row, strMusicName);//���ݿ���ɾ��
				//���²����б����
                this->updateMusicList();
            }
        }
    }
	else
	{
		QMessageBox::information(NULL, tr("��Ϣ"), tr("��ǰ�����б�Ϊ�գ�"), QMessageBox::Yes);
	}
}

//���²����б����
void MusicList::updateMusicList()
{
    for (int index = row; index < this->rowCount(); ++index)
    {
        QTableWidgetItem *rowItem = this->item(index, 0);
        rowItem->setText(QString::number(index + 1));
    }
}

//���ѡ�е���
void MusicList::slot_TableClicked(int index)
{
    row = index;
}

//��껬����ÿһ��
void MusicList::slot_TableRowEnter( int row, int /*col*/ )
{
	QTableWidgetItem *item = NULL;

	//��ԭ��һ�е���ɫ
	item = this->item(m_nPerviousColorRow, 0);
	
	if (item != NULL)
	{
		//this->setRowColor(m_nPerviousColorRow, QColor(0, 0, 0, 0));
		for (int i = 0; i < this->columnCount(); ++i)
		{
			QTableWidgetItem *item = this->item(m_nPerviousColorRow, i);
			item->setFont(QFont("΢���ź�", 10, QFont::Normal));
			item->setBackgroundColor(QColor(0, 0, 0, 0));
		}
	}

	//���õ�ǰ����ɫ
	this->setRowColor(row, QColor(5, 184, 204));

	m_nPerviousColorRow = row;
}

//��������ɫ
void MusicList::setRowColor( int row, QColor color )
{
	for (int i = 0; i < this->columnCount(); ++i)
	{
		QTableWidgetItem *item = this->item(row, i);
		item->setFont(QFont("΢���ź�", 10, QFont::Bold));
		item->setBackgroundColor(color);
	}
}

//��Ӧ����Ҽ�����¼�
void MusicList::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
	menu.addAction(QIcon(":/app/images/shareMusic.png"), tr("����"), this, SLOT(slot_ShareCurrentMusic()));
    menu.addAction(QIcon(":/app/images/locateButton.png"), tr("��λ����ǰ���Ÿ���"), this, SLOT(slot_LocateCurrentMusic()));
    menu.addSeparator();
	menu.addAction(QIcon(":/app/images/delCurrentMusic.png"), tr("ɾ����ǰ����"), this, SLOT(slot_DelCurrentMusic()));
	menu.addAction(QIcon(":/app/images/clearMusicList.png"), tr("����б�"), this, SLOT(slot_ClearAllMusicList()));
	menu.addAction(QIcon(":/app/images/saveList.png"), tr("�����б�"), this, SLOT(slot_SaveList()));
    menu.addSeparator();
	menu.addAction(QIcon(":/app/images/quitAction.png"), tr("�ر������б�"), parent, SLOT(slot_OpenMusicList()));
    menu.exec(event->globalPos());//�������ָ���ȫ��λ��
    event->accept();
}

//��λ����ǰ����
void MusicList::slot_LocateCurrentMusic()
{
	if (m_nCurrentMusicRow == 0)
	{
		this->selectRow(m_nCurrentMusicRow + 1);
	}
	else
	{
		this->selectRow(m_nCurrentMusicRow - 1);
	}
	this->selectRow(m_nCurrentMusicRow);
}

//���浱ǰ�б�
void MusicList::slot_SaveList()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("���沥���б�"), tr("�����б�"), tr("*.m3u *.pls"));
	if (!fileName.isEmpty())
	{
		ofstream file(fileName.toStdString());

		QString strSuffixName = fileName.right(3);//�õ���׺��
		if (strSuffixName == "m3u" || strSuffixName == "M3U")
		{
			file << "#EXTM3U" << endl;			//��ʾһ��M3U�ļ�
		}
		else if (strSuffixName == "pls" || strSuffixName == "PLS")
		{
			file << "[playlist]" << endl;		//��ʾһ��pls�ļ�
		}

		vector<MusicListSaveFormatEntity> vec;
		if (DBModule::readMusicListSaveFormatEntity(vec))
		{
			int i = 0;
			foreach (MusicListSaveFormatEntity entity, vec)
			{
				++i;
				QTime qTime;
				int nTime = qTime.secsTo(QTime::fromString(QString::fromStdString(entity.getTime()), "mm:ss"));

				if (strSuffixName == "m3u" || strSuffixName == "M3U")
				{
					QString sFileName = QString::fromStdString(entity.getFileName());
					QString strMusicName = "";
					if (sFileName.contains(" - "))
					{
						strMusicName = sFileName.split(" - ").at(1);
					}
					file << "#EXTINF:" << nTime << "," << strMusicName.toStdString() << "\n" << entity.getFilePath() << endl;
				}
				else if (strSuffixName == "pls" || strSuffixName == "PLS")
				{
					QString sFileName = QString::fromStdString(entity.getFileName());
					QString strMusicName = "";
					if (sFileName.contains(" - "))
					{
						strMusicName = sFileName.split(" - ").at(1);
					}
					file << "File" << i << "=" << entity.getFilePath() << "\n"
						 << "Title" << i << "=" << strMusicName.toStdString() << "\n"
						 << "Length" << i << "=" << nTime << endl;
				}
			}
			if (strSuffixName == "pls" || strSuffixName == "PLS")
			{
				file << "NumberOfEntries=" << i <<  endl;
			}
		}
	}
}

//���õ�ǰ�������ŵ��к�
void MusicList::setCurrentMusicRow( int row )
{
	m_nCurrentMusicRow = row;
}

//����ǰ����
void MusicList::slot_ShareCurrentMusic()
{
	if (this->rowCount())
	{
		if (row < this->rowCount())
		{
			emit sig_SharedCurrentMusic(this->item(row, 1)->text());
		}
	}
	else
	{
		QMessageBox::information(NULL, tr("��Ϣ"), tr("��ǰ�����б�Ϊ�գ�"), QMessageBox::Yes);
	}
}