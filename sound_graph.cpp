#include "sound_graph.h"

SoundGraph::SoundGraph() :
    m_axisX(new QValueAxis()),
    m_axisY(new QValueAxis()),
    m_step(0)
{
    resize(400, 300);
    chart()->setTitle("Sound Graphic");
    chart()->legend()->hide();
    chart()->setAnimationOptions(QChart::AllAnimations);

    m_series[0] = new QSplineSeries(this);
    m_series[1] = new QSplineSeries(this);
    m_series[2] = new QSplineSeries(this);

    QPen green(Qt::green);
    green.setWidth(3);
    m_series[0]->setPen(green);

    QPen red(Qt::red);
    red.setWidth(3);
    m_series[1]->setPen(red);

    QPen blue(Qt::blue);
    blue.setWidth(3);
    m_series[2]->setPen(blue);

    chart()->addSeries(m_series[0]);
    chart()->addSeries(m_series[1]);
    chart()->addSeries(m_series[2]);

    chart()->addAxis(m_axisX,Qt::AlignBottom);
    chart()->addAxis(m_axisY,Qt::AlignLeft);

    m_series[0]->attachAxis(m_axisX);
    m_series[0]->attachAxis(m_axisY);

    m_series[1]->attachAxis(m_axisX);
    m_series[1]->attachAxis(m_axisY);

    m_series[2]->attachAxis(m_axisX);
    m_series[2]->attachAxis(m_axisY);

    m_axisX->setTickCount(5);
    m_axisX->setRange(0, 10);
    m_axisY->setRange(0, 10);

    m_coordX = new QGraphicsSimpleTextItem(chart());
    m_coordX->setPos(chart()->size().width()/2+20, chart()->size().height());
    m_coordX->setText("X: ");
}

void SoundGraph::DrawSound(quint8 u8_detector, qreal d_time, quint16 u16_sound)
{
    if (0 == m_axisX->min() && (1 < d_time))
    {
        m_axisX->setMin(d_time);
        m_axisX->setMax(d_time);
    }

    if (d_time > m_axisX->max())
    {
        m_axisX->setMax(d_time);
    }

    if ((qreal)u16_sound > m_axisY->max())
    {
        m_axisY->setMax((qreal)u16_sound);
    }

    m_series[u8_detector]->append(d_time, (qreal)u16_sound);
}

void SoundGraph::clear()
{
    m_series[0]->clear();
    m_series[1]->clear();
    m_series[2]->clear();

    m_axisX->setMin(0);
    m_axisX->setMax(0);
    m_axisX->setRange(0, 10);
}

void SoundGraph::mouseMoveEvent(QMouseEvent *event)
{
    double X = chart()->mapToValue(event->pos()).x();
    m_coordX->setText(QString("X: %1").arg(X, 0, 'f', 5));
    QGraphicsView::mouseMoveEvent(event);
}

