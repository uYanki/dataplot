#include "uabstractdatatransmitter.h"

uAbstractDataTransmitter::uAbstractDataTransmitter(QWidget* parent) : QWidget(parent), mTmrSpeedCalculator(new QTimer()) {
    clear();
    connect(mTmrSpeedCalculator, &QTimer::timeout, [&]() {  // @ kBps
        mSpeedOfRecv     = QString("%1 kB/s").arg(QString::number((mBytesOfRecv - mLastBytesOfRecv) / 1024.0f, 'f', 2));
        mSpeedOfSend     = QString("%1 kB/s").arg(QString::number((mBytesOfSend - mLastBytesOfSend) / 1024.0f, 'f', 2));
        mLastBytesOfRecv = mBytesOfRecv;
        mLastBytesOfSend = mBytesOfSend;
        emit speedometer();
    });

}

uAbstractDataTransmitter::~uAbstractDataTransmitter() {}

int8_t uAbstractDataTransmitter::bind(uAbstractDataChannel* channel) {
    if (mChannels.contains(channel)) return -1;
    mChannels.append(channel);
    connect(channel, &uAbstractDataChannel::readyReadLine, this, &uAbstractDataTransmitter::processLine);
    connect(channel, &uAbstractDataChannel::runStateChanged, [&](bool state) {
        emit runStateChanged( mRunState = state);
        if (state) {
            mRunningID = mTempID;
            mTmrSpeedCalculator->start(1000);  // 1s
        } else {
            mRunningID = -1;
            mBuffer.clear();
            mTmrSpeedCalculator->stop();
            mSpeedOfSend = mSpeedOfRecv = QLatin1String("0.00 kB/s");
            emit speedometer();
        };
    });

    return mChannels.count() - 1;
}

void uAbstractDataTransmitter::processLine(const QByteArray& bytes) {
    mBuffer += bytes;
    mBytesOfSend += bytes.count();
    emit bytecounter();
    emit readyReadData(bytes);
    if (bytes.endsWith('\n')) {
        ++mLinesOfRecv;
        emit linecounter();
        emit readyReadLine(mBuffer);
        mBuffer.clear();
    }
}

void uAbstractDataTransmitter::clear() {
    mLastBytesOfRecv = mLastBytesOfSend = 0;
    mBytesOfRecv = mLinesOfRecv = mBytesOfSend = 0;
    mSpeedOfRecv = mSpeedOfSend = QLatin1String("0.00 kB/s");
    emit bytecounter();
    emit linecounter();
    emit speedometer();
}
