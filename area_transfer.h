#ifndef AREA_TRANSFER_H
#define AREA_TRANSFER_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QInputDialog>

#include "page_subrecv.h"
#include "input.h"
#include "qmenuplus.h"

namespace Ui {
class area_transfer;
}

class area_transfer : public QWidget
{
    Q_OBJECT

public:
    explicit area_transfer(QWidget *parent = nullptr);
    ~area_transfer();

    Input* input_recv;

private:
    Ui::area_transfer *ui;

    QMenu* const m_MenuOfSend;
    QMenu* const m_MenuOfRecv;


};

#endif // AREA_TRANSFER_H
