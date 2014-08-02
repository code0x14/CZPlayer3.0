#include "AboutPanel.h"

QTabWidget* AboutPanel::tabWidget = 0;

AboutPanel::AboutPanel(void)
{
	//���ô��ڻ�������
	this->resize(530, 535);//���ô����С
	this->setMinimumSize(530, 535);
	this->setWindowTitle(tr("����CZPlayer"));

	logoLabel = new QLabel;
	logoLabel->setPixmap(QPixmap(tr(":/app/images/CZPlayerWhite.png")));

	//����ҳ��
	aboutPage = new AboutPage;

	//����ҳ��
	authorPage = new AuthorPage;

	//��лҳ��
	thanksPage = new ThanksPage;

	tabWidget = new QTabWidget;
	tabWidget->addTab(aboutPage, tr("����"));
	tabWidget->addTab(authorPage, tr("����"));
	tabWidget->addTab(thanksPage, tr("��л"));

	okButton = new QPushButton;
	okButton->setText(tr("ȷ��"));

	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->addStretch();
	bottomLayout->addWidget(okButton);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(logoLabel);
	mainLayout->addWidget(tabWidget);
	mainLayout->addLayout(bottomLayout);
	this->setLayout(mainLayout);

	connect(okButton, SIGNAL(clicked()), this, SLOT(slot_CloseAboutPanel()));
}


AboutPanel::~AboutPanel(void)
{
}

void AboutPanel::slot_CloseAboutPanel()
{
	this->close();
}
