#ifndef OBJECTDESIGNERWIDGET_H
#define OBJECTDESIGNERWIDGET_H

#include <QPainter>
#include <QFileDialog>
#include <QFile>
#include <QDataStream>

#include "DesignerWidget.h"
#include "BezierWidget.h"
#include "ControlPoint.h"
#include "ListScrollArea.h"
#include "PointPropertyBoard.h"
#include "ObjectViewWidget.h"
#include "MaterialBoard.h"
#include "LightBoard.h"

class ObjectDesignerWidget : public DesignerWidget
{
    Q_OBJECT
public:
    explicit ObjectDesignerWidget(QWidget *parent = nullptr);

    virtual Mode mode() override;

protected:
    virtual void paintEvent(QPaintEvent* ev) override;

private:
    unsigned long pointCounter;

    // left sidebar
    QLabel* pointListTitle;
    MenuButton* pointAddButton;
    ListScrollArea* pointList;
    MenuButton* textureEditButton;

    MenuButton* pointEditButton;
    MenuButton* materialButton;
    MenuButton* lightButton;

    // mid widget
    BezierWidget* bezierWidget;
    ObjectViewWidget* objectWidget;

    // property boards
    PointPropertyBoard* pointBoard;
    MaterialBoard* materialBoard;
    LightBoard* lightBoard;

    bool isPointAdding;

    void addPoint(const QPoint& pos); // pos in bezierWidget
    void addPoint(const QString& name, ControlPoint::Type type, const QPoint& pos, const QPoint& offset, bool relative = true);

    void setPointEdit();
    void setTextureEdit();

    void hideAllBoards();

    void save();
    void load();

signals:

};

#endif // OBJECTDESIGNERWIDGET_H
