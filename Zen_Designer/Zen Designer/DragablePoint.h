#ifndef DRAGABLEPOINT_H
#define DRAGABLEPOINT_H

#include <QLabel>
#include <QMouseEvent>
#include <QtMath>

class DragablePoint : public QLabel
{
    Q_OBJECT
public:
    explicit DragablePoint(QWidget *parent = nullptr);
    explicit DragablePoint(bool autoDrag, QWidget *parent = nullptr);

    void setRangeRect(const QRect& r);

protected:
    virtual void mousePressEvent(QMouseEvent* ev) override;
    virtual void mouseMoveEvent(QMouseEvent* ev) override;
    virtual void mouseReleaseEvent(QMouseEvent* ev) override;

private:
    bool isDragging;
    QRect range;

signals:
    void pressed();
    void drag(QPoint offset);
};

#endif // DRAGABLEPOINT_H
