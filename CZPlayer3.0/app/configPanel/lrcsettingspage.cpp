#include "lrcsettingspage.h"
#include "static.h"
#include "defaultScheme.h"
#include "czplayerconfig.h"

QLineEdit* LrcSettingsPage::noPlayEdit = 0;
QLineEdit* LrcSettingsPage::noPlayEdit2 = 0;
QLineEdit* LrcSettingsPage::noPlayEdit3 = 0;
QLineEdit* LrcSettingsPage::playedEdit = 0;
QLineEdit* LrcSettingsPage::playedEdit2 = 0;
QLineEdit* LrcSettingsPage::playedEdit3 = 0;
QComboBox* LrcSettingsPage::fontCombo = 0;
QComboBox* LrcSettingsPage::fontSizeCombo = 0;
QComboBox* LrcSettingsPage::fontTypeCombo = 0;
QAction* LrcSettingsPage::jingDianWhite = 0;
QAction* LrcSettingsPage::haiYangBlue = 0;
QAction* LrcSettingsPage::huoYanRed = 0;
QAction* LrcSettingsPage::kuXuanBlack = 0;
QAction* LrcSettingsPage::qiuSeYellow = 0;
QAction* LrcSettingsPage::langManPurple = 0;
QAction* LrcSettingsPage::ziRanGreen = 0;
QSlider* LrcSettingsPage::shadowSlider = 0;
QSpinBox* LrcSettingsPage::shadowSpinBox = 0;
//QPalette LrcSettingsPage::palette;

LrcSettingsPage::LrcSettingsPage(QWidget *parnet) : m_parent(parnet)
{
	//���ô��ڻ�������
	//this->resize(507, 423);//���ô����С
	//this->setMinimumSize(665, 482);
	//this->setMaximumSize(665, 482);

    //����
    QGroupBox *fontGB = new QGroupBox(tr("����"));

    fontLabel = new QLabel(this);
    fontLabel->setObjectName(tr("fontLabel"));
    fontLabel->setText(tr("����"));

    fontCombo = new QComboBox(this);
    QFontDatabase font;
    //QStringList fontList = font.families(QFontDatabase::SimplifiedChinese);//��ȡϵͳ����ʾ���ĵ�����
    QStringList fontList = font.families();//��ȡϵͳ����ʾ���ĵ�����
    fontCombo->addItems(fontList);

    fontSizeLabel = new QLabel(this);
    fontSizeLabel->setObjectName(tr("fontSizeLabel"));
    fontSizeLabel->setText(tr("�ֺ�"));

    fontSizeCombo = new QComboBox(this);
    QStringList fontSizeList;
    QString fontSizeStr;
    for (int i = 14; i <= 72; ++i)
    {
        fontSizeStr = QString::number(i, 10);
        fontSizeList << fontSizeStr;
    }
    fontSizeCombo->addItems(fontSizeList);

    fontTypeLabel = new QLabel(this);
    fontTypeLabel->setObjectName(tr("fontTypeLabel"));
    fontTypeLabel->setText(tr("����"));

    fontTypeCombo = new QComboBox(this);
    QStringList fontTypeList;
    fontTypeList << tr("����") << tr("����") << tr("��б") << tr("���� ��б");
    fontTypeCombo->addItems(fontTypeList);

    //��ɫ
    QGroupBox *colorGB = new QGroupBox(tr("��ɫ"));

    //δ����
    QGroupBox *noPlayGB = new QGroupBox(tr("δ����"));

    noPlayEdit = new QLineEdit(this);
    noPlayEdit->setObjectName(tr("noPlayEdit"));
    noPlayEdit->setReadOnly(true);

    noPlayEdit2 = new QLineEdit(this);
    noPlayEdit2->setObjectName(tr("noPlayEdit3"));
    noPlayEdit2->setReadOnly(true);

    noPlayEdit3 = new QLineEdit(this);
    noPlayEdit3->setObjectName(tr("noPlayEdit3"));
    noPlayEdit3->setReadOnly(true);

    changeNoPlayButton = new QPushButton(this);
    changeNoPlayButton->setObjectName(tr("changeNoPlayButton"));
    changeNoPlayButton->setText(tr("�ϲ���ɫ"));

    changeNoPlayButton2 = new QPushButton(this);
    changeNoPlayButton2->setObjectName(tr("changeNoPlayButton2"));
    changeNoPlayButton2->setText(tr("�в���ɫ"));

    changeNoPlayButton3 = new QPushButton(this);
    changeNoPlayButton3->setObjectName(tr("changeNoPlayButton3"));
    changeNoPlayButton3->setText(tr("�²���ɫ"));

    //�Ѳ���
    QGroupBox *playedGB = new QGroupBox(tr("�Ѳ���"));

    playedEdit = new QLineEdit(this);
    playedEdit->setObjectName(tr("playedEdit"));
    playedEdit->setReadOnly(true);

    playedEdit2 = new QLineEdit(this);
    playedEdit2->setObjectName(tr("playedEdit2"));
    playedEdit2->setReadOnly(true);

    playedEdit3 = new QLineEdit(this);
    playedEdit3->setObjectName(tr("playedEdit3"));
    playedEdit3->setReadOnly(true);

    changePlayedButton = new QPushButton(this);
    changePlayedButton->setObjectName(tr("changePlayedButton"));
    changePlayedButton->setText(tr("�ϲ���ɫ"));

    changePlayedButton2 = new QPushButton(this);
    changePlayedButton2->setObjectName(tr("changePlayedButton2"));
    changePlayedButton2->setText(tr("�в���ɫ"));

    changePlayedButton3 = new QPushButton(this);
    changePlayedButton3->setObjectName(tr("changePlayedButton3"));
    changePlayedButton3->setText(tr("�²���ɫ"));

    yuSheButton = new QPushButton(this);
    yuSheButton->setObjectName(tr("yuSheButton"));
    yuSheButton->setText(tr("Ԥ�跽��"));

    backToButton = new QPushButton(this);
    backToButton->setObjectName(tr("backToButton"));
    backToButton->setText(tr("�ָ�Ĭ�Ϸ���"));

    //�Ҷ���ʾ
    QGroupBox *shadowGB = new QGroupBox(tr("��ӰЧ��"));

    shadowLabel = new QLabel(this);
    shadowLabel->setObjectName(tr("shadowLabel"));
    shadowLabel->setText(tr("��ӰЧ��"));

    shadowSlider = new QSlider(Qt::Horizontal, this);
    shadowSlider->setObjectName(tr("shadowSlider"));
    shadowSlider->setRange(0, 255);

    shadowSpinBox = new QSpinBox(this);
    shadowSpinBox->setObjectName(tr("shadowSpinBox"));
    shadowSpinBox->setRange(0, 255);

    //���ģ����ʾ
    lrcDemo = new LrcDemo();
    lrcDemo->resize(300, 60);

    jingDianWhite = new QAction(this);
    jingDianWhite->setText(tr("�����"));
    jingDianWhite->setIcon(QIcon(":/app/images/jingDianWhite.png"));

    haiYangBlue = new QAction(this);
    haiYangBlue->setText(tr("������"));
    haiYangBlue->setIcon(QIcon(":/app/images/haiYangBlue.png"));

    huoYanRed = new QAction(this);
    huoYanRed->setText(tr("�����"));
    huoYanRed->setIcon(QIcon(":/app/images/huoYanRed.png"));

    kuXuanBlack = new QAction(this);
    kuXuanBlack->setText(tr("���ź�"));
    kuXuanBlack->setIcon(QIcon(":/app/images/kuXuanBlack.png"));

    qiuSeYellow = new QAction(this);
    qiuSeYellow->setText(tr("��ɫ��"));
    qiuSeYellow->setIcon(QIcon(":/app/images/qiuSeYellow.png"));

    langManPurple = new QAction(this);
    langManPurple->setText(tr("������"));
    langManPurple->setIcon(QIcon(":/app/images/langManPurple.png"));

    ziRanGreen = new QAction(this);
    ziRanGreen->setText(tr("��Ȼ��"));
    ziRanGreen->setIcon(QIcon(":/app/images/ziRanGreen.png"));

    QHBoxLayout *fontLayout = new QHBoxLayout;
    fontLayout->addWidget(fontLabel);
    fontLayout->addWidget(fontCombo);
    fontLayout->addStretch();
    fontLayout->addWidget(fontSizeLabel);
    fontLayout->addWidget(fontSizeCombo);
    fontLayout->addStretch();
    fontLayout->addWidget(fontTypeLabel);
    fontLayout->addWidget(fontTypeCombo);
    fontGB->setLayout(fontLayout);

    QGridLayout *noPlayLayout = new QGridLayout;
    noPlayLayout->addWidget(noPlayEdit, 0, 0);
    noPlayLayout->addWidget(changeNoPlayButton, 0, 1);
    noPlayLayout->addWidget(noPlayEdit2, 1, 0);
    noPlayLayout->addWidget(changeNoPlayButton2, 1, 1);
    noPlayLayout->addWidget(noPlayEdit3, 2, 0);
    noPlayLayout->addWidget(changeNoPlayButton3, 2, 1);
    noPlayGB->setLayout(noPlayLayout);

    QGridLayout *playedLayout = new QGridLayout;
    playedLayout->addWidget(playedEdit, 0, 0);
    playedLayout->addWidget(changePlayedButton, 0, 1);
    playedLayout->addWidget(playedEdit2, 1, 0);
    playedLayout->addWidget(changePlayedButton2, 1, 1);
    playedLayout->addWidget(playedEdit3, 2, 0);
    playedLayout->addWidget(changePlayedButton3, 2, 1);
    playedGB->setLayout(playedLayout);

    QHBoxLayout *shadowLayout = new QHBoxLayout;
    shadowLayout->addWidget(shadowLabel);
    shadowLayout->addWidget(shadowSlider);
    shadowLayout->addWidget(shadowSpinBox);
    shadowLayout->addStretch();
    shadowGB->setLayout(shadowLayout);

    QHBoxLayout *colorLayout = new QHBoxLayout;
    colorLayout->addWidget(noPlayGB);
    colorLayout->addStretch();
    colorLayout->addWidget(playedGB);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(yuSheButton);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(backToButton);

    QVBoxLayout *colorLayout2 = new QVBoxLayout;
    colorLayout2->addLayout(colorLayout);
    colorLayout2->addLayout(buttonsLayout);
    colorGB->setLayout(colorLayout2);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(fontGB);
    mainLayout->addWidget(colorGB);
    mainLayout->addWidget(shadowGB);
    mainLayout->addWidget(lrcDemo);
    this->setLayout(mainLayout);

    connect(changeNoPlayButton, SIGNAL(clicked()), this, SLOT(slot_ChangeColor()));
    connect(changeNoPlayButton2, SIGNAL(clicked()), this, SLOT(slot_ChangeColor()));
    connect(changeNoPlayButton3, SIGNAL(clicked()), this, SLOT(slot_ChangeColor()));
    connect(changePlayedButton, SIGNAL(clicked()), this, SLOT(slot_ChangeColor()));
    connect(changePlayedButton2, SIGNAL(clicked()), this, SLOT(slot_ChangeColor()));
    connect(changePlayedButton3, SIGNAL(clicked()), this, SLOT(slot_ChangeColor()));
    connect(backToButton, SIGNAL(clicked()), this, SLOT(slot_BackToColor()));//�ָ�Ĭ�Ϸ���
    connect(yuSheButton, SIGNAL(clicked()), this, SLOT(slot_YuSheColor()));//Ԥ�跽��
    connect(jingDianWhite, SIGNAL(triggered()), this, SLOT(slot_ChangeYuSheColor()));
    connect(haiYangBlue, SIGNAL(triggered()), this, SLOT(slot_ChangeYuSheColor()));
    connect(huoYanRed, SIGNAL(triggered()), this, SLOT(slot_ChangeYuSheColor()));
    connect(kuXuanBlack, SIGNAL(triggered()), this, SLOT(slot_ChangeYuSheColor()));
    connect(qiuSeYellow, SIGNAL(triggered()), this, SLOT(slot_ChangeYuSheColor()));
    connect(langManPurple, SIGNAL(triggered()), this, SLOT(slot_ChangeYuSheColor()));
    connect(ziRanGreen, SIGNAL(triggered()), this, SLOT(slot_ChangeYuSheColor()));

    connect(fontCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(slot_ChangeLrcDemoFont(QString)));
    connect(fontSizeCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(slot_ChangeLrcDemoFont(QString)));
    connect(fontTypeCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(slot_ChangeLrcDemoFont(QString)));

    connect(noPlayEdit, SIGNAL(textChanged(QString)), this, SLOT(slot_ChangeLrcDemoColor(QString)));
    connect(noPlayEdit2, SIGNAL(textChanged(QString)), this, SLOT(slot_ChangeLrcDemoColor(QString)));
    connect(noPlayEdit3, SIGNAL(textChanged(QString)), this, SLOT(slot_ChangeLrcDemoColor(QString)));

    connect(playedEdit, SIGNAL(textChanged(QString)), this, SLOT(slot_ChangeLrcDemoColor(QString)));
    connect(playedEdit2, SIGNAL(textChanged(QString)), this, SLOT(slot_ChangeLrcDemoColor(QString)));
    connect(playedEdit3, SIGNAL(textChanged(QString)), this, SLOT(slot_ChangeLrcDemoColor(QString)));

    //slider��spinboxͬ������ֵ
    connect(shadowSlider, SIGNAL(valueChanged(int)), shadowSpinBox, SLOT(setValue(int)));
    connect(shadowSpinBox, SIGNAL(valueChanged(int)), shadowSlider, SLOT(setValue(int)));

    connect(shadowSlider, SIGNAL(valueChanged(int)), this, SLOT(slot_ChangelrcDemoShadow(int)));
}

LrcSettingsPage::~LrcSettingsPage()
{
}

//�ı���ɫ
void LrcSettingsPage::slot_ChangeColor()
{
    QPushButton *button = dynamic_cast<QPushButton *>(sender());
    QString preColorStr;
    if (button == changeNoPlayButton)
    {
        preColorStr = noPlayEdit->text();
    }
    else if (button == changeNoPlayButton2)
    {
        preColorStr = noPlayEdit2->text();
    }
    else if (button == changeNoPlayButton3)
    {
        preColorStr = noPlayEdit3->text();
    }
    else if (button == changePlayedButton)
    {
        preColorStr = playedEdit->text();
    }
    else if (button == changePlayedButton2)
    {
        preColorStr = playedEdit2->text();
    }
    else if (button == changePlayedButton3)
    {
        preColorStr = playedEdit3->text();
    }

    QStringList colorList = preColorStr.split(" ");
    int red = colorList.at(0).toInt();
    int green = colorList.at(1).toInt();
    int blue = colorList.at(2).toInt();
    QColor color = QColorDialog::getColor(QColor(red, green, blue), this, tr("ѡ����ɫ"), QColorDialog::DontUseNativeDialog);
    if (color.isValid())
    {
        QString colorStr = QString::number(color.red(), 10) + " " +
                           QString::number(color.green(), 10) + " " +
                           QString::number(color.blue(), 10);

//        QPalette palette;
//        palette.setColor(QPalette::Active, QPalette::Base, QColor(color.red(), color.green(), color.blue()));
        QString colorStr2 = QString("background-color: rgb(%1, %2, %3)").arg(color.red())
                                                                        .arg(color.green())
                                                                        .arg(color.blue());
        if (button == changeNoPlayButton)
        {
            noPlayEdit->setText(colorStr);
            noPlayEdit->setStyleSheet(colorStr2);
        }
        else if (button == changeNoPlayButton2)
        {
            noPlayEdit2->setText(colorStr);
            noPlayEdit2->setStyleSheet(colorStr2);
        }
        else if (button == changeNoPlayButton3)
        {
            noPlayEdit3->setText(colorStr);
            noPlayEdit3->setStyleSheet(colorStr2);
        }
        else if (button == changePlayedButton)
        {
            playedEdit->setText(colorStr);
            playedEdit->setStyleSheet(colorStr2);
        }
        else if (button == changePlayedButton2)
        {
            playedEdit2->setText(colorStr);
            playedEdit2->setStyleSheet(colorStr2);
        }
        else if (button == changePlayedButton3)
        {
            playedEdit3->setText(colorStr);
            playedEdit3->setStyleSheet(colorStr2);
        }
    }
}

//�ָ�Ĭ�Ϸ���
void LrcSettingsPage::slot_BackToColor()
{
    LrcSettingsPage::setFontCombo(CZPlayerConfig::getValue("DEFAULTFONT").toString());
    LrcSettingsPage::setFontSizeCombo(fontSizeStr);
    LrcSettingsPage::setFontTypeCombo(CZPlayerConfig::getValue("DEFAULTFONTTYPE").toString());
    LrcSettingsPage::setNoPlayEdit(noPlayStr);
    LrcSettingsPage::setNoPlayEdit2(noPlayStr2);
    LrcSettingsPage::setNoPlayEdit3(noPlayStr3);
    LrcSettingsPage::setPlayedEdit(playedStr);
    LrcSettingsPage::setPlayedEdit2(playedStr2);
    LrcSettingsPage::setPlayedEdit3(playedStr3);
}

//Ԥ�跽��
void LrcSettingsPage::slot_YuSheColor()
{
    QMenu menu;
    menu.addAction(jingDianWhite);
    menu.addAction(haiYangBlue);
    menu.addAction(huoYanRed);
    menu.addAction(kuXuanBlack);
    menu.addAction(qiuSeYellow);
    menu.addAction(langManPurple);
    menu.addAction(ziRanGreen);
    menu.exec(QCursor::pos());
}

//�ı�Ԥ��Ԥ�跽��
void LrcSettingsPage::slot_ChangeYuSheColor()
{
    QAction *action = dynamic_cast<QAction *>(sender());
    if (action == jingDianWhite)//�����
    {
        LrcSettingsPage::setNoPlayEdit(noPlayWhite);
        LrcSettingsPage::setNoPlayEdit2(noPlayWhite2);
        LrcSettingsPage::setNoPlayEdit3(noPlayWhite3);
        LrcSettingsPage::setPlayedEdit(playedWhite);
        LrcSettingsPage::setPlayedEdit2(playedWhite2);
        LrcSettingsPage::setPlayedEdit3(playedWhite3);
        jingDianWhite->setText(tr("����� ��"));
        haiYangBlue->setText(tr("������"));
        huoYanRed->setText(tr("�����"));
        kuXuanBlack->setText(tr("���ź�"));
        qiuSeYellow->setText(tr("��ɫ��"));
        langManPurple->setText(tr("������"));
        ziRanGreen->setText(tr("��Ȼ��"));
    }
    else if (action == haiYangBlue)//������
    {
        LrcSettingsPage::setNoPlayEdit(noPlayBlue);
        LrcSettingsPage::setNoPlayEdit2(noPlayBlue2);
        LrcSettingsPage::setNoPlayEdit3(noPlayBlue3);
        LrcSettingsPage::setPlayedEdit(playedBlue);
        LrcSettingsPage::setPlayedEdit2(playedBlue2);
        LrcSettingsPage::setPlayedEdit3(playedBlue3);
        jingDianWhite->setText(tr("�����"));
        haiYangBlue->setText(tr("������ ��"));
        huoYanRed->setText(tr("�����"));
        kuXuanBlack->setText(tr("���ź�"));
        qiuSeYellow->setText(tr("��ɫ��"));
        langManPurple->setText(tr("������"));
        ziRanGreen->setText(tr("��Ȼ��"));
    }
    else if (action == huoYanRed)//�����
    {
        LrcSettingsPage::setNoPlayEdit(noPlayRed);
        LrcSettingsPage::setNoPlayEdit2(noPlayRed2);
        LrcSettingsPage::setNoPlayEdit3(noPlayRed3);
        LrcSettingsPage::setPlayedEdit(playedRed);
        LrcSettingsPage::setPlayedEdit2(playedRed2);
        LrcSettingsPage::setPlayedEdit3(playedRed3);
        jingDianWhite->setText(tr("�����"));
        haiYangBlue->setText(tr("������"));
        huoYanRed->setText(tr("����� ��"));
        kuXuanBlack->setText(tr("���ź�"));
        qiuSeYellow->setText(tr("��ɫ��"));
        langManPurple->setText(tr("������"));
        ziRanGreen->setText(tr("��Ȼ��"));
    }
    else if (action == kuXuanBlack)//���ź�
    {
        LrcSettingsPage::setNoPlayEdit(noPlayBlack);
        LrcSettingsPage::setNoPlayEdit2(noPlayBlack2);
        LrcSettingsPage::setNoPlayEdit3(noPlayBlack3);
        LrcSettingsPage::setPlayedEdit(playedBlack);
        LrcSettingsPage::setPlayedEdit2(playedBlack2);
        LrcSettingsPage::setPlayedEdit3(playedBlack3);
        jingDianWhite->setText(tr("�����"));
        haiYangBlue->setText(tr("������"));
        huoYanRed->setText(tr("�����"));
        kuXuanBlack->setText(tr("���ź� ��"));
        qiuSeYellow->setText(tr("��ɫ��"));
        langManPurple->setText(tr("������"));
        ziRanGreen->setText(tr("��Ȼ��"));
    }
    else if (action == qiuSeYellow)//��ɫ��
    {
        LrcSettingsPage::setNoPlayEdit(noPlayYellow);
        LrcSettingsPage::setNoPlayEdit2(noPlayYellow2);
        LrcSettingsPage::setNoPlayEdit3(noPlayYellow3);
        LrcSettingsPage::setPlayedEdit(playedYellow);
        LrcSettingsPage::setPlayedEdit2(playedYellow2);
        LrcSettingsPage::setPlayedEdit3(playedYellow3);
        jingDianWhite->setText(tr("�����"));
        haiYangBlue->setText(tr("������"));
        huoYanRed->setText(tr("�����"));
        kuXuanBlack->setText(tr("���ź�"));
        qiuSeYellow->setText(tr("��ɫ�� ��"));
        langManPurple->setText(tr("������"));
        ziRanGreen->setText(tr("��Ȼ��"));
    }
    else if (action == langManPurple)//������
    {
        LrcSettingsPage::setNoPlayEdit(noPlayPurple);
        LrcSettingsPage::setNoPlayEdit2(noPlayPurple2);
        LrcSettingsPage::setNoPlayEdit3(noPlayPurple3);
        LrcSettingsPage::setPlayedEdit(playedPurple);
        LrcSettingsPage::setPlayedEdit2(playedPurple2);
        LrcSettingsPage::setPlayedEdit3(playedPurple3);
        jingDianWhite->setText(tr("�����"));
        haiYangBlue->setText(tr("������"));
        huoYanRed->setText(tr("�����"));
        kuXuanBlack->setText(tr("���ź�"));
        qiuSeYellow->setText(tr("��ɫ��"));
        langManPurple->setText(tr("������ ��"));
        ziRanGreen->setText(tr("��Ȼ��"));
    }
    else if (action == ziRanGreen)//��Ȼ��
    {
        LrcSettingsPage::setNoPlayEdit(noPlayGreen);
        LrcSettingsPage::setNoPlayEdit2(noPlayGreen2);
        LrcSettingsPage::setNoPlayEdit3(noPlayGreen3);
        LrcSettingsPage::setPlayedEdit(playedGreen);
        LrcSettingsPage::setPlayedEdit2(playedGreen2);
        LrcSettingsPage::setPlayedEdit3(playedGreen3);
        jingDianWhite->setText(tr("����� *"));
        haiYangBlue->setText(tr("������"));
        huoYanRed->setText(tr("�����"));
        kuXuanBlack->setText(tr("���ź�"));
        qiuSeYellow->setText(tr("��ɫ��"));
        langManPurple->setText(tr("������"));
        ziRanGreen->setText(tr("��Ȼ�� ��"));
    }
}

//�ı�������
void LrcSettingsPage::slot_ChangeLrcDemoFont(QString value)
{
    QComboBox *comboBox = dynamic_cast<QComboBox *>(sender());
    if (comboBox == fontCombo)
    {
        LrcDemo::setFontYS(value);
    }
    else if (comboBox == fontSizeCombo)
    {
        LrcDemo::setFontSize(value);
        LrcDemo::setLrcMaskWidth(value.toInt() * 13 / 2);
    }
    else if (comboBox == fontTypeCombo)
    {
        LrcDemo::setFontType(value);
    }
    lrcDemo->update();
}

//�ı���������ɫ
void LrcSettingsPage::slot_ChangeLrcDemoColor(QString value)
{
    QLineEdit *lineEdit = dynamic_cast<QLineEdit *>(sender());
    if (lineEdit == noPlayEdit)
    {
        LrcDemo::setLinearGradient(value);
    }
    else if (lineEdit == noPlayEdit2)
    {
        LrcDemo::setLinearGradient2(value);
    }
    else if (lineEdit == noPlayEdit3)
    {
        LrcDemo::setLinearGradient3(value);
    }
    else if (lineEdit == playedEdit)
    {
        LrcDemo::setMaskLinearGradient(value);
    }
    else if (lineEdit == playedEdit2)
    {
        LrcDemo::setMaskLinearGradient2(value);
    }
    else if (lineEdit == playedEdit3)
    {
        LrcDemo::setMaskLinearGradient3(value);
    }
    lrcDemo->update();
}

//�ı�lrcDemo��ʻҶ�
void LrcSettingsPage::slot_ChangelrcDemoShadow(int value)
{
    LrcDemo::setLrcDemoShadow(value);
    lrcDemo->update();
}

//�õ�Ԥ����ɫ
QString LrcSettingsPage::getYuSheColor()
{
    QString str = "";
    if (jingDianWhite->text().contains("��"))
    {
        str = "�����";
    }
    else if (haiYangBlue->text().contains("��"))
    {
        str =  "������";
    }
    else if (huoYanRed->text().contains("��"))
    {
        str =  "�����";
    }
    else if (kuXuanBlack->text().contains("��"))
    {
        str =  "���ź�";
    }
    else if (qiuSeYellow->text().contains("��"))
    {
        str =  "��ɫ��";
    }
    else if (langManPurple->text().contains("��"))
    {
        str = "������";
    }
    else if (ziRanGreen->text().contains("��"))
    {
        str = "��Ȼ��";
    }
    return str;
}

//����Ԥ����ɫ
void LrcSettingsPage::setYuSheColor(const QString &value)
{
    if (value == "�����")
    {
        jingDianWhite->setText(tr("����� ��"));
        haiYangBlue->setText(tr("������"));
        huoYanRed->setText(tr("�����"));
        kuXuanBlack->setText(tr("���ź�"));
        qiuSeYellow->setText(tr("��ɫ��"));
        langManPurple->setText(tr("������"));
        ziRanGreen->setText(tr("��Ȼ��"));
    }
    else if (value == "������")
    {
        jingDianWhite->setText(tr("�����"));
        haiYangBlue->setText(tr("������ ��"));
        huoYanRed->setText(tr("�����"));
        kuXuanBlack->setText(tr("���ź�"));
        qiuSeYellow->setText(tr("��ɫ��"));
        langManPurple->setText(tr("������"));
        ziRanGreen->setText(tr("��Ȼ��"));
    }
    else if (value == "�����")
    {
        jingDianWhite->setText(tr("�����"));
        haiYangBlue->setText(tr("������"));
        huoYanRed->setText(tr("����� ��"));
        kuXuanBlack->setText(tr("���ź�"));
        qiuSeYellow->setText(tr("��ɫ��"));
        langManPurple->setText(tr("������"));
        ziRanGreen->setText(tr("��Ȼ��"));
    }
    else if (value == "���ź�")
    {
        jingDianWhite->setText(tr("�����"));
        haiYangBlue->setText(tr("������"));
        huoYanRed->setText(tr("�����"));
        kuXuanBlack->setText(tr("���ź� ��"));
        qiuSeYellow->setText(tr("��ɫ��"));
        langManPurple->setText(tr("������"));
        ziRanGreen->setText(tr("��Ȼ��"));
    }
    else if (value == "��ɫ��")
    {
        jingDianWhite->setText(tr("�����"));
        haiYangBlue->setText(tr("������"));
        huoYanRed->setText(tr("�����"));
        kuXuanBlack->setText(tr("���ź�"));
        qiuSeYellow->setText(tr("��ɫ�� ��"));
        langManPurple->setText(tr("������"));
        ziRanGreen->setText(tr("��Ȼ��"));
    }
    else if (value == "������")
    {
        jingDianWhite->setText(tr("�����"));
        haiYangBlue->setText(tr("������"));
        huoYanRed->setText(tr("�����"));
        kuXuanBlack->setText(tr("���ź�"));
        qiuSeYellow->setText(tr("��ɫ��"));
        langManPurple->setText(tr("������ ��"));
        ziRanGreen->setText(tr("��Ȼ��"));
    }
    else if (value == "��Ȼ��")
    {
        jingDianWhite->setText(tr("����� *"));
        haiYangBlue->setText(tr("������"));
        huoYanRed->setText(tr("�����"));
        kuXuanBlack->setText(tr("���ź�"));
        qiuSeYellow->setText(tr("��ɫ��"));
        langManPurple->setText(tr("������"));
        ziRanGreen->setText(tr("��Ȼ�� ��"));
    }
}

QString LrcSettingsPage::getShadowSlider()
{
    return QString::number(shadowSlider->value(), 10);
}

void LrcSettingsPage::setShadowSlider(const int &value)
{
    shadowSlider->setValue(value);
}

QString LrcSettingsPage::getFontTypeCombo()
{
    return fontTypeCombo->currentText();
}

void LrcSettingsPage::setFontTypeCombo(QString const &value)
{
    for (int i = 0; i < fontTypeCombo->count(); ++i)
    {
        if (value == fontTypeCombo->itemText(i))
        {
            fontTypeCombo->setCurrentIndex(i);
        }
    }
}

QString LrcSettingsPage::getFontSizeCombo()
{
    return fontSizeCombo->currentText();
}

void LrcSettingsPage::setFontSizeCombo(QString const &value)
{
    for (int i = 0; i < fontSizeCombo->count(); ++i)
    {
        if (value == fontSizeCombo->itemText(i))
        {
            fontSizeCombo->setCurrentIndex(i);
        }
    }
}

QString LrcSettingsPage::getFontCombo()
{
    return fontCombo->currentText();
}

void LrcSettingsPage::setFontCombo(QString const &value)
{
    for (int i = 0; i < fontCombo->count(); ++i)
    {
        if (value == fontCombo->itemText(i))
        {
            fontCombo->setCurrentIndex(i);
        }
    }
}

QString LrcSettingsPage::getPlayedEdit3()
{
    return playedEdit3->text();
}

void LrcSettingsPage::setPlayedEdit3(QString const &value)
{
    playedEdit3->setText(value);
    LrcSettingsPage::setColor(playedEdit3, value);
}

QString LrcSettingsPage::getPlayedEdit2()
{
    return playedEdit2->text();
}

void LrcSettingsPage::setPlayedEdit2(QString const &value)
{
    playedEdit2->setText(value);
    LrcSettingsPage::setColor(playedEdit2, value);
}

QString LrcSettingsPage::getPlayedEdit()
{
    return playedEdit->text();
}

void LrcSettingsPage::setPlayedEdit(QString const &value)
{
    playedEdit->setText(value);
    LrcSettingsPage::setColor(playedEdit, value);
}

QString LrcSettingsPage::getNoPlayEdit3()
{
    return noPlayEdit3->text();
}

void LrcSettingsPage::setNoPlayEdit3(QString const &value)
{
    noPlayEdit3->setText(value);
    LrcSettingsPage::setColor(noPlayEdit3, value);
}

QString LrcSettingsPage::getNoPlayEdit2()
{
    return noPlayEdit2->text();
}

void LrcSettingsPage::setNoPlayEdit2(QString const &value)
{
    noPlayEdit2->setText(value);
    LrcSettingsPage::setColor(noPlayEdit2, value);
}

QString LrcSettingsPage::getNoPlayEdit()
{
    return noPlayEdit->text();
}

void LrcSettingsPage::setNoPlayEdit(QString const &value)
{
    noPlayEdit->setText(value);
    LrcSettingsPage::setColor(noPlayEdit, value);
}

//����QLineEdit��ɫ
void LrcSettingsPage::setColor(QLineEdit *lineEdit, const QString &value)
{
    QStringList colorList = value.split(" ");
    QString redStr = colorList.at(0);
    QString greenStr = colorList.at(1);
    QString blueStr = colorList.at(2);
    QString colorStr = QString("background-color: rgb(%1, %2, %3)").arg(redStr).arg(greenStr).arg(blueStr);
    lineEdit->setStyleSheet(colorStr);
}









