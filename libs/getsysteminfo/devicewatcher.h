#ifndef DEVWATCHER_H
#define DEVWATCHER_H

/**************** 硬件热插拔监测(仅用于Windows) ****************/

#include <QAbstractNativeEventFilter>
#include <QApplication>
#include <QDebug>
#include <QWidget>

#ifdef Q_OS_WIN

#include <windows.h>
#include "dbt.h"
#include "hidclass.h"
#include "initguid.h"
#include "ntddkbd.h"
#include "ntddmou.h"
#include "usbiodef.h"
// #pragma comment(lib, "user32.lib")

class uDeviceWatcher : public QWidget, public QAbstractNativeEventFilter {
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(uDeviceWatcher)  // 禁用拷贝构造，赋值构造，移动构造

public:
    uDeviceWatcher(QWidget* parent = Q_NULLPTR);
    ~uDeviceWatcher() {}

signals:
    /* state: true = insert-in, false = pull-out */
    void comStateChanged(QString com, bool state);
    void mouseStateChanged(QString guid, bool state);
    void keyboardStateChanged(QString guid, bool state);

protected:
    bool nativeEventFilter(const QByteArray& eventType, void* message, long* result) Q_DECL_OVERRIDE;
};

extern uDeviceWatcher* gDeviceWatcher;

#endif

#define initDeviceWatcher() gDeviceWatcher = new uDeviceWatcher(nullptr)

// #ifdef Q_OS_WIN
// initDeviceWatcher();
// #endif

#endif  // UDEVWATCHER_H
