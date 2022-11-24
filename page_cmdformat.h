#ifndef PAGE_CMDFORMAT_H
#define PAGE_CMDFORMAT_H

#include <QWidget>

namespace Ui {
class page_cmdformat;
}

class page_cmdformat : public QWidget
{
    Q_OBJECT

public:
    explicit page_cmdformat(QWidget *parent = nullptr);
    ~page_cmdformat();

private:
    Ui::page_cmdformat *ui;
};

#endif // PAGE_CMDFORMAT_H
