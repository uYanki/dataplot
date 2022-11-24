#include "qdocktabbar.h"

QDockTabBar::QDockTabBar(QWidget* parent) : QTabBar(parent) {
    setMovable(true);
    setTabsClosable(true);
}

void QDockTabBar::mousePressEvent(QMouseEvent* event) {
    m_isDraggingTab = (event->button() == Qt::LeftButton) && (currentIndex() >= 0);
    QTabBar::mousePressEvent(event);
}

void QDockTabBar::mouseMoveEvent(QMouseEvent* event) {
    if (m_isDraggingTab && event->buttons()) {
        if (!contentsRect().contains(event->pos())) {  // 判断是否脱离Tabbar

            // qdock_tabwidget 中, 接收信号 dragOut 的槽函数里 调用了 drag->exec(), 该函数导致 tabbar 无法触发 release 事件, 所以这里手动触发下
            QMouseEvent* e = new QMouseEvent(QEvent::MouseButtonRelease, event->pos(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
            QApplication::postEvent(this, e);

            emit dragOut(currentIndex());
        }
    }

    QTabBar::mouseMoveEvent(event);
}

void QDockTabBar::mouseReleaseEvent(QMouseEvent* event) {
    m_isDraggingTab = false;
    QTabBar::mouseReleaseEvent(event);
}
