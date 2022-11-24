#ifndef PAGE_SUBRECV_H
#define PAGE_SUBRECV_H

#include <QWidget>

namespace Ui {
class page_subrecv;
}

class page_subrecv : public QWidget
{
    Q_OBJECT

public:
    explicit page_subrecv(QWidget *parent = nullptr);
    ~page_subrecv();

    bool isFilterEnable(int& mode,QByteArray& bytes);

public slots:
    void recvdata(const QByteArray& bytes);

private:
    Ui::page_subrecv *ui;

    QAction* m_actSubFilter;
    QAction* m_actTimeStamp;
};

#endif // PAGE_SUBRECV_H
