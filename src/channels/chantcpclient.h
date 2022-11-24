#ifndef CHANTCPCLIENT_H
#define CHANTCPCLIENT_H

#include <QClipboard>
#include <QMessageBox>
#include <QTcpSocket>
#include <QTimer>
#include <QWidget>

#include "getlocalip.h"
#include "setwidgetsprop.h"
#include "uabstractdatachannel.h"

namespace Ui {
    class chanTCPClient;
}

class chanTCPClient : public uAbstractDataChannel {
    Q_OBJECT

public:
    explicit chanTCPClient(QWidget* parent = nullptr);
    ~chanTCPClient();

    void open() Q_DECL_OVERRIDE;
    void close() Q_DECL_OVERRIDE;
    void write(const QByteArray& bytes) Q_DECL_OVERRIDE;

private:
    bool eventFilter(QObject* watched, QEvent* event) Q_DECL_OVERRIDE;

private:
    Ui::chanTCPClient* ui;
    QTcpSocket* const  mChannel;
    QTimer* const      mTmrReconnect;            // 自动重连
    bool               bAutoAssignPort = false;  // 自动分配端口( port=0 时启用 )
};

#endif  // CHANTCPCLIENT_H
