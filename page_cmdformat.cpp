#include "page_cmdformat.h"
#include "ui_page_cmdformat.h"

page_cmdformat::page_cmdformat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page_cmdformat)
{
    ui->setupUi(this);
}

page_cmdformat::~page_cmdformat()
{
    delete ui;
}
