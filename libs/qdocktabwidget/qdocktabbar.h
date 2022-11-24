#ifndef QDOCKTABBAR_H
#define QDOCKTABBAR_H

#include <QTabBar>

#include <QApplication>
#include <QMouseEvent>

class QDockTabBar : public QTabBar {
    Q_OBJECT
public:
    explicit QDockTabBar(QWidget* parent = nullptr);

signals:
    void dragOut(int index);

private:
    bool m_isDraggingTab = false;

public:
    QRect boundary(int dx = 100, int dy = 60) { return contentsRect().adjusted(-dx, -dy, dx, dy); }  // 🖱 最小拖拽距离

protected:
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
};

#endif  // QDOCKTABBAR_H
