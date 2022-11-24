#include "setwidgetsprop.h"

void setWidgetsEnable(bool enable, QWidget *widget,...)
{
    va_list ap; // arg_ptr
    va_start(ap, widget);
    do{
        widget->setEnabled(enable);
    }while((widget = va_arg(ap,QWidget*))!=Q_NULLPTR);
    va_end(ap);
}

void setWidgetsVisible(bool visible, QWidget *widget,...)
{
    va_list ap; // arg_ptr
    va_start(ap, widget);
    do{
        widget->setVisible(visible);
    }while((widget = va_arg(ap,QWidget*))!=Q_NULLPTR);
    va_end(ap);
}

