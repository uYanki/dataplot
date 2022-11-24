#include "datagraph.h"

void datagraph::configAxis(QCPGraph* graph, int cfg)  // 坐标轴配置
{
    // 源码改动1, qcustomplot.c -> QCustomPlot.axisRect() : return rectList.at(qBound(0, index, rectList.size()-1));
    // 源码改动2, qcustomplot.c -> QCPAxisRect.axis() : return ax.at(qBound(0, index, ax.size()-1)););

    if (BIT_GET(cfg, bConfigAxis)) {
        QCPAxisRect* axrc = m_plot->axisRect(BIT_GETN(cfg, axisRectIndex, 2));  // 使能标志位
        if (BIT_GET(cfg, keyAxisType) != BIT_GET(cfg, valueAxisType)) {         /* 确保1个是水平轴, 1个是垂直轴 */
                                                                                //    qDebug() << BIT_GET(cfg, bConfigAxis) // 1
                                                                                //             << BIT_GETN(cfg, axisRectIndex, 2) // 0
                                                                                //             << BIT_GETN(cfg, keyAxis, 2) // 2
                                                                                //             << BIT_GET(cfg, keyAxisIndex) // 0
                                                                                //             << BIT_GETN(cfg, valueAxis, 2) // 2
                                                                                //             << BIT_GET(cfg, valueAxisIndex); // 1
            graph->setKeyAxis(axrc->axis(QCPAxis::AxisType(1 << BIT_GETN(cfg, keyAxis, 2)), BIT_GET(cfg, keyAxisIndex)));
            graph->setValueAxis(axrc->axis(QCPAxis::AxisType(1 << BIT_GETN(cfg, valueAxis, 2)), BIT_GET(cfg, valueAxisIndex)));
        }
    }
}

int datagraph::generate_config(bool bEnableOrg, int orgAxisCfg, bool bClearBeforeAdding, bool bEnableFFT, int fftAxisCfg)
{
    int ret = 0;
    if(bEnableOrg){
        ret = BIT_SET(ret, graph_config::bEnableOrg);
        ret = BIT_SETV(ret, graph_config::axisOrgBegin,orgAxisCfg);
        if(bClearBeforeAdding) ret = BIT_SET(ret, graph_config::bClearBeforeAdding);
    }
    if(bEnableFFT){
        ret = BIT_SET(ret, graph_config::bEnableFFT);
        ret = BIT_SETV(ret, graph_config::axisFFTBegin,fftAxisCfg);
    }
    return ret;
}


int datagraph::generate_config_axis(bool bConfigAxis, char axisRectIndex, axis_pos keyAxis, char keyAxisIndex, axis_pos valueAxis, char valueAxisIndex) {
    int ret = 0;
    if (bConfigAxis) {
        ret = BIT_SET(ret, config_axis::bConfigAxis);
        ret = BIT_SETV(ret, config_axis::axisRectIndex, axisRectIndex);
        ret = BIT_SETV(ret, config_axis::keyAxis, keyAxis);
        if (keyAxisIndex) ret = BIT_SET(ret, config_axis::keyAxisIndex);
        ret = BIT_SETV(ret, config_axis::valueAxis, valueAxis);
        if (valueAxisIndex) ret = BIT_SET(ret, config_axis::valueAxisIndex);
    }
    return ret;
}

void datagraph::config(int cfg) {
    configOrg(BIT_GET(cfg, graph_config::bEnableOrg), BIT_GETN(cfg, graph_config::axisOrgBegin, (graph_config::axisOrgEnd - graph_config::axisOrgBegin)));
    configFFT(BIT_GET(cfg, graph_config::bEnableFFT), BIT_GETN(cfg, graph_config::axisFFTBegin, (graph_config::axisFFTEnd - graph_config::axisFFTBegin)));
    m_bClearBeforeAdding = BIT_GET(cfg, graph_config::bClearBeforeAdding);
}

void datagraph::configOrg(bool enable, int cfg_axis) {
    if (enable) {
        if (graph_org == Q_NULLPTR)
            (graph_org = m_plot->addGraph())->setName(m_name_org);
        configAxis(graph_org, cfg_axis);
    } else if (graph_org != Q_NULLPTR) {
        m_plot->removeGraph(graph_org);
        graph_org = Q_NULLPTR;
    }
}

void datagraph::configFFT(bool enable, int cfg_axis) {
    if (enable) {
        if (graph_fft == Q_NULLPTR)
            (graph_fft = m_plot->addGraph())->setName(m_name_fft);
        configAxis(graph_fft, cfg_axis);
    } else if (graph_fft != Q_NULLPTR) {
        m_plot->removeGraph(graph_fft);
        graph_fft = Q_NULLPTR;
    }
}

void datagraph::addValues(const QVector<double>& values) {
//    qDebug() << values;
    if (graph_org != Q_NULLPTR) {
        if (m_bClearBeforeAdding) graph_org->data().clear();
        for (int i = 0, j = graph_org->dataCount(); i < values.size(); ++i, ++j)
            graph_org->data()->add(QCPGraphData(j, values[i]));
    }
    if (graph_fft != Q_NULLPTR) {
        graph_fft->data()->clear();
        QVector<double> values_fft;
//        qDebug() << "size" << values.count();
        fft4(values,values_fft,int(log2(values.count())));
        for (int i = 0; i < values_fft.size(); ++i)
            graph_fft->data()->add(QCPGraphData(i, values_fft[i]));
    }
}
