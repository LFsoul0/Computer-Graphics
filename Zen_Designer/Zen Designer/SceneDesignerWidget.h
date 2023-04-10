#ifndef SCENEDESIGNERWIDGET_H
#define SCENEDESIGNERWIDGET_H

#include <QFileDialog>

#include "DesignerWidget.h"
#include "ListScrollArea.h"
#include "SceneViewWidget.h"
#include "EnvironmentBoard.h"
#include "ObjectPropertyBoard.h"

class SceneDesignerWidget : public DesignerWidget
{
    Q_OBJECT
public:
    explicit SceneDesignerWidget(QWidget *parent = nullptr);

    virtual Mode mode() override;

private:
    // left sidebar
    QLabel* objectListTitle;
    MenuButton* objectAddButton;
    ListScrollArea* objectList;

    MenuButton* envButton;

    // mid widget
    SceneViewWidget* sceneWidget;

    // property boards
    ObjectPropertyBoard* objectBoard;
    EnvironmentBoard* envBoard;

    void addObject();
    void connectObject(SceneObject* obj);

    void hideAllBoards();

    void save();
    void load();

signals:

};

#endif // SCENEDESIGNERWIDGET_H
