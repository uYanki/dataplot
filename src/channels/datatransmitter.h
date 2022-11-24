#ifndef datatransmitter_H
#define datatransmitter_H

#include <QWidget>

#include "uabstractdatatransmitter.h"

#include "chanblclient.h"
#include "chanserialport.h"
#include "chantcpclient.h"
#include "chantcpserver.h"

#include "ui_datatransmitter.h"

namespace Ui {
    class datatransmitter;
}

class datatransmitter : public uAbstractDataTransmitter {
    Q_OBJECT
private:
    Ui::datatransmitter* ui;

public:
    explicit datatransmitter(QWidget* parent = nullptr);
    ~datatransmitter();

public:
    template <typename T, typename = typename std::enable_if<std::is_base_of<uAbstractDataChannel, T>::value> /* 限制模板类型 */>
    void addChannel(QString name) {
        // todo: 加个互斥锁🔒(静态变量)以防重复添加？ QMutex QMutexLocker
        ui->cmb_channel->addItem(name);
        T* chan = new T();
        ui->stack_channel->addWidget(chan);
        bind(chan);
        // qDebug("%p,%p",ui->stack_channel,chan->parent());
    }
};

#endif  // datatransmitter_H
