#include "qmenuplus.h"
#include <QDebug>

QAction *cfgAct(QAction *act, bool checkable, bool checked)
{
    act->setCheckable(checkable);
    act->setChecked(checked);
    return act;
}
void setWdgEnable(bool enable, QWidget *widget,...)
{
    va_list ap; // arg_ptr
    va_start(ap, widget);
    do{
        widget->setEnabled(enable);
    }while((widget = va_arg(ap,QWidget*))!=Q_NULLPTR);
    va_end(ap);
}

void setWdgVisible(bool visible, QWidget *widget,...)
{
    va_list ap; // arg_ptr
    va_start(ap, widget);
    do{
        widget->setVisible(visible);
    }while((widget = va_arg(ap,QWidget*))!=Q_NULLPTR);
    va_end(ap);
}

