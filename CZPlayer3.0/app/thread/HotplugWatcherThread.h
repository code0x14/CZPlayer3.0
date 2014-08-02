#ifndef HOTPLUGWATCHERTHTREAD_H
#define HOTPLUGWATCHERTHTREAD_H

#ifndef __GNUC__
#define __PRETTY_FUNCTION__  __FUNCTION__
#endif

#include "head.h"
#include "qdevicewatcher.h"
#include "qdevicechangeevent.h"

class HotplugWatcherThread : public QThread
{
	Q_OBJECT

public:
	HotplugWatcherThread(QObject *parent = 0);
	~HotplugWatcherThread(void);

protected:
	//virtual void run();
	virtual bool event(QEvent *e);

public slots:
	void slot_DeviceAdded(const QString &dev);			//�����豸
	void slot_DeviceRemoved(const QString &dev);		//�Ƴ��豸
	void slot_DeviceChanged(const QString &dev);		//�ı��豸

private:
	QDeviceWatcher *watcher;						//�豸����ָ��
};

#endif // HOTPLUGWATCHERTHTREAD_H

