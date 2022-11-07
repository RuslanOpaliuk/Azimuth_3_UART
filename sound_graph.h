#ifndef SOUNDGRAPH_H
#define SOUNDGRAPH_H

#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QStringList>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

class SoundGraph : public QChartView
{
public:
    SoundGraph();
    void DrawSound(quint8 u8_detector, qreal d_time, quint16 u16_sound);
    void clear();
private:
    QSplineSeries *m_series[3];
    QStringList m_titles;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
    qreal m_step;
    qreal m_x;
    qreal m_y;
    QGraphicsSimpleTextItem *m_coordX;

protected:
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // SOUNDGRAPH_H
