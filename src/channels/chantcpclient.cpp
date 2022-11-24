#include "chantcpclient.h"
#include "ui_chantcpclient.h"

chanTCPClient::chanTCPClient(QWidget* parent) : uAbstractDataChannel(parent),
                                                ui(new Ui::chanTCPClient),
                                                mChannel(new QTcpSocket(this)),
                                                mTmrReconnect(new QTimer(this)) {
    ui->setupUi(this);

    //////////////////////////////////////////////////////////////////////

    // left double click label and then copy ip / port to clipboard 左键双击复制

    ui->lbl_server_ip->installEventFilter(this);
    ui->lbl_server_port->installEventFilter(this);
    ui->lbl_server_addr->installEventFilter(this);
    ui->lbl_client_port->installEventFilter(this);
    ui->lbl_client_addr->installEventFilter(this);

    //////////////////////////////////////////////////////////////////////

    // enum local ip 枚举本机ip

    ui->cmb_server_ip->addItems(getlocalip());

    //////////////////////////////////////////////////////////////////////

    // auto reconnect 自动重连

    ui->lbl_reconnect->setVisible(false);
    connect(ui->chk_reconnect, &QCheckBox::stateChanged, [&](int state) {
        if (state == Qt::Unchecked && mTmrReconnect->isActive()) {
            mTmrReconnect->stop();
            mTmrReconnect->setProperty("stop", QVariant(true));
            ui->lbl_reconnect->setVisible(false);
        }
    });

    connect(mTmrReconnect, &QTimer::timeout, [&]() {
        if (ui->lbl_reconnect->text().size() < 3) {  // 动画
            ui->lbl_reconnect->setText(ui->lbl_reconnect->text() + ".");
        } else {
            ui->lbl_reconnect->setText(QString("."));
        }
        open();
    });

    void (QTcpSocket::*signalSocketError)(QAbstractSocket::SocketError) = &QTcpSocket::error;
    connect(mChannel, signalSocketError, [&](QAbstractSocket::SocketError e) {  // SocketError 先于 disconnected 处理
        // 自动重连中 ( 在调用 connectToHost() 之后, 如在几秒后没连接上服务器, 会发出 SocketError 信号, 即使调用了 close() 也还会发出该信号 )
        if (mTmrReconnect->isActive()) {
            return;
        } else if (mTmrReconnect->property("stop").toBool()) {
            mTmrReconnect->setProperty("stop", QVariant(false));
            return;
        }

        // 异常处理
        qDebug() << mChannel->state() << e << mChannel->errorString();
        switch (e) {
            case QAbstractSocket::RemoteHostClosedError:  // 服务器被关闭
            {
                if (ui->chk_reconnect->checkState() == Qt::Checked) {
                    ui->lbl_reconnect->setText(".");
                    ui->lbl_reconnect->setVisible(true);
                    mTmrReconnect->start(1000);
                }
                break;
            }
            default: {
                QMessageBox::warning(this, QLatin1String("ERROR"), QLatin1String("fail to connect server"));
                break;
            }
        }
    });

    //////////////////////////////////////////////////////////////////////

    // 连接状态变更

    connect(mChannel, &QTcpSocket::connected, [&]() {
        // stop automatic reconnection
        mTmrReconnect->stop();
        ui->lbl_reconnect->setVisible(false);
        // set widgets disable
        setWidgetsEnable(false, ui->cmb_server_ip, ui->spn_server_port, ui->spn_client_port, Q_NULLPTR);
        // add ip to combobox
        if (ui->cmb_server_ip->findText(ui->cmb_server_ip->currentText()) == -1)
            ui->cmb_server_ip->addItem(ui->cmb_server_ip->currentText());
        // current tcp client port
        if (bAutoAssignPort) ui->spn_client_port->setValue(mChannel->localPort());
        emit runStateChanged(true);
    });

    connect(mChannel, &QTcpSocket::disconnected, [&]() {
        // set widgets enable
        setWidgetsEnable(true, ui->cmb_server_ip, ui->spn_server_port, ui->spn_client_port, Q_NULLPTR);
        // if auto assign port, set input of client port to 0
        if (bAutoAssignPort) ui->spn_client_port->setValue(0);
        emit runStateChanged(false);
    });

    // 数据读取

    connect(mChannel, &QTcpSocket::readyRead, [=]() { while(mChannel->bytesAvailable()) emit readyReadLine(mChannel->readLine()); });
}

chanTCPClient::~chanTCPClient() {
    delete ui;
}

void chanTCPClient::open() {
    // bind local port
    bAutoAssignPort = ui->spn_client_port->value() == 0;
    if (!bAutoAssignPort) mChannel->bind(ui->spn_client_port->value());
    // connect to server
    mChannel->connectToHost(
        ui->cmb_server_ip->currentText(),
        ui->spn_server_port->text().toUInt(),
        QIODevice::ReadWrite, QAbstractSocket::IPv4Protocol);
}

void chanTCPClient::close() {
    mChannel->close();
}

void chanTCPClient::write(const QByteArray& bytes) {
    mChannel->write(bytes);
}

bool chanTCPClient::eventFilter(QObject* watched, QEvent* event) {
    // 复制 ip / port / ip:port 到剪辑版

    if (event->type() == QEvent::MouseButtonDblClick) {
        if (watched == ui->lbl_server_ip) {
            QApplication::clipboard()->setText(ui->cmb_server_ip->currentText());
        } else if (watched == ui->lbl_server_port) {
            QApplication::clipboard()->setText(ui->spn_server_port->text());
        } else if (watched == ui->lbl_server_addr) {
            QApplication::clipboard()->setText(QString("%1:%2").arg(ui->cmb_server_ip->currentText()).arg(ui->spn_server_port->text()));
        } else if (watched == ui->lbl_client_port) {
            if (ui->spn_client_port->value() > 0)
                QApplication::clipboard()->setText(ui->spn_client_port->text());
        } else if (watched == ui->lbl_client_addr) {
            if (ui->spn_client_port->value() > 0)
                QApplication::clipboard()->setText(QString("127.0.0.1:%1").arg(ui->spn_client_port->text()));
        }
    }

    return uAbstractDataChannel::eventFilter(watched, event);
}
