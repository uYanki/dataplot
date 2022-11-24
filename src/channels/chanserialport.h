#ifndef CHANSERIALPORT_H
#define CHANSERIALPORT_H

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QMessageBox>

#include <QDesktopServices>
#include <QUrl>

#include <QEventLoop>
#include <QTimer>

#include "devicewatcher.h"
#include "uabstractdatachannel.h"

namespace Ui {
    class chanSerialPort;
}

class chanSerialPort : public uAbstractDataChannel {
    Q_OBJECT
private:
    Ui::chanSerialPort* ui;

public:
    explicit chanSerialPort(QWidget* parent = nullptr);
    ~chanSerialPort();

private:
#ifndef Q_OS_WIN
    bool eventFilter(QObject* watched, QEvent* event) Q_DECL_OVERRIDE;
#endif

public:
    void open() Q_DECL_OVERRIDE;
    void close() Q_DECL_OVERRIDE;
    void write(const QByteArray& bytes) Q_DECL_OVERRIDE;

private:
    void scan();  // 扫描串口

    QSerialPort* const mChannel;
};

#endif  // CHANSERIALPORT_H
