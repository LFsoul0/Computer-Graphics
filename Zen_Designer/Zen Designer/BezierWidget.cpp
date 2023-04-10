#include "BezierWidget.h"

BezierWidget::BezierWidget(QWidget *parent) : BezierWidget(0, 0, parent) { }

BezierWidget::BezierWidget(int axisX, int axisY, QWidget *parent) : QWidget(parent), axisX(axisX), axisY(axisY)
{

}

void BezierWidget::addPoint(ControlPoint* p)
{
    p->setDragRange(axisY, 0, width() - axisY, axisX + 1);
    p->setOriginPoint(axisY, axisX);
    points.append(p);
    connect(p, &ControlPoint::changeType, this, &BezierWidget::update);
    connect(p, &ControlPoint::changePos, this, &BezierWidget::update);
    connect(p, &ControlPoint::changeOffset, this, &BezierWidget::update);
    connect(p, &ControlPoint::destroyed, this, [=]() { removePoint(p); });

    update();
}

void BezierWidget::removePoint(ControlPoint *p)
{
    points.removeAll(p);
    p->disconnect(this);

    update();
}

void BezierWidget::update()
{
    QWidget::update();
}

void BezierWidget::paintEvent(QPaintEvent* ev)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // draw axis
    QPen pen;
    pen.setColor(Qt::darkCyan);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(3);
    painter.setPen(pen);
    painter.drawLine(axisY, 0, axisY, height());    // draw y-axis
    painter.drawLine(0, axisX, width(), axisX);     // draw x-axis

    // draw bezier
    pen.setColor(Qt::white);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(5);
    painter.setPen(pen);
    if (!points.isEmpty()) {
        QPainterPath path(points[0]->getPos(false));
        int num = points.size();
        for (int i = 0; i < num - 1; ++i) {
            switch (points[i]->getType()) {
            case ControlPoint::StraightLine:
                path.lineTo(points[i + 1]->getPos(false));
                break;
            case ControlPoint::Curve:
                path.cubicTo(points[i]->getForwardPoint(false), points[i + 1]->getBackwardPoint(false), points[i + 1]->getPos(false));
                break;
            }
        }
        painter.drawPath(path);
    }

    QWidget::paintEvent(ev);
}

void BezierWidget::mousePressEvent(QMouseEvent* ev)
{
    emit pressed(ev->pos());
    QWidget::mousePressEvent(ev);
}

QVector<QVector3D> BezierWidget::getBezierLine()
{
    QVector<QVector3D> line;

    int num = points.size();
    if (num > 1) {
        const double step = (double)(num - 1) / (sampleNum - 1);

        int current = 0;
        double t = 0;
        QVector3D p0 = QVector3D(points[0]->getPos());
        QVector3D p1 = QVector3D(points[0]->getForwardPoint());
        QVector3D p2 = QVector3D(points[1]->getBackwardPoint());
        QVector3D p3 = QVector3D(points[1]->getPos());
        for (unsigned int i = 0; i < sampleNum; ++i) {
            switch (points[current]->getType()) {
            case ControlPoint::StraightLine:
                line.append((1 - t) * p0 + t * p3);
                break;
            case ControlPoint::Curve:
                line.append(qPow(1 - t, 3) * p0 + 3 * t * qPow(1 - t, 2) * p1 + 3 * qPow(t, 2) * (1 - t) * p2 + qPow(t, 3) * p3);
                break;
            }

            t += step;
            if (t > 1 && current < num - 2) {
                t -= 1;
                current++;
                p0 = QVector3D(points[current]->getPos());
                p1 = QVector3D(points[current]->getForwardPoint());
                p2 = QVector3D(points[current + 1]->getBackwardPoint());
                p3 = QVector3D(points[current + 1]->getPos());
            }
        }
    }
    else {
        line = QVector<QVector3D>(sampleNum, QVector3D(0, 0, 0));
    }

    return line;
}

void BezierWidget::save(QDataStream& ds)
{
    ds << points.size();
    for (ControlPoint* p : points) {
        ds << p->getName();
        ds << p->getType();
        ds << p->getPos();
        ds << p->getOffset();
    }
}

void BezierWidget::clear()
{
    for (ControlPoint* p : points) {
        p->deleteLater();
    }
}
