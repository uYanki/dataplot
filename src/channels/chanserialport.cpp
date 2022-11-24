#include "chanserialport.h"
#include "ui_chanserialport.h"

chanSerialPort::chanSerialPort(QWidget* parent) : uAbstractDataChannel(parent),
                                                  ui(new Ui::chanSerialPort),
                                                  mChannel(new QSerialPort(this)) {
    ui->setupUi(this);

    //////////////////////////////////////////////////////////////////////

    // mChannel->setBaudRate(QSerialPort::BaudRate::Baud115200);
    // mChannel->setDataBits(QSerialPort::DataBits::Data8);
    // mChannel->setStopBits(QSerialPort::StopBits::OneStop);
    // mChannel->setParity(QSerialPort::Parity::NoParity);
    // mChannel->setFlowControl(QSerialPort::FlowControl::NoFlowControl);

    void (QComboBox::*pComboBoxSignal)(int) = &QComboBox::currentIndexChanged;
    connect(ui->cmb_com, pComboBoxSignal, [=](int) {
        ui->lbl_desc->setText(ui->cmb_com->currentData().toString());
        ui->lbl_desc->setToolTip(ui->cmb_com->currentData().toString());
    });
    connect(ui->cmb_baudrate, pComboBoxSignal, [=](int) { mChannel->setBaudRate(ui->cmb_baudrate->currentText().toInt()); });
    connect(ui->cmb_databits, pComboBoxSignal, [=](int) { mChannel->setDataBits(ui->cmb_databits->currentData().value<QSerialPort::DataBits>()); });
    connect(ui->cmb_stopbits, pComboBoxSignal, [=](int) { mChannel->setStopBits(ui->cmb_stopbits->currentData().value<QSerialPort::StopBits>()); });
    connect(ui->cmb_parity, pComboBoxSignal, [=](int) { mChannel->setParity(ui->cmb_parity->currentData().value<QSerialPort::Parity>()); });

    // 端口号 com

#ifdef Q_OS_WIN
    // 监测串口的热插拔
    connect(gDeviceWatcher, &uDeviceWatcher::comStateChanged, [=](QString com, bool state) {
        int index = ui->cmb_com->findText(com);
        if (state) {  // append
            if (index == -1) ui->cmb_com->addItem(com, QSerialPortInfo(com).description());
        } else {  // remove
            if (mRunState && index == ui->cmb_com->currentIndex()) close();
            ui->cmb_com->removeItem(index);
        }
        ui->lbl_desc->setVisible(ui->cmb_com->count() > 0);
    });
#else
    // 鼠标移入时对串口进行枚举
    ui->cmb_com->installEventFilter(this);

    // 串口断开监测
    connect(mChannel, &QSerialPort::errorOccurred, [&](QSerialPort::SerialPortError e) {
        // handle error
        if (e != QSerialPort::NoError) {
            emit reset(mChannel->errorString());
            mChannel->clearError();
            // wait for device to reload
            QEventLoop loop;
            QTimer::singleShot(1000, &loop, SLOT(quit()));  // delay_ms(1000)
            loop.exec();
            // scan serial port
            scan();
        }
    });
#endif
    scan();  // 扫描串口

    // 端口描述 description

    ui->lbl_desc->setVisible(false);

    // 波特率 baudrate

    ui->cmb_baudrate->addItems(QStringList() << "4800"
                                             << "9600"
                                             << "57600"
                                             << "115200"
                                             << "460800"
                                             << "921600");
    ui->cmb_baudrate->setCurrentIndex(3);  // 115200

    // 数据位 databits

    ui->cmb_databits->addItem("8", QSerialPort::DataBits::Data8);
    ui->cmb_databits->addItem("7", QSerialPort::DataBits::Data7);
    ui->cmb_databits->addItem("6", QSerialPort::DataBits::Data6);
    ui->cmb_databits->addItem("5", QSerialPort::DataBits::Data5);

    // 停止位 stopbits

    ui->cmb_stopbits->addItem("1", QSerialPort::StopBits::OneStop);
    ui->cmb_stopbits->addItem("1.5", QSerialPort::StopBits::OneAndHalfStop);
    ui->cmb_stopbits->addItem("2", QSerialPort::StopBits::TwoStop);

    // 奇偶校验位 parity

    ui->cmb_parity->addItem("No", QSerialPort::Parity::NoParity);
    ui->cmb_parity->addItem("Even", QSerialPort::Parity::EvenParity);
    ui->cmb_parity->addItem("Odd", QSerialPort::Parity::OddParity);
    ui->cmb_parity->addItem("Space", QSerialPort::Parity::SpaceParity);
    ui->cmb_parity->addItem("Mark", QSerialPort::Parity::MarkParity);

    //////////////////////////////////////////////////////////////////////

    // 流控信号 flow control signals

    connect(ui->chk_DTR, &QCheckBox::toggled, [=](bool b) { mChannel->setDataTerminalReady(b); });
    connect(ui->chk_RTS, &QCheckBox::toggled, [=](bool b) { mChannel->setRequestToSend(b); });

    //////////////////////////////////////////////////////////////////////

    // 设备管理器

    connect(ui->btn_devmgr, &QPushButton::clicked, []() { QDesktopServices::openUrl(QUrl("file:///C:/Windows/System32/devmgmt.msc", QUrl::TolerantMode)); });

    //////////////////////////////////////////////////////////////////////

    // 数据读取

    connect(mChannel, &QSerialPort::readyRead, [&]() { while(mChannel->bytesAvailable()) emit readyReadLine(mChannel->readLine()); });

    // 状态变更

    connect(this, &uAbstractDataChannel::runStateChanged, [=](bool state) {
        ui->cmb_com->setEnabled(!state);
        ui->chk_DTR->setEnabled(state);
        ui->chk_RTS->setEnabled(state);
    });
}

chanSerialPort::~chanSerialPort() {
    delete ui;
}

void chanSerialPort::open() {
    if (ui->cmb_com->count() == 0) {
        QMessageBox::warning(this, QLatin1String("ERROR"), QLatin1String("not available com"));  // 没有可用的串口
        return;
    }

    mChannel->setPortName(ui->cmb_com->currentText());
    if (mChannel->open(QIODevice::ReadWrite)) {
        emit runStateChanged(true);
    } else {
        QMessageBox::warning(this, QLatin1String("ERROR"), QLatin1String("fail to open serial port"));  // 端口被占用
    }
}

void chanSerialPort::close() {
    mChannel->close();
    mChannel->clear();       // 清除缓冲
    mChannel->clearError();  // 清除错误
    emit runStateChanged(false);
}

void chanSerialPort::write(const QByteArray& bytes) {
    mChannel->write(bytes);
}

void chanSerialPort::scan() {
    ui->cmb_com->clear();
    Q_FOREACH (const QSerialPortInfo& info, QSerialPortInfo::availablePorts()) {
        // qDebug() << info.isBusy();
        ui->cmb_com->addItem(info.portName(), info.description());
    }
    ui->lbl_desc->setVisible(ui->cmb_com->count() > 0);
}

#ifndef Q_OS_WIN

bool chanSerialPort::eventFilter(QObject* watched, QEvent* event) {
    // scan available serial ports when hover enter
    if (watched == ui->cmb_com && event->type() == QEvent::HoverEnter && mRunState == false) scan();
    return uAbstractDataChannel::eventFilter(watched, event);
}

#endif
