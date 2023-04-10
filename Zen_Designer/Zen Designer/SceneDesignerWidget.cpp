#include "SceneDesignerWidget.h"

SceneDesignerWidget::SceneDesignerWidget(QWidget *parent) : DesignerWidget(parent)
{
    // left sidebar
    modeTitle->setText("Scene Mode");
    connect(modeButton, &MenuButton::clicked, this, [=]() { emit switchMode(ObjectMode, this); });
    connect(saveButton, &MenuButton::clicked, this, &SceneDesignerWidget::save);
    connect(loadButton, &MenuButton::clicked, this, &SceneDesignerWidget::load);

    objectListTitle = new QLabel("Objects", leftSidebar);
    objectListTitle->setFixedSize(150, 80);
    objectListTitle->move(20, 80);
    objectListTitle->setFont(Utils::menuFont());
    objectListTitle->setStyleSheet("color: white");

    objectAddButton = new MenuButton("+", leftSidebar);
    objectAddButton->setFixedSize(80, 80);
    objectAddButton->move(leftSidebar->width() - objectAddButton->width(), 80);
    objectAddButton->setTriggeredFontSizeByRatio(1.2);
    objectAddButton->setNormalColor(Qt::white);
    objectAddButton->setPressedColor(Qt::cyan);
    connect(objectAddButton, &MenuButton::clicked, this, &SceneDesignerWidget::addObject);

    objectList = new ListScrollArea(leftSidebar);
    objectList->move((leftSidebar->width() - objectList->width()) / 2, 160);

    envButton = new MenuButton("Environment", leftSidebar);
    envButton->move((leftSidebar->width() - envButton->width()) / 2, height() - 280);
    envButton->setTriggeredFontSizeByRatio(1.2);
    envButton->setNormalColor(Qt::yellow);
    envButton->setPressedColor(Qt::cyan);
    connect(envButton, &MenuButton::clicked, this, [=]() {
        hideAllBoards();
        envBoard->show();
    });

    // scene widget
    sceneWidget = new SceneViewWidget(this);
    sceneWidget->setFixedSize(width() - leftSidebar->width(), height());
    sceneWidget->move(leftSidebar->width(), 0);
    connect(sceneWidget, &SceneViewWidget::focusIn, this, &SceneDesignerWidget::hideAllBoards);
    connect(sceneWidget, &SceneViewWidget::loadObject, this, &SceneDesignerWidget::connectObject);

    // property boards
    objectBoard = new ObjectPropertyBoard(this);
    objectBoard->move(width() - objectBoard->width(), 0);
    objectBoard->hide();

    envBoard = new EnvironmentBoard(this);
    envBoard->move(width() - envBoard->width(), 0);
    envBoard->setSceneWidget(sceneWidget);
    envBoard->hide();
}

DesignerWidget::Mode SceneDesignerWidget::mode()
{
    return SceneMode;
}

void SceneDesignerWidget::addObject()
{
    // check dir
    QDir dir;
    if (!dir.exists("./object")) {
        dir.mkdir("./object");
    }

    // get path
    const QString path = QFileDialog::getOpenFileName(this, "Load Object", "./object", "Object File (*.obj)");
    if (path == "") return;

    // open file
    QFile file(path);
    file.open(QIODevice::ReadWrite);
    QDataStream data(&file);

    SceneObject* obj = sceneWidget->addObject(data);
    obj->setFilePath(path);

    connectObject(obj);
}

void SceneDesignerWidget::connectObject(SceneObject* obj)
{
    MenuButton* button = new MenuButton(obj->getName());
    button->setFixedSize(objectList->width() - 50, 45);
    button->setNormalFontSize(32);
    button->setTriggeredFontSizeByRatio(1.0);
    button->setCheckable(true);
    button->setAutoRaise(true);
    button->setAlignment(Qt::AlignLeft);
    button->setNormalColor(Qt::white);
    button->setPressedColor(Qt::cyan);
    connect(obj, &SceneObject::changeName, button, &MenuButton::setText);
    connect(obj, &SceneObject::changeFocus, button, &MenuButton::setCheck);
    connect(button, &MenuButton::checked, obj, &SceneObject::setFocus);
    connect(obj, &SceneObject::destroyed, button, &MenuButton::deleteLater);
    objectList->addWidget(button);

    connect(obj, &SceneObject::changeFocus, this, [=](bool focus) {
        if (focus) {
            hideAllBoards();
            objectBoard->bind(obj);
        }
        else {
            objectBoard->unbind(obj);
        }
    });

    obj->focusIn();
}

void SceneDesignerWidget::hideAllBoards()
{
    objectBoard->hide();
    envBoard->hide();
}

void SceneDesignerWidget::save()
{
    // check dir
    QDir dir;
    if (!dir.exists("./scene")) {
        dir.mkdir("./scene");
    }

    // get path
    const QString path = QFileDialog::getSaveFileName(this, "Save Scene", "./scene", "Scene File (*.scene)");
    if (path == "") return;

    // open file
    QFile file(path);
    file.open(QIODevice::ReadWrite | QIODevice::Truncate);
    QDataStream data(&file);

    // save
    sceneWidget->save(data);
}

void SceneDesignerWidget::load()
{
    // check dir
    QDir dir;
    if (!dir.exists("./scene")) {
        dir.mkdir("./scene");
    }

    // get path
    const QString path = QFileDialog::getOpenFileName(this, "Load Scene", "./scene", "Scene File (*.scene)");
    if (path == "") return;

    // open file
    QFile file(path);
    file.open(QIODevice::ReadWrite);
    QDataStream data(&file);

    // load
    hideAllBoards();
    sceneWidget->load(data);

    envBoard->setSceneWidget(sceneWidget);
}
