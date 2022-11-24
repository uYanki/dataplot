#include "chantcpserver.h"
#include "ui_chantcpserver.h"

chanTCPServer::chanTCPServer(QWidget* parent) : uAbstractDataChannel(parent),
                                                ui(new Ui::chanTCPServer),
                                                mChannel(new QTcpServer(this)) {
    ui->setupUi(this);

    //////////////////////////////////////////////////////////////////////

    ui->lbl_server_ip->installEventFilter(this);
    ui->lbl_server_port->installEventFilter(this);

    //////////////////////////////////////////////////////////////////////

    // enum local ip 枚举本机ip

    ui->cmb_server_ip->addItems(getlocalip());

    //////////////////////////////////////////////////////////////////////

    // client list menu 客户列表的右键菜单

    mMenu = new QMenu(this);

    mMenu->addAction("select all", [=]() { enumClients([](QCheckBox* chk) { chk->setCheckState(Qt::Checked); }); });
    mMenu->addAction("unselect all", [=]() { enumClients([](QCheckBox* chk) { chk->setCheckState(Qt::Unchecked); }); });
    mMenu->addAction("reverse all", [=]() { enumClients([](QCheckBox* chk) { chk->setCheckState(chk->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked); }); });

    mMenu->addSeparator();

    mMenu->addAction("disconnect this", [=] { ui->list_client_addr->selectedItems().first()->data(Qt::UserRole).value<QTcpSocket*>()->deleteLater(); });
    mMenu->addAction("disconnect selected", [=] { enumClients([](QCheckBox* chk, QTcpSocket* client) { if (chk->checkState() == Qt::Checked) client->deleteLater(); }); });
    mMenu->addAction("disconnect unselected", [=] { enumClients([](QCheckBox* chk, QTcpSocket* client) { if (chk->checkState() == Qt::Unchecked) client->deleteLater(); }); });
    mMenu->addAction("disconnect all", [=]() { enumClients([](QCheckBox*, QTcpSocket* client) { client->deleteLater(); }); });

    ui->list_client_addr->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    connect(ui->list_client_addr, &QListWidget::customContextMenuRequested, [&](const QPoint&) { mMenu->exec(QCursor::pos()); });

    //////////////////////////////////////////////////////////////////////

    // handle connection

    connect(mChannel, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

chanTCPServer::~chanTCPServer() {
    delete ui;
}

void chanTCPServer::open() {
    // 注：该方法无法独占端口，即 TCPServer 也可监视相同端口，不报端口已被占用
    // 测试：开启两个 TCPServer，然后开启两个 TCPClient 去连接，结果是只有先开的 TCPServer 有 TCPClient去连接，后开的没有。
    //      在关闭先开的 TCPServer 之后，后开的 TCPServer 才有连接。

    if (mChannel->listen(QHostAddress(ui->cmb_server_ip->currentText()), ui->spn_server_port->value())) {
        setWidgetsEnable(false, ui->cmb_server_ip, ui->spn_server_port, Q_NULLPTR);
        emit runStateChanged(true);
    } else {
        QMessageBox::warning(this, QLatin1String("ERROR"), QLatin1String("fail to listen port"));
    }
}

void chanTCPServer::close() {
    mChannel->close();
    enumClients([](QCheckBox*, QTcpSocket* client) { client->deleteLater(); });
    setWidgetsEnable(true, ui->cmb_server_ip, ui->spn_server_port, Q_NULLPTR);
    emit runStateChanged(false);
}

void chanTCPServer::write(const QByteArray& bytes) {
    enumClients([&](QCheckBox* chk, QTcpSocket* client) { if (chk->checkState() == Qt::Checked) client->write(bytes); });
}

void chanTCPServer::newConnection() {
    while (mChannel->hasPendingConnections()) {
        QTcpSocket* client = mChannel->nextPendingConnection();

        // max connections

        int limit = ui->spn_client_limit->text().toInt();
        if (limit != -1 &&
            limit <= ui->list_client_addr->count()) {
            client->deleteLater();
            return;  // 已达最大连接数
        }

        QVariant client_socket;
        client_socket.setValue<QTcpSocket*>(client);

        QString client_addr = QLatin1String("%1:%2")
                                  .arg(client->peerAddress().toString())  // ip
                                  .arg(client->peerPort());               // port

        // new item

        QListWidgetItem* item = new QListWidgetItem(ui->list_client_addr);

        QCheckBox* chk = new QCheckBox(client_addr, this);
        chk->setCheckState(Qt::Checked);
        ui->list_client_addr->setItemWidget(item, chk);

        item->setData(Qt::UserRole, client_socket);  // associate QTcpSocket to item

        ui->list_client_addr->addItem(item);

        // count of connections

        ui->lbl_client_num->setNum(ui->list_client_addr->count());

        // 以下必须使用值捕获，不可使用引用捕获，因为退出此作用域时, 变量会被释放, 再使用会导致程序崩溃

        // read data

        connect(client, &QTcpSocket::readyRead, [this, client, chk]() {
            while (client->bytesAvailable()) {
                if (chk->checkState() == Qt::Checked) {
                    emit readyReadLine(client->readLine());
                } else {
                    client->readAll();  // 丢掉
                }
            }
        });

        // detect disconnection (掉线检测)

        connect(client, &QTcpSocket::disconnected, [this, client, chk, item]() {
            client->deleteLater();
            delete chk;
            delete item;
            ui->lbl_client_num->setNum(ui->list_client_addr->count());
        });
    }
}

void chanTCPServer::enumClients(std::function<void(QCheckBox* checkbox)> callback) {
    for (int32_t i = 0; i < ui->list_client_addr->count(); ++i) {
        QListWidgetItem* item = ui->list_client_addr->item(i);
        callback(static_cast<QCheckBox*>(ui->list_client_addr->itemWidget(item)));
    }
}

void chanTCPServer::enumClients(std::function<void(QCheckBox* checkbox, QTcpSocket* client)> callback) {
    for (int32_t i = 0; i < ui->list_client_addr->count(); ++i) {
        QListWidgetItem* item = ui->list_client_addr->item(i);
        callback(static_cast<QCheckBox*>(ui->list_client_addr->itemWidget(item)),
                 item->data(Qt::UserRole).value<QTcpSocket*>());
    }
}

bool chanTCPServer::eventFilter(QObject* watched, QEvent* event) {
    // left doubleclick label and then copy ip / port to clipboard

    if (event->type() == QEvent::MouseButtonDblClick) {
        if (watched == ui->lbl_server_ip) {
            QApplication::clipboard()->setText(ui->cmb_server_ip->currentText());
        } else if (watched == ui->lbl_server_port) {
            QApplication::clipboard()->setText(ui->spn_server_port->text());
        }
    }

    return uAbstractDataChannel::eventFilter(watched, event);
}
