#ifndef BEZIERWIDGET_H
#define BEZIERWIDGET_H

#include <QWidget>
#include <QVector>
#include <QVector3D>
#include <QPainter>
#include <QPainterPath>
#include <QtMath>
#include <QDataStream>

#include "ControlPoint.h"

class BezierWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BezierWidget(QWidget *parent = nullptr);
    explicit BezierWidget(int axisX, int axisY, QWidget *parent = nullptr);

    void addPoint(ControlPoint* p);
    void removePoint(ControlPoint* p);

    QVector<QVector3D> getBezierLine();

    virtual void update();

    void save(QDataStream& ds);
    void clear();

protected:
    virtual void paintEvent(QPaintEvent* ev) override;
    virtual void mousePressEvent(QMouseEvent* ev) override;

private:
    static constexpr unsigned int sampleNum = 200;

    const int axisX; // y of x-axis
    const int axisY; // x of y-axis
    QVector<ControlPoint *> points;

signals:
    void pressed(const QPoint& pos);
};

#endif // BEZIERWIDGET_H
