#ifndef DATAGRAPH_H
#define DATAGRAPH_H

#include "dataplot.h"
#include "fft4.h"

class datagraph {
private:
    dataplot* m_plot;

    const QString m_name_org;
    QCPGraph*     graph_org = Q_NULLPTR;

    const QString m_name_fft;
    QCPGraph*     graph_fft = Q_NULLPTR;

    void configOrg(bool enable, int cfg_axis);
    void configFFT(bool enable, int cfg_axis);
    void configAxis(QCPGraph* graph, int cfg);

public:
    enum graph_config {
        // graph_org
        bEnableOrg,
        axisOrgBegin,
        axisOrgEnd = axisOrgBegin + 9,
        bClearBeforeAdding,
        // graph_fft
        bEnableFFT,
        axisFFTBegin,
        axisFFTEnd = axisFFTBegin + 9,
    };
    Q_ENUMS(graph_config)

    enum axis_pos {
        atLeft,
        atRight,
        atTop,
        atBottom
    };
    Q_ENUMS(axis_pos)

    enum config_axis /* count = 9 */ {
        bConfigAxis,                  // 1
        axisRectIndex,                // 2
        keyAxis = axisRectIndex + 2,  // 2
        keyAxisType,
        keyAxisIndex = keyAxis + 2,  // 1
        valueAxis,                   // 2
        valueAxisType,
        valueAxisIndex = valueAxis + 2,  // 1
    };
    Q_ENUMS(config_axis);

    // 0b 1000100011

    static int generate_config(
            bool bEnableOrg,
            int orgAxisCfg,
            bool bClearBeforeAdding,
            bool bEnableFFT,
            int fftAxisCfg);

    static int generate_config_axis(
        bool     bConfigAxis,
        char     axisRectIndex,
        axis_pos keyAxis,
        char     keyAxisIndex,
        axis_pos valueAxis,
        char     valueAxisIndex);

    datagraph(dataplot* plot, const QString& name)
        : m_plot(plot),
          m_name_org(QString("[org] %1").arg(name)),
          m_name_fft(QString("[fft] %1").arg(name)) {}
    ~datagraph() {}

    void config(int cfg);

    void addValues(const QVector<double>& values);

    bool m_bClearBeforeAdding = false;  // only for graph_org
};

#endif  // DATAGRAPH_H
