#include "UpdatePage.h"

UpdatePage::UpdatePage(QWidget *parent) : m_parent(parent)
{
	QGroupBox *updateGB = new QGroupBox(tr("�������"), this);

	updateLabel = new QLabel(this);
	updateLabel->setText(tr("������ʹ�õİ汾��V3.0 ����ʱ��:2014��07��18��"));

	updateButton = new QPushButton(this);
	updateButton->setText(tr("�ֶ�������°汾"));

	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->addWidget(updateLabel);
	hLayout->addStretch();

	QHBoxLayout *hLayout2 = new QHBoxLayout;
	hLayout2->addWidget(updateButton);
	hLayout2->addStretch();

	QVBoxLayout *vLayout = new QVBoxLayout;
	//vLayout->addWidget(updateLabel);
	//vLayout->addWidget(updateButton);
	vLayout->addLayout(hLayout);
	vLayout->addLayout(hLayout2);
	updateGB->setLayout(vLayout);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(updateGB);
	mainLayout->addStretch();
	this->setLayout(mainLayout);
}


UpdatePage::~UpdatePage(void)
{
}
