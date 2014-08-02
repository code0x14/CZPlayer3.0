#include "HotplugWatcherThread.h"

HotplugWatcherThread::HotplugWatcherThread(QObject *parent) : QThread(parent)
{
	qDebug("��ǰ�߳�id:%#x, %s", (unsigned int)QThread::currentThreadId(), __PRETTY_FUNCTION__);
	this->start();
	this->moveToThread(this); //Let bool event(QEvent *e) be in another thread

	watcher = new QDeviceWatcher;
	watcher->moveToThread(this);
	watcher->appendEventReceiver(this);

	connect(watcher, SIGNAL(deviceAdded(QString)), this, SLOT(slot_DeviceAdded(QString)));
	connect(watcher, SIGNAL(deviceChanged(QString)), this, SLOT(slot_DeviceChanged(QString)));
	connect(watcher, SIGNAL(deviceRemoved(QString)), this, SLOT(slot_DeviceRemoved(QString)));
	watcher->start();
}


HotplugWatcherThread::~HotplugWatcherThread(void)
{
}

//void HotplugWatcherThread::run()
//{
//
//}

//�����豸
void HotplugWatcherThread::slot_DeviceAdded( const QString &dev )
{
	//qDebug() << "�����豸";
	qDebug("��ǰ�߳�id:%#x, %s, ����:%s", (unsigned int) QThread::currentThreadId(), __PRETTY_FUNCTION__, qPrintable(dev));
}

//�Ƴ��豸
void HotplugWatcherThread::slot_DeviceRemoved( const QString &dev )
{
	//qDebug() << "�Ƴ��豸";
	qDebug("��ǰ�߳�id:%#x, %s, �Ƴ�:%s", (unsigned int) QThread::currentThreadId(), __PRETTY_FUNCTION__, qPrintable(dev));
}

//�ı��豸
void HotplugWatcherThread::slot_DeviceChanged( const QString &dev )
{
	//qDebug() << "�ı��豸";
	qDebug("��ǰ�߳�id:#x, %s, �ı�:%s", (unsigned int)QThread::currentThreadId(), __PRETTY_FUNCTION__, qPrintable(dev));
}

bool HotplugWatcherThread::event(QEvent *e)
{
	if (e->type() == QDeviceChangeEvent::registeredType()) 
	{
		QDeviceChangeEvent *event = dynamic_cast<QDeviceChangeEvent*>(e);
		QString action("�ı�");

		if (event->action() == QDeviceChangeEvent::Add)
		{
			action = "����";
		}
		else if (event->action() == QDeviceChangeEvent::Remove)
		{
			action = "�Ƴ�";
		}

		qDebug("��ǰ�߳�id:%#x, ����:%d, %s: %s %s", (unsigned int)QThread::currentThreadId(), e->type(), __PRETTY_FUNCTION__, qPrintable(action), qPrintable(event->device()));
		event->accept();
		return true;
	}
	return QObject::event(e);
}