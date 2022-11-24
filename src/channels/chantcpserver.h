#ifndef CHANTCPSERVER_H
#define CHANTCPSERVER_H

#include <QCheckBox>
#include <QListWidgetItem>
#include <QMenu>
#include <QWidget>

#include <QClipboard>
#include <QMessageBox>

#include <QTcpServer>
#include <QTcpSocket>

#include "getlocalip.h"
#include "setwidgetsprop.h"
#include "uabstractdatachannel.h"

namespace Ui {
    class chanTCPServer;
}

class chanTCPServer : public uAbstractDataChannel {
    Q_OBJECT
private:
    Ui::chanTCPServer* ui;

public:
    explicit chanTCPServer(QWidget* parent = nullptr);
    ~chanTCPServer();

private:
    bool eventFilter(QObject* watched, QEvent* event) Q_DECL_OVERRIDE;

public:
    void open() Q_DECL_OVERRIDE;
    void close() Q_DECL_OVERRIDE;
    void write(const QByteArray& bytes) Q_DECL_OVERRIDE;

private slots:
    void newConnection();

private:
    void enumClients(std::function<void(QCheckBox* checkbox)> callback);
    void enumClients(std::function<void(QCheckBox* checkbox, QTcpSocket* socket)> callback);

    QTcpServer* const mChannel;
    QMenu*            mMenu = Q_NULLPTR;
};

#endif  // CHANTCPSERVER_H
