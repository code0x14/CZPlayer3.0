#include "ThanksPage.h"

ThanksPage::ThanksPage(void)
{
	thanksLabel = new QLabel;
	thanksLabel->setObjectName(tr("thanksLabel"));
	thanksLabel->setText(tr("��л��\n		���� <787280310@qq.com>"
								   "\n		���� <787280310@qq.com>"));

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(thanksLabel);
	mainLayout->addStretch();
	this->setLayout(mainLayout);
}


ThanksPage::~ThanksPage(void)
{
}
