#ifndef CHANBLCLIENT_H
#define CHANBLCLIENT_H

#include <QWidget>
#include <QListWidgetItem>

#include <QBluetoothSocket>
#include <QBluetoothLocalDevice>
#include <QBluetoothServiceDiscoveryAgent> // 蓝牙扫描
#include <QBluetoothUuid>
#include <QBluetoothServiceInfo>
#include <QLowEnergyController> // 低功耗
#include <QLowEnergyService>
#include <QLowEnergyDescriptor>

#include <QClipboard>

#include "uabstractdatachannel.h"
#include "setwidgetsprop.h"

namespace Ui {
    class chanBLClient;
}

class chanBLClient : public uAbstractDataChannel {
    Q_OBJECT

private:
    Ui::chanBLClient* ui;
    bool eventFilter(QObject* watched, QEvent* event) Q_DECL_OVERRIDE;


public:
    explicit chanBLClient(QWidget* parent = nullptr);
    ~chanBLClient();

public:
    void open() Q_DECL_OVERRIDE;
    void close() Q_DECL_OVERRIDE;
    void write(const QByteArray& bytes) Q_DECL_OVERRIDE;

private:
    void scan();
    void refreshLocalInfo();

    QBluetoothLocalDevice  *  mLocalDevice; // 设备操作
     QBluetoothSocket * const mChannel; // 数据传输
    QBluetoothDeviceDiscoveryAgent  * const mScanner; // 蓝牙扫描

};

#endif  // chanBLClient_H
