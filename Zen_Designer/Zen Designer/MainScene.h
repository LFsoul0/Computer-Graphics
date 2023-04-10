#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QWidget>

#include "ObjectDesignerWidget.h"
#include "SceneDesignerWidget.h"

class MainScene : public QWidget
{
    Q_OBJECT
public:
    explicit MainScene(QWidget *parent = nullptr);

private:
    ObjectDesignerWidget* objectWidget;
    SceneDesignerWidget* sceneWidget;

    void switchMode(DesignerWidget::Mode mode, DesignerWidget* current);

signals:
    void back();
    void switchScene();

};

#endif // MAINSCENE_H
