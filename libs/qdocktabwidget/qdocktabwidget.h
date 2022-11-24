#ifndef QDOCKTABWDG_H
#define QDOCKTABWDG_H

#include <QDrag>
#include <QMimeData>
#include <QPainter>
#include <QPixmap>

#include <QTabWidget>

#include "qdockpopup.h"
#include "qdocktabbar.h"

class QDockTabWidget : public QTabWidget {
    Q_OBJECT
public:
    explicit QDockTabWidget(QWidget* parent = nullptr);

private:
    QDockTabBar* const m_tabbar;
    QList<QWidget*>    m_FixedTabs;

public:
    int add(QWidget* widget, const QString& title, bool fixed = false);

signals:
    void doubleClickBlank(void);  // 双击tabbtn右边的空白处

private slots:
    void dargTab(int index);

protected:
    bool event(QEvent*) override;
};

#endif  // QDOCKTABWDG_H
