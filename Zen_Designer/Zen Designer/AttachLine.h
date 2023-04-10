#ifndef LINELABEL_H
#define LINELABEL_H

#include <QLabel>
#include <QPainter>
#include <QtMath>

#include "Utils.h"

class AttachLine : public QLabel
{
    Q_OBJECT
public:
    explicit AttachLine(QWidget *parent = nullptr);

    void setLine(const QPoint& start, const QPoint& end);

    void setLineColor(const QColor& color);
    void setLineStyle(Qt::PenStyle style);
    void setLineWidth(int width);

protected:
    void paintEvent(QPaintEvent* ev) override;

private:
    enum Orientation {
        Slash,
        Backslash
    };
    Orientation orient;

    QColor lineColor;
    Qt::PenStyle lineStyle;
    int lineWidth;

signals:

};

#endif // LINELABEL_H
