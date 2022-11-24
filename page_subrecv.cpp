#include "page_subrecv.h"
#include "ui_page_subrecv.h"

#include <QDebug>

#include "qmenuplus.h"

page_subrecv::page_subrecv(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page_subrecv)
{
    ui->setupUi(this);

    ui->input_recv->setPlaceholderText("waiting for data to arrive");

    QMenu* menu = ui->input_recv->menu;

    m_actSubFilter = menu->addAction("Sub Filter",[=](bool checked){
        setWdgVisible(checked,ui->cmb_filter,ui->input_filter, Q_NULLPTR);
    });
    m_actSubFilter->setCheckable(true);
    m_actSubFilter->setChecked(true);

//    m_actTimeStamp

    menu->addSeparator();
    menu->addAction("Copy All",[=](){QApplication::clipboard()->setText(ui->input_recv->toPlainText());});
    menu->addAction("Save as TXT",[=](){ui->input_recv->saveAsTXT();});
    menu->addAction("Save as CSV",[=](){ui->input_recv->saveAsCSV();});
    menu->addSeparator();
    menu->addAction("Clear",[=](){ui->input_recv->clear();});

}

page_subrecv::~page_subrecv()
{
   qDebug()<<"hhh";
   delete ui;
}

bool page_subrecv::isFilterEnable(int& mode,QByteArray& bytes)
{
    if(m_actSubFilter->isChecked()){
        const QString& data =  ui->input_recv->toPlainText();
        if(!data.isEmpty()){
            mode = ui->cmb_filter->currentIndex();
            bytes = data.toLocal8Bit();
            return true;
        }
    }
    return false;
}

bool isOK(const QByteArray &bytes){
    return true;
}

void page_subrecv::recvdata(const QByteArray &bytes)
{
    if(!m_actSubFilter->isChecked() || isOK(bytes))
        ui->input_recv->append(bytes);
}
