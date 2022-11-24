#include "devicewatcher.h"

#ifdef Q_OS_WIN

uDeviceWatcher* gDeviceWatcher = Q_NULLPTR;

uDeviceWatcher::uDeviceWatcher(QWidget* parent) : QWidget(parent) {
    qApp->installNativeEventFilter(this);

    DEV_BROADCAST_DEVICEINTERFACE mouseInterface;
    ZeroMemory(&mouseInterface, sizeof(DEV_BROADCAST_DEVICEINTERFACE));
    mouseInterface.dbcc_size       = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    mouseInterface.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    mouseInterface.dbcc_classguid  = GUID_DEVINTERFACE_MOUSE;
    if (!RegisterDeviceNotificationW((HANDLE)this->winId(), &mouseInterface, DEVICE_NOTIFY_WINDOW_HANDLE))
        qDebug() << "failed to register mouse guid";

    DEV_BROADCAST_DEVICEINTERFACE keyboardInterface;
    ZeroMemory(&keyboardInterface, sizeof(DEV_BROADCAST_DEVICEINTERFACE));
    keyboardInterface.dbcc_size       = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    keyboardInterface.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    keyboardInterface.dbcc_classguid  = GUID_DEVINTERFACE_KEYBOARD;
    if (!RegisterDeviceNotificationW((HANDLE)this->winId(), &keyboardInterface, DEVICE_NOTIFY_WINDOW_HANDLE))
        qDebug() << "failed to register keyboard guid";

    connect(this, &uDeviceWatcher::comStateChanged, [](QString com, bool state) { qDebug() << com << ":" << (state ? "insert-in" : "pull-out"); });
    connect(this, &uDeviceWatcher::mouseStateChanged, [](QString guid, bool state) { qDebug() << "mouse:" << (state ? "insert-in" : "pull-out"); });
    connect(this, &uDeviceWatcher::keyboardStateChanged, [](QString guid, bool state) { qDebug() << "keyboard:" << (state ? "insert-in" : "pull-out"); });

}

bool uDeviceWatcher::nativeEventFilter(const QByteArray& eventType, void* message, long* result) {
    if (eventType == "windows_generic_MSG") {
        MSG* msg = reinterpret_cast<MSG*>(message);
        // 热插拔监测
        if (msg->message == WM_DEVICECHANGE && msg->hwnd == HWND(this->winId())) {
            auto lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
            switch (msg->wParam) {
                // 设备插入
                case DBT_DEVICEARRIVAL: {
                    switch (lpdb->dbch_devicetype) {
                        case DBT_DEVTYP_PORT: {  // 串口
                            auto lpdbv = (PDEV_BROADCAST_PORT)lpdb;
                            emit comStateChanged(QString::fromWCharArray(lpdbv->dbcp_name), true);
                            break;
                        }
                        case DBT_DEVTYP_DEVICEINTERFACE: {
                            auto lpdbv = (PDEV_BROADCAST_DEVICEINTERFACE)lpdb;
                            if (lpdbv->dbcc_classguid == GUID_DEVINTERFACE_MOUSE) {  // 鼠标
                                emit mouseStateChanged(QString::fromWCharArray(lpdbv->dbcc_name), true);
                            } else if (lpdbv->dbcc_classguid == GUID_DEVINTERFACE_KEYBOARD) {  //键盘
                                emit keyboardStateChanged(QString::fromWCharArray(lpdbv->dbcc_name), true);
                            }
                            break;
                        }
                    }
                    break;
                }
                // 设备拔出
                case DBT_DEVICEREMOVECOMPLETE: {
                    switch (lpdb->dbch_devicetype) {
                        case DBT_DEVTYP_PORT: {  // 串口
                            auto lpdbv = (PDEV_BROADCAST_PORT)lpdb;
                            emit comStateChanged(QString::fromWCharArray(lpdbv->dbcp_name), false);
                            break;
                        }
                        case DBT_DEVTYP_DEVICEINTERFACE: {
                            auto lpdbv = (PDEV_BROADCAST_DEVICEINTERFACE)lpdb;
                            if (lpdbv->dbcc_classguid == GUID_DEVINTERFACE_MOUSE) {  // 鼠标
                                emit mouseStateChanged(QString::fromWCharArray(lpdbv->dbcc_name), false);
                            } else if (lpdbv->dbcc_classguid == GUID_DEVINTERFACE_KEYBOARD) {  //键盘
                                emit keyboardStateChanged(QString::fromWCharArray(lpdbv->dbcc_name), false);
                            }
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
    return false;
}

#endif
