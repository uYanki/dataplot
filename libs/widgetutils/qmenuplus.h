#ifndef QMENUPLUS_H
#define QMENUPLUS_H

#include <QWidget>

#include <QLabel>
#include <QToolBar>

#include <QHBoxLayout>
#include <QAction>
#include <QWidgetAction>
#include <QMenu>

// Horizontal & Vertical layout

template<typename T> QWidget* newLytWdg(QWidget* parent, QWidget* left, QWidget* right, int margin = 6){
    static_assert(std::is_base_of<QBoxLayout,T>::value,"the var isn't base of QBoxLayout");
    QWidget* wdg = new QWidget(parent);
    T* lyt = new T(wdg); // QHBoxLayout / QVBoxLayout
    left->setParent(wdg);
    right->setParent(wdg);
    lyt->addWidget(left);
    lyt->addWidget(right);
    lyt->setMargin(margin);
    return wdg;
}

// Config QAction

template<typename T> T* addWdgAct(QMenu* menu, T* widget){
    static_assert(std::is_base_of<QWidget,T>::value,"the var isn't base of QWidget");
    QWidgetAction* wdgAct = new QWidgetAction(menu);
    widget->setParent(menu);
    wdgAct->setDefaultWidget(widget);
    menu->addAction(wdgAct);
    return widget;
}

QAction *cfgAct(QAction* act,  bool checkable, bool checked = false);

// config QWidget Params

void setWdgEnable(bool enable,QWidget* widget,...);
void setWdgVisible(bool visible,QWidget* widget,...);
// usage: setWdgVisible(checked, ui->label,ui->combobox, Q_NULLPTR);

#endif // QMENUPLUS_H
