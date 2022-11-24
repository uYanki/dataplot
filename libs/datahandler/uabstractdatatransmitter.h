#ifndef UABSTRACTDATATRANSMITTER_H
#define UABSTRACTDATATRANSMITTER_H

/**************** 数据中转站 ****************/

#include <QTimer>
#include <QWidget>

#include "uabstractdatachannel.h"

class uAbstractDataTransmitter : public QWidget {
    Q_OBJECT

public:
    uAbstractDataTransmitter(QWidget* parent = Q_NULLPTR);
    ~uAbstractDataTransmitter();

    /////////////////////////////

signals:
    void runStateChanged(bool state);
    void readyReadData(const QByteArray& bytes);
    void readyReadLine(const QByteArray& bytes);

public:
    int8_t bind(uAbstractDataChannel* channel);  // return id

    void open(int8_t id){ if (mRunningID == -1 && id > -1 && id < mChannels.count())  mChannels.at(mTempID = id)->open();}
    void close(){mChannels[mRunningID]->close();}
    void write(const QByteArray& bytes){ mChannels[mRunningID]->write(bytes);}

    bool runState(){return mRunState;}

private slots:
    // connect to uAbstractDataTransmitter::readyReadLine
    void processLine(const QByteArray& bytes);

private:
    bool mRunState = false;
    QVector<uAbstractDataChannel*> mChannels;
    int8_t                         mRunningID = -1, mTempID;

    QByteArray mBuffer;  // 行数据缓冲区

    /////////////////////////////

signals:
    void bytecounter();  // 字节计数器
    void linecounter();  // 行计数器
    void speedometer();  // 收发速度计

private:
    int mLastBytesOfRecv = 0, mLastBytesOfSend = 0;

public:
    int           mLinesOfRecv;
    int           mBytesOfRecv, mBytesOfSend;
    QString       mSpeedOfRecv, mSpeedOfSend;
    QTimer* const mTmrSpeedCalculator;

    void clear();
};

#endif  // UABSTRACTDATATRANSMITTER_H
