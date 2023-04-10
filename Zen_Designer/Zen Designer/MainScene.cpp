#include "MainScene.h"

MainScene::MainScene(QWidget *parent) : QWidget(parent)
{
    // set window size
    setFixedSize(parent->size());

    // create designer widgets
    objectWidget = new ObjectDesignerWidget(this);
    connect(objectWidget, &DesignerWidget::back, this, &MainScene::back);
    connect(objectWidget, &DesignerWidget::switchMode, this, &MainScene::switchMode);
    sceneWidget = new SceneDesignerWidget(this);
    connect(sceneWidget, &DesignerWidget::back, this, &MainScene::back);
    connect(sceneWidget, &DesignerWidget::switchMode, this, &MainScene::switchMode);
    sceneWidget->hide();
}

void MainScene::switchMode(DesignerWidget::Mode mode, DesignerWidget* current)
{
    current->hide();

    switch (mode) {
    case DesignerWidget::ObjectMode: {
        objectWidget->show();
        break;
    }
    case DesignerWidget::SceneMode: {
        sceneWidget->show();
        break;
    }
    }

    emit switchScene();
}
