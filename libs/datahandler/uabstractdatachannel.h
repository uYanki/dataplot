#ifndef UABSTRACTDATACHANNELH
#define UABSTRACTDATACHANNELH

/**************** 数据传输通道 ****************/

#include <QDebug>
#include <QWidget>

class uAbstractDataChannel : public QWidget {
    Q_OBJECT

public:
    uAbstractDataChannel(QWidget* parent = Q_NULLPTR) : QWidget(parent) {
        connect(this,&uAbstractDataChannel::runStateChanged,[&](bool state){mRunState = state;});
    }
    ~uAbstractDataChannel() {}

    virtual void open()                         = 0;  // sucess -> emit runStateChanged(true);
    virtual void close()                        = 0;  // sucess -> emit runStateChanged(false);
    virtual void write(const QByteArray& bytes) = 0;

signals:
    void readyReadLine(const QByteArray& bytes);
    void runStateChanged(bool state = false);

protected:
    bool mRunState = false;
};

#endif  // UABSTRACTDATACHANNELH
