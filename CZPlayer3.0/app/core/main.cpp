#include "widget.h"
#include "static.h"
#include <QApplication>
#include <qtsingleapplication.h>

//����֧��
void initqt_zh_CN()
{
	//���ñ����ʽ
#ifdef _WIN32_WINNT
	QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
#else
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("gbk"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("gbk"));
#endif
	QTranslator *translator = new QTranslator();
	translator->load(QString(":app/qt_zh_CN/qt_zh_CN"), QCoreApplication::applicationDirPath(), NULL, NULL);
	qApp->installTranslator(translator);
}

int main(int argc, char *argv[])
{
	QtSingleApplication app(argc, argv);
	//QApplication app(argc, argv);
	if (app.isRunning())
	{
		//qDebug() << "�����Ѿ�������";
		return EXIT_SUCCESS;
	}
	app.setApplicationName("CZPlayer");

	initqt_zh_CN();//����֧��
	//connectFileToProgram();

	//��������qss��ʽ��
	QApplication::setStyle(new QPlastiqueStyle);
	QFile file(":app/qss/musicPlayer.qss");
	file.open(QFile::ReadOnly);
	app.setStyleSheet(file.readAll());
	file.close();

	//��ʾ������
	Widget w(argv[1]);
	w.show();

	return app.exec();
}
