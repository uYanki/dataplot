#include "dataplot.h"

#include <QDebug>

#include "datagraph.h"

dataplot::dataplot(QWidget* parent) : QCustomPlot(parent) {
    initLegendAction();
    initAxisAction();
    initAxisRect();
    initSelectedAreaMask();
    initRightClickMenu();
    initValuesTip();
}

void dataplot::cmdConfigGraph(QByteArray name ,int flags) {

    datagraph* graph = m_datagraphs.value(name);
    if (graph == Q_NULLPTR) {
        graph = new datagraph(this, QString::fromLocal8Bit(name));
        std::string s= name.toStdString();
        m_datagraphs.insert(QByteArray(s.data(),s.length()), graph);
    }
    graph->config(flags);

    refresh();
}

void dataplot::cmdAppendValues(QByteArray name, QVector<double> values) {

//    qDebug() << m_datagraphs;


        if (m_datagraphs.contains(name)) {
//            qDebug() << "add values" << key;
            datagraph* graph = m_datagraphs.value(name);
            graph->addValues(values);
//            qDebug() << key << kvPairs.value(key);
        }else{
//            qDebug() << "can't find" << key;
        }

    refresh();
}

void dataplot::cmdRemoveGraphs(QVector<QByteArray>& keys) {
    Q_FOREACH (QCPGraph* graph, mGraphs) {
        const QByteArray& key = graph->name().toLocal8Bit();
        if (keys.contains(key)) removeGraph(graph);
    }
    refresh();
}

void dataplot::initRightClickMenu() {
    // 右键菜单
    m_Menu = new QMenu(this);
    setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

    xAxis->rescale();
    m_Menu->addAction("rescale x", [=]() {
        xAxis->rescale();
        xAxis2->rescale();
        refresh();
    });
    m_Menu->addAction("rescale y", [=]() {
        yAxis->rescale();
        yAxis2->rescale();
        refresh();
    });
    m_Menu->addAction("rescale all", [=]() {
        rescaleAxes(true);
        refresh();
    });
    m_Menu->addAction("clear", [&]() {
        m_datagraphs.clear();
        clearGraphs();
        refresh();
    });
    m_Menu->addAction("save");
}

void dataplot::initAxisAction() {
    setInteraction(QCP::Interaction::iSelectAxes, true);  // 坐标轴可选
    setInteraction(QCP::Interaction::iRangeDrag, true);   // 允许鼠标拖拽
    setInteraction(QCP::Interaction::iRangeZoom, true);   // 允许滚轮缩放

    connect(this, &QCustomPlot::selectionChangedByUser, [&]() {
        // when an axis is selected, only that direction can be dragged and zoomed
        auto testFlag = [&](QCPAxisRect* axrc, QCPAxis::AxisTypes types) -> int {
            Q_FOREACH (auto axis, axrc->axes(types)) {
                if (axis->selectedParts().testFlag(QCPAxis::spAxis) ||
                    axis->selectedParts().testFlag(QCPAxis::spTickLabels)) {
                    // 轴和刻度同步选中
                    axis->setSelectedParts(QCPAxis::spAxis | QCPAxis::spTickLabels);
                    return axis->axisType();
                }
            }
            return 0;
        };
        Q_FOREACH (auto axrc, axisRects()) {
            // 沿指定方向拖拽缩放
            if (testFlag(axrc, QCPAxis::atLeft) | testFlag(axrc, QCPAxis::atRight)) {
                // testFlag((axrc,QCPAxis::atLeft | QCPAxis::atRight)
                axrc->setRangeZoom(Qt::Vertical);
                axrc->setRangeDrag(Qt::Vertical);
            } else if (testFlag(axrc, QCPAxis::atTop) | testFlag(axrc, QCPAxis::atBottom)) {
                // else if (testFlag(QCPAxis::atTop | QCPAxis::atBottom))
                axrc->setRangeZoom(Qt::Horizontal);
                axrc->setRangeDrag(Qt::Horizontal);
            } else {
                axrc->setRangeDrag(Qt::Horizontal | Qt::Vertical);
                axrc->setRangeZoom(Qt::Horizontal | Qt::Vertical);
            }
        }
    });
}

void dataplot::setAxisZoomFactor(double factor) {
    Q_FOREACH (auto axrc, axisRects())
        axrc->setRangeZoomFactor(factor);
}

void dataplot::initLegendAction() {
    legend->setVisible(true);  // 图例可视

    setInteraction(QCP::Interaction::iSelectLegend, true);  // 图列可选
    legend->setSelectableParts(QCPLegend::spItems);         // 允许选中的部分

    // 右键点击图例 -> 弹出菜单

    connect(this, &QCustomPlot::legendClick, [=](QCPLegend*, QCPAbstractLegendItem* item, QMouseEvent* event) {
        if (item && event->type() == QEvent::MouseButtonRelease) {
            QCPPlottableLegendItem* pLegendItem = qobject_cast<QCPPlottableLegendItem*>(item);        // 图列
            QCPGraph*               pPlottable  = qobject_cast<QCPGraph*>(pLegendItem->plottable());  // 曲线

            deselectAll();
            pLegendItem->setSelected(true);  // 选中图例
            // pPlottable->setSelection(QCPDataSelection(pPlottable->data()->dataRange())); // 选中曲线
            refresh();

            switch (event->button()) {
                case Qt::LeftButton: {  // 缩放至曲线区域
                    bool b;
                    auto keyRange = pPlottable->data()->keyRange(b);
                    pPlottable->keyAxis()->setRange(keyRange.lower, keyRange.upper);
                    auto valueRange = pPlottable->data()->valueRange(b);
                    pPlottable->valueAxis()->setRange(valueRange.lower, valueRange.upper);
                    break;
                }
                case Qt::RightButton: {  // 右键菜单

                    QMenu* menu = new QMenu(this);

                    // 隐藏或显示曲线

                    QAction* actionSetGraphVisible = menu->addAction("Visible", [=](bool checked) {
                        pPlottable->setVisible(checked);
                        pLegendItem->setTextColor(QColor(checked ? Qt::black : Qt::gray));
                        refresh();
                    });
                    actionSetGraphVisible->setCheckable(true);
                    actionSetGraphVisible->setChecked(pPlottable->visible());

                    void (QComboBox::*slotCurrentIndexChanged)(int index) = &QComboBox::currentIndexChanged;

                    // 线条样式
                    QComboBox* lsComboBox = new QComboBox(menu);
                    addWdgAct(menu, newLytWdg<QHBoxLayout>(menu, new QLabel("Line:"), lsComboBox));
                    lsComboBox->addItems(QStringList() << "None"
                                                       << "Line"
                                                       << "StepLeft"
                                                       << "StepCenter"
                                                       << "StepRight"
                                                       << "Impulse");
                    lsComboBox->setCurrentIndex(int(pPlottable->lineStyle()));
                    connect(lsComboBox, slotCurrentIndexChanged, [=](int index) {
                        pPlottable->setLineStyle((QCPGraph::LineStyle)index);
                        refresh();
                    });

                    // 转折点样式
                    QComboBox* ssComboBox = new QComboBox(menu);
                    addWdgAct(menu, newLytWdg<QHBoxLayout>(menu, new QLabel("Scatter:"), ssComboBox));
                    ssComboBox->addItems(QStringList() << "None"
                                                       << "Dot"
                                                       << "Cross"
                                                       << "Plus"
                                                       << "Circle"
                                                       << "Disc"
                                                       << "Square"
                                                       << "Diamond"
                                                       << "Star"
                                                       << "Triangle"
                                                       << "TriangleInverted"
                                                       << "CrossSquare"
                                                       << "PlusSquare"
                                                       << "CrossCircle"
                                                       << "PlusCircle"
                                                       << "Peace");
                    ssComboBox->setCurrentIndex(int(pPlottable->scatterStyle().shape()));
                    connect(ssComboBox, slotCurrentIndexChanged, [=](int index) {
                        pPlottable->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)index));
                        refresh();
                    });

                    menu->addSeparator();

                    // 保存数据点

                    menu->addAction("Save");
                    menu->addAction("Remove", [=]() {
                        removeGraph(pPlottable);
                        replot();
                    });

                    menu->exec(QCursor::pos());  // 弹出菜单

                    delete menu;  // 释放菜单
                    break;
                }
                default: break;
            }
        }
    });
}

void dataplot::initValuesTip() {
    // 鼠标十字线

    m_MouseHorLine = new QCPItemStraightLine(this);
    m_MouseHorLine->setSelectable(false);
    m_MouseHorLine->setPen(QPen(Qt::GlobalColor::blue));

    m_MouseVerLine = new QCPItemStraightLine(this);
    m_MouseVerLine->setSelectable(false);
    m_MouseVerLine->setPen(QPen(Qt::GlobalColor::blue));

    // layer( default,低->顶 ):  background -> grid -> main -> axes -> legend -> overlay
    // 自定义的 "crosshair" 层需在 "main" 层的下方, 否则十字线会导致 "main" 层及以下层无法响应部分鼠标事件
    addLayer(("crosshair"), layer("legend"), QCustomPlot::LayerInsertMode::limBelow);  // 插入新层
    m_MouseHorLine->setLayer("crosshair");
    m_MouseVerLine->setLayer("crosshair");

    m_GraphValuesLabel = new QCPItemText(this);
    m_GraphValuesLabel->setPadding(QMargins(6, 6, 6, 6));
    m_GraphValuesLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_GraphValuesLabel->setBrush(QBrush(QColor(0, 0, 255, 100)));  // crBackground
    m_GraphValuesLabel->setColor(Qt::GlobalColor::white);          // crText
    m_GraphValuesLabel->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_GraphValuesLabel->setLayer("overlay");

    updateTipArea(mapFromGlobal(QCursor::pos()));
    // connect(this, &QCustomPlot::beforeReplot, [&]() {});
}

bool dataplot::updateTipArea(const QPoint& pos) {
    auto axrc = isMouseInAxisRect(pos);
    if (axrc != nullptr) {
        QList<QCPGraph*> graphs = axrc->graphs();
        if (graphs.count() > 0) {
            float x = xAxis->pixelToCoord(pos.x());
            float y = yAxis->pixelToCoord(pos.y());
            // 十字线
            m_MouseHorLine->point1->setCoords(x, y);
            m_MouseHorLine->point2->setCoords(x + 1, y);
            m_MouseVerLine->point1->setCoords(x, y);
            m_MouseVerLine->point2->setCoords(x, y + 1);
            // 曲线值
            QString   tip;
            const int offset = 20;
            Q_FOREACH (auto graph, graphs) {
                bool b;
                int i = graph->keyAxis()->pixelToCoord(
                            (graph->keyAxis()->axisType() == QCPAxis::atTop ||
                             graph->keyAxis()->axisType() == QCPAxis::atBottom) ?
                                pos.x() : pos.y()
                            ); // 按方向选取轴
                if (graph->visible() &&                          // 可视
                    (graph->getKeyRange(b).contains(i - 0.3) ||  // 鼠标在索引范围内/附近
                     graph->getKeyRange(b).contains(i + 0.3))) {
                    tip.append(QString("%1%2:(%3,%4)")
                               .arg(tip.isEmpty()? "":"\r\n")
                               .arg(graph->name())
                               .arg(i)
                               .arg(graph->data()->at(i)->value));
                }
            }
            if (!tip.isEmpty()) {
                m_GraphValuesLabel->setText(tip);
                m_GraphValuesLabel->position->setCoords(
                    xAxis->pixelToCoord(pos.x() + offset),
                    yAxis->pixelToCoord(pos.y() + offset));
                // 置可视
                m_MouseHorLine->setVisible(true);
                m_MouseVerLine->setVisible(true);
                m_GraphValuesLabel->setVisible(true);
                refresh();
                return true;
            }
        }
    }
    if (m_GraphValuesLabel->visible()) {  // 减少刷新次数
        m_MouseHorLine->setVisible(false);
        m_MouseVerLine->setVisible(false);
        m_GraphValuesLabel->setVisible(false);
        refresh();
    }
    return false;
}

QCPAxisRect* dataplot::isMouseInAxisRect(const QPoint& pos) {
    if (!legend->outerRect().contains(pos))
        Q_FOREACH (auto axrc, axisRects())
            if (axrc->rect().contains(pos))
                return axrc;
    return nullptr;
}

void dataplot::initAxisRect() {
    Q_FOREACH (auto axrc, axisRects()) {
        axrc->setupFullAxesBox();
        axrc->addAxis(QCPAxis::atLeft);                   // 双y轴
        axrc->axis(QCPAxis::atRight)->setSubTicks(true);  // 设置刻度可视
        axrc->axis(QCPAxis::atTop)->setSubTicks(true);
        axrc->setRangeDragAxes(axrc->axes());                                // 配置可拖动轴
        axrc->setRangeZoomAxes(axrc->axes());                                // 配置可缩放轴
        axrc->axis(QCPAxis::atLeft)->grid()->setZeroLinePen(QPen(Qt::red));  // 坐标轴颜色
        axrc->axis(QCPAxis::atBottom)->grid()->setZeroLinePen(QPen(Qt::red));
    }
}

void dataplot::initSelectedAreaMask() {
    m_SelectedArea = new QRubberBand(QRubberBand::Rectangle, this);
    m_SelectedArea->setBackgroundRole(QPalette::Light);
}

bool dataplot::updateSelectArea(uint8_t e, QMouseEvent* event) {
    static QPoint ptStart;  // 框选起始点
    if (e == QEvent::MouseButtonPress) {
        /* mousePressEvent */
        ptStart = event->pos();  // 记录框选的起始点
        m_SelectedArea->setGeometry(QRect(ptStart, QSize()));
        m_SelectedArea->show();
        return true;
    } else if (m_SelectedArea->isVisible() && e == QEvent::MouseMove) {
        /* mouseMoveEvent */
        // 框选区域可视化
        m_SelectedArea->setGeometry(QRect(
            QPoint(qMin(ptStart.x(), event->pos().x()), qMin(ptStart.y(), event->pos().y())),
            QPoint(qMax(ptStart.x(), event->pos().x()), qMax(ptStart.y(), event->pos().y()))));
        return true;
    } else if (m_SelectedArea->isVisible() && e == QEvent::MouseButtonRelease) {
        /* mouseReleaseEvent */
        m_SelectedArea->hide();
        const QRect rc = m_SelectedArea->geometry();
        if (rc.width() > 5 && rc.height() > 5) {
            // 将图表的显示区域设置为框选区域
            xAxis->setRange(xAxis->pixelToCoord(rc.left()), xAxis->pixelToCoord(rc.right()));
            yAxis->setRange(yAxis->pixelToCoord(rc.top()), yAxis->pixelToCoord(rc.bottom()));
            refresh();
            return true;
        }
    }
    return false;
}

void dataplot::mouseMoveEvent(QMouseEvent* event) {
    updateSelectArea(QEvent::MouseMove, event);
    updateTipArea(event->pos());
    QCustomPlot::mouseMoveEvent(event);
}

void dataplot::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::RightButton)
        updateSelectArea(QEvent::MouseButtonPress, event);
    QCustomPlot::mousePressEvent(event);
}

void dataplot::mouseReleaseEvent(QMouseEvent* event) {
    if (!updateSelectArea(QEvent::MouseButtonRelease, event) &&
        event->button() == Qt::RightButton &&
        isMouseInAxisRect(event->pos()) != nullptr) {
        m_Menu->exec(QCursor::pos());  // 弹出右键菜单
    }
    QCustomPlot::mouseReleaseEvent(event);
}

void dataplot::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Control: setAxisZoomFactor(0.3); break;
        default: break;
    }
    QCustomPlot::keyPressEvent(event);
}

void dataplot::keyReleaseEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Control: setAxisZoomFactor(0.85); break;
        default: break;
    }
    QCustomPlot::keyPressEvent(event);
}
