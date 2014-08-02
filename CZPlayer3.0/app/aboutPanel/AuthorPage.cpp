#include "AuthorPage.h"


AuthorPage::AuthorPage(void)
{
	mainAuthorLabel = new QLabel;
	mainAuthorLabel->setObjectName(tr("mainAuthorLabel"));
	mainAuthorLabel->setText(tr("���Ŀ�����\n		���� <787280310@qq.com>"));

	plugAuthorLabel = new QLabel;
	plugAuthorLabel->setObjectName(tr("plugAuthorLabel"));
	plugAuthorLabel->setText(tr("���������\n		���� <787280310@qq.com>"));

	artAuthorLabel = new QLabel;
	artAuthorLabel->setObjectName(tr("artAuthorLabel"));
	artAuthorLabel->setText(tr("������\n		���� <787280310@qq.com>"));

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(mainAuthorLabel);
	mainLayout->addWidget(plugAuthorLabel);
	mainLayout->addWidget(artAuthorLabel);
	mainLayout->addStretch();
	this->setLayout(mainLayout);
}


AuthorPage::~AuthorPage(void)
{
}
