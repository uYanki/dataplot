#ifndef DATAPLOT_H
#define DATAPLOT_H

#include <QMap>

#include "bits_tool.h"
#include "qcustomplot.h"

#include "qmenuplus.h"

class datagraph;  // 声明类存在，在 dataplot.cpp 进行包含

class dataplot : public QCustomPlot {
public:
    dataplot(QWidget* parent = nullptr);

    QMap<QByteArray, datagraph*> m_datagraphs;

    void cmdConfigGraph(QByteArray name ,int flags);
    void cmdAppendValues(QByteArray name, QVector<double> values);
    void cmdRemoveGraphs(QVector<QByteArray>& keys);
    void refresh() { replot(QCustomPlot::RefreshPriority::rpQueuedReplot); }

private:
    QMenu* m_Menu = nullptr;
    void   initRightClickMenu();

    void initLegendAction();

    QCPAxisRect* isMouseInAxisRect(const QPoint& pos);

    // 拖拽缩放坐标轴

    void initAxisRect();
    void initAxisAction();
    void setAxisZoomFactor(double factor = 0.85 /* 缩放因子：值越小缩放越快 */);

    // 曲线值提示

    QCPItemText*         m_GraphValuesLabel;               // 曲线值标签
    QCPItemStraightLine *m_MouseHorLine, *m_MouseVerLine;  // 十字指示线 crosshair
    void                 initValuesTip();
    bool                 updateTipArea(const QPoint& pos /* mouse pos */);

    // 右键框选缩放
    QRubberBand* m_SelectedArea = nullptr;  // 掩膜
    void         initSelectedAreaMask(void);
    bool         updateSelectArea(uint8_t e, QMouseEvent* event);

protected:
    // 重载鼠标事件
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    // 重载键盘事件
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
};

#endif  // DATAPLOT_H
