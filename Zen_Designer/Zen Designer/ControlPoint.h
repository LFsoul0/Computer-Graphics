#ifndef CONTROLPOINT_H
#define CONTROLPOINT_H

#include <QObject>
#include <QLabel>
#include <QMouseEvent>

#include "DragablePoint.h"
#include "AttachLine.h"

class ControlPoint : public QObject
{
    Q_OBJECT
public:
    enum Type {
        Curve,
        StraightLine
    };

    explicit ControlPoint(QWidget *parent = nullptr);
    explicit ControlPoint(const QString& name, QWidget *parent = nullptr);
    virtual ~ControlPoint();

    void setOriginPoint(int x, int y);
    void setOriginPoint(const QPoint& p);

    void setDragRange(int left, int top, int width, int height);
    void setDragRange(const QRect& r);

    QString getName();
    void setName(const QString& name);

    Type getType();
    void setType(Type t);

    QPoint getPos(bool relative = true);
    void move(int x, int y, bool relative = true);
    void move(const QPoint& p, bool relative = true);

    QPoint getOffset(bool relative = true);
    void setOffset(int x, int y, bool relative = true);
    void setOffset(const QPoint& p, bool relative = true);

    QPoint getForwardPoint(bool relative = true);
    QPoint getBackwardPoint(bool relative = true);

    void setFocus(bool focus);
    void focusIn();
    void focusOut();

private:
    static const QSize centralPointSize;
    static const QSize attachPointSize;

    bool focus;
    Type type;

    QString name;
    QPoint pos;     // relative to parent
    QPoint offset;
    QPoint origin;  // for coodinate trans

    QPixmap normalPix;
    QPixmap selectedPix;
    QPixmap attachPix;

    DragablePoint* centralPoint;
    AttachLine* attachLine;
    DragablePoint* forwardPoint;
    DragablePoint* backwardPoint;

    void updateAllPos();

signals:
    void changeName(const QString& name);
    void changeType(Type type);
    void changePos(const QPoint& pos);
    void dragPos(const QPoint& pos);
    void changeOffset(const QPoint& offset);
    void dragOffset(const QPoint& offset);
    void changeFocus(bool focus);
};

#endif // CONTROLPOINT_H
