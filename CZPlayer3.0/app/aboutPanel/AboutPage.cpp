#include "AboutPage.h"

AboutPage::AboutPage(void)
{
	strLabel = new QLabel;
	strLabel->setObjectName("strLabel");
	strLabel->setText(tr("����Qt�Ķ�ý�岥������CZPlayer��\n��Ʒչ:http://www.qtcn.org/bbs/read-htm-tid-55824.html\nԴ����:https://github.com/chxuan/CZPlayer3.0.git"));

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(strLabel);
	mainLayout->addStretch();
	this->setLayout(mainLayout);
}


AboutPage::~AboutPage(void)
{
}
