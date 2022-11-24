#ifndef page_datafilter_H
#define page_datafilter_H

#include <QMenu>
#include <QWidget>

#include "treeitem_datafilter.h"

namespace Ui {
    class page_datafilter;
}

class page_datafilter : public QWidget {
    Q_OBJECT

public:
    explicit page_datafilter(QWidget* parent = nullptr);
    ~page_datafilter();

private:
    Ui::page_datafilter* ui;

private:
    QMenu* const m_Menu = new QMenu(this);

    QList<treeitem_datafilter*> m_filters;

    QString m_prefix;
    QString m_suffix;
    QString m_delimiter;

private:
    bool filter(const QByteArray& bytes);
};

#endif  // page_datafilter_H
