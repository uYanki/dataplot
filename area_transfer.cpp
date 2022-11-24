#include "area_transfer.h"
#include "ui_area_transfer.h"

area_transfer::area_transfer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::area_transfer),
    m_MenuOfSend(new QMenu(this)),
    m_MenuOfRecv(new QMenu(this))
{
    ui->setupUi(this);

    // 接收区选项卡

    input_recv = new Input(this);
    input_recv->setReadOnly(true);
    input_recv->setPlaceholderText("waiting for data to arrive");

    ui->tabwdg_recv->add(input_recv, "Recv", true);

    auto menu = input_recv->menu;

    QActionGroup* actGroup = new QActionGroup(menu);
    actGroup->setExclusive(true); // 排他性
    QToolBar* toolbar = new QToolBar(menu);
    addWdgAct(menu, newLytWdg<QHBoxLayout>(menu,new QLabel("Format:"),toolbar));
    toolbar->addAction(cfgAct(actGroup->addAction("DEC"),true,true));
    toolbar->addAction(cfgAct(actGroup->addAction("HEX"),true,false));

    menu->addAction("Excute CMD",[=](){});
    menu->addSeparator();
    menu->addAction("Copy All",[=](){QApplication::clipboard()->setText(input_recv->toPlainText());});
    menu->addAction("Save as TXT",[=](){input_recv->saveAsTXT();});
    menu->addAction("Save as CSV",[=](){input_recv->saveAsCSV();});
    menu->addSeparator();
    menu->addAction("Clear",[=](){input_recv->clear();});

    connect(ui->tabwdg_recv, &QDockTabWidget::doubleClickBlank, [&]() {  // 子过滤器
        bool    ok;
        QString name = QInputDialog::getText(this, "New Tab", "Name of sub receiving area:", QLineEdit::Normal, "", &ok);
        if (ok && !name.isEmpty()) {
            auto filter = new page_subrecv(ui->tabwdg_recv);
            // m_SubFilters.append(filter);
            ui->tabwdg_recv->add(filter, name);
            connect(filter, &page_subrecv::destroyed, [=]() {/* m_SubFilters.removeOne(filter); */ });
        }
    });



    ui->input_send->setPlaceholderText("enter something you want to send");






//    connect(ui->tabwidget_recv, &dock_tabwidget::dblBlank, [&]() {  // 子过滤器
//        bool    ok;
//        QString name = QInputDialog::getText(this, "Tab", "new tab name:", QLineEdit::Normal, "", &ok);
//        if (ok && !name.isEmpty()) {
//            auto filter = new subfilter(ui->tabwidget_recv);
//            m_SubFilters.append(filter);
//            ui->tabwidget_recv->add(filter, name);
//            connect(filter, &subfilter::destroyed, [=]() { m_SubFilters.removeOne(filter); });
//        }
//    });

//    // 接收区菜单

//    input_recv->bindMenu(m_MenuOfRecv);
//    m_MenuOfRecv->addAction(QLatin1String("raw data"), [&](bool b) { m_RawDataMd = b; })->setCheckable(true);
//    m_MenuOfRecv->addAction(QLatin1String("timestamp"), [&](bool b) { m_TimestampMd = b; })->setCheckable(true);
//    m_MenuOfRecv->addSeparator();
//    m_MenuOfRecv->addAction(QLatin1String("save to txt"), [&]() { m_input_recv->saveToTXT(); });
//    m_MenuOfRecv->addAction(QLatin1String("save to csv"), [&]() { m_input_recv->saveToCSV(); });
//    m_MenuOfRecv->addSeparator();
//    m_MenuOfRecv->addAction(QLatin1String("clear"), [&]() { m_input_recv->clear(); });

//    // 发送区菜单

//    connect(ui->input_send, &QPlainTextEdit::customContextMenuRequested, [&]() { m_MenuOfSend->exec(QCursor::pos()); });

//    m_MenuOfSend->addAction(QLatin1String("clear"), [&]() { ui->input_send->clear(); });


}

area_transfer::~area_transfer()
{
    delete ui;
}
