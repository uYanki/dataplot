#include "datatransmitter.h"

datatransmitter::datatransmitter(QWidget* parent) : uAbstractDataTransmitter(parent),
                                                    ui(new Ui::datatransmitter) {
    ui->setupUi(this);

    connect(ui->cmb_channel, SIGNAL(currentIndexChanged(int)), ui->stack_channel, SLOT(setCurrentIndex(int)));
    connect(ui->btn_run, &QPushButton::clicked, [=]() { runState() ? close() : open(ui->stack_channel->currentIndex()); });

    // Transmission Control Protocol
    addChannel<chanSerialPort>("Serial Port");
    addChannel<chanTCPClient>("TCP Client");
    addChannel<chanTCPServer>("TCP Server");
    // addChannel<chanUdpClient>("UDP Client");
    // addChannel<chanUdpServer>("UDP Server");
    addChannel<chanBLClient>("BL Client");
    // addChannel<chanBLServer>("BL Server (doing)");
    // addChannel<chanMQTT>("MQTT");

    connect(this, &uAbstractDataTransmitter::runStateChanged, [=](bool state) { ui->btn_run->setChecked(state); });
}

datatransmitter::~datatransmitter() {
    delete ui;
}
