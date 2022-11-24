#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    parser = new uCommandParser();

    connect(ui->tab_interface, &datatransmitter::readyReadData, [&](const QByteArray& bytes) {
        ui->page_transfer->input_recv->append(bytes);
    });
    connect(ui->tab_interface, &datatransmitter::readyReadLine, [=](const QByteArray& bytes) {
        QByteArray cmd;
        getMiddleBytes(bytes,cmd,QByteArray("#{"),QByteArray("}"));
        parser->parse(cmd);

    });

    // Device Manager
    ui->menu_sys->addAction("设备管理器",[](){QDesktopServices::openUrl(QUrl("file:///C:/Windows/System32/devmgmt.msc", QUrl::TolerantMode));});
    ui->menu_sys->addAction("控制面板",[](){QProcess::startDetached("\"C:\\Windows\\System32\\control.exe\"");});
    ui->menu_sys->addAction("注册表编辑",[](){QDesktopServices::openUrl(QUrl("file:///C:/Windows/System32/regedt32.exe", QUrl::TolerantMode));});
    ui->menu_sys->addAction("服务",[](){QDesktopServices::openUrl(QUrl("file:///C:/Windows/System32/services.msc", QUrl::TolerantMode));});
    ui->menu_sys->addAction("系统信息",[](){QDesktopServices::openUrl(QUrl("file:///C:/Windows/System32/msinfo32.exe", QUrl::TolerantMode));});
    ui->menu_sys->addAction("蓝牙传输",[](){QProcess::startDetached("\"C:\\Windows\\System32\\fsquirt.exe\"");});
    ui->menu_sys->addAction("计算器",[](){QProcess::startDetached("\"C:\\Windows\\System32\\calc.exe\"");});

//    ui->splitter_1->setSizes(QList<int>() << 1 << 3 );  // 隐藏收发区
    ui->splitter_1->setStretchFactor(0,1);
    ui->splitter_1->setStretchFactor(1,0);

    ui->splitter_2->setStretchFactor(0,0);
    ui->splitter_2->setStretchFactor(1,1);

    registerCommandGroup();
    testCommandGroup();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::registerCommandGroup()
{

    parser->bind("PLOT.GRAPH.CFG",[=](const QByteArray& funcName, const QMap<QByteArray /*parmName*/, QVector<QByteArray> /*parmValues*/>& params){
        QMap<QByteArray, double> kvPairs;
        Q_FOREACH(auto name,params.keys()){
            ui->plot->cmdConfigGraph(name,params.value(name).first().toInt());
        }
        // convertBytesToDoubleVector(params,kvPairs);
    });


    parser->bind("PLOT.VALUE.ADD",[=](const QByteArray& funcName, const QMap<QByteArray /*parmName*/, QVector<QByteArray> /*parmValues*/>& params){
        QMap<QByteArray, double> kvPairs;
        Q_FOREACH(auto name,params.keys()){
            QVector<double> values;
            convertBytesToDoubleVector(params.value(name).first(),values);
            ui->plot->cmdAppendValues(name,values);
        }
    });



}

#include "datagraph.h"

void MainWindow::testCommandGroup()
{

    qDebug()<< ( datagraph::generate_config_axis(true,0,datagraph::atTop,0,datagraph::atLeft,1)<<1)+1;
    parser->parse(" PLOT.GRAPH.CFG @A 2099 @B 2099 @C 2323 ");
    parser->parse(" PLOT.VALUE.ADD @A 1,2,3 @B 3,2,1 @C 5,4,1,5 ");

    parser->parse(" PLOT.GRAPH.CFG @D 2595 ");
    parser->parse(" PLOT.VALUE.ADD @D 1,4,1,4,1 ");
}



//    ui->tab_interface->senddata(ui->input_send->toPlainText().toLocal8Bit());


