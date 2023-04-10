#include "ObjectDesignerWidget.h"

ObjectDesignerWidget::ObjectDesignerWidget(QWidget *parent) : DesignerWidget(parent)
{
    pointCounter = 0;

    // left sidebar
    modeTitle->setText("Object Mode");  // designer mode config
    connect(modeButton, &MenuButton::clicked, this, [=]() { emit switchMode(SceneMode, this); });
    connect(saveButton, &MenuButton::clicked, this, &ObjectDesignerWidget::save);
    connect(loadButton, &MenuButton::clicked, this, &ObjectDesignerWidget::load);

    textureEditButton = new MenuButton("Edit Textures", leftSidebar);
    textureEditButton->move((leftSidebar->width() - textureEditButton->width()) / 2, height() - 280);
    textureEditButton->setTriggeredFontSizeByRatio(1.2);
    textureEditButton->setNormalColor(Qt::white);
    textureEditButton->setPressedColor(Qt::cyan);
    connect(textureEditButton, &MenuButton::clicked, this, &ObjectDesignerWidget::setTextureEdit);

    pointListTitle = new QLabel("Points", leftSidebar);
    pointListTitle->setFixedSize(150, 80);
    pointListTitle->move(20, 100);
    pointListTitle->setFont(Utils::menuFont());
    pointListTitle->setStyleSheet("color: white");

    pointAddButton = new MenuButton("+", leftSidebar);
    pointAddButton->setFixedSize(80, 80);
    pointAddButton->move(leftSidebar->width() - pointAddButton->width(), 100);
    pointAddButton->setTriggeredFontSizeByRatio(1.2);
    pointAddButton->setCheckable(true);
    pointAddButton->setNormalColor(Qt::white);
    pointAddButton->setPressedColor(Qt::cyan);
    connect(pointAddButton, &MenuButton::checked, this, [=](bool isChecked) {
        isPointAdding = isChecked;
    });

    pointList = new ListScrollArea(leftSidebar);
    pointList->move((leftSidebar->width() - pointList->width()) / 2, 180);

    pointEditButton = new MenuButton("Edit Points", leftSidebar);
    pointEditButton->move((leftSidebar->width() - pointEditButton->width()) / 2, height() - 280);
    pointEditButton->setTriggeredFontSizeByRatio(1.2);
    pointEditButton->setNormalColor(Qt::white);
    pointEditButton->setPressedColor(Qt::cyan);
    connect(pointEditButton, &MenuButton::clicked, this, &ObjectDesignerWidget::setPointEdit);

    materialButton = new MenuButton("Material", leftSidebar);
    materialButton->move((leftSidebar->width() - materialButton->width()) / 2, 150);
    materialButton->setTriggeredFontSizeByRatio(1.2);
    materialButton->setNormalColor(Qt::yellow);
    materialButton->setPressedColor(Qt::cyan);
    connect(materialButton, &MenuButton::clicked, this, [=]() {
        hideAllBoards();
        materialBoard->show();
    });

    lightButton = new MenuButton("Light", leftSidebar);
    lightButton->move((leftSidebar->width() - lightButton->width()) / 2, 250);
    lightButton->setTriggeredFontSizeByRatio(1.2);
    lightButton->setNormalColor(Qt::yellow);
    lightButton->setPressedColor(Qt::cyan);
    connect(lightButton, &MenuButton::clicked, this, [=]() {
        hideAllBoards();
        lightBoard->show();
    });

    // bezier widget
    bezierWidget = new BezierWidget(height() - 100, 200, this);
    bezierWidget->setFixedSize(width() - leftSidebar->width(), height());
    bezierWidget->move(leftSidebar->width(), 0);
    connect(bezierWidget, &BezierWidget::pressed, this, [=](const QPoint& pos) {
        if (isPointAdding) {
            addPoint(pos);
            isPointAdding = false;
            pointAddButton->setCheck(false);
        }
    });

    // object widget
    objectWidget = new ObjectViewWidget(this);
    objectWidget->setFixedSize(width() - leftSidebar->width(), height());
    objectWidget->move(leftSidebar->width(), 0);

    // property boards
    pointBoard = new PointPropertyBoard(this);
    pointBoard->move(width() - pointBoard->width(), 0);
    pointBoard->hide();

    materialBoard = new MaterialBoard(this);
    materialBoard->move(width() - materialBoard->width(), 0);
    materialBoard->setObjectWidget(objectWidget);
    materialBoard->hide();

    lightBoard = new LightBoard(this);
    lightBoard->move(width() - lightBoard->width(), 0);
    lightBoard->setObjectWidget(objectWidget);
    lightBoard->hide();

    // state
    isPointAdding = false;

    setPointEdit();
}

DesignerWidget::Mode ObjectDesignerWidget::mode()
{
    return ObjectMode;
}

void ObjectDesignerWidget::addPoint(const QPoint& pos)
{
    QString name = QString("point %1").arg(pointCounter++);

    addPoint(name, ControlPoint::Curve, pos, QPoint(100, 0), false);
}

void ObjectDesignerWidget::addPoint(const QString& name, ControlPoint::Type type, const QPoint& pos, const QPoint& offset, bool relative)
{
    ControlPoint* point = new ControlPoint(name, bezierWidget);
    bezierWidget->addPoint(point);
    point->setType(type);
    point->move(pos, relative);
    point->setOffset(offset, relative);

    MenuButton* button = new MenuButton(name);
    button->setFixedSize(pointList->width() - 50, 45);
    button->setNormalFontSize(32);
    button->setTriggeredFontSizeByRatio(1.0);
    button->setCheckable(true);
    button->setAutoRaise(true);
    button->setAlignment(Qt::AlignLeft);
    button->setNormalColor(Qt::white);
    button->setPressedColor(Qt::cyan);
    connect(point, &ControlPoint::changeName, button, &MenuButton::setText);
    connect(point, &ControlPoint::changeFocus, button, &MenuButton::setCheck);
    connect(button, &MenuButton::checked, point, &ControlPoint::setFocus);
    connect(point, &ControlPoint::destroyed, button, &MenuButton::deleteLater);
    pointList->addWidget(button);

    connect(point, &ControlPoint::changeFocus, this, [=](bool focus) {
        if (focus) {
            hideAllBoards();
            pointBoard->bind(point);
        }
        else {
            pointBoard->unbind(point);
        }
    });

    point->focusIn();
}

void ObjectDesignerWidget::setPointEdit()
{
    // hide
    pointEditButton->hide();
    materialButton->hide();
    lightButton->hide();

    objectWidget->hide();

    hideAllBoards();

    // show
    pointListTitle->show();
    pointAddButton->show();
    pointList->show();
    textureEditButton->show();

    bezierWidget->show();
}

void ObjectDesignerWidget::setTextureEdit()
{
    // hide
    pointListTitle->hide();
    pointAddButton->hide();
    pointList->hide();
    textureEditButton->hide();

    bezierWidget->hide();

    hideAllBoards();

    // show
    pointEditButton->show();
    materialButton->show();
    lightButton->show();

    objectWidget->setRotatorLine(bezierWidget->getBezierLine());
    objectWidget->show();
}

void ObjectDesignerWidget::hideAllBoards()
{
    pointBoard->hide();
    materialBoard->hide();
    lightBoard->hide();
}

void ObjectDesignerWidget::paintEvent(QPaintEvent* ev)
{
    QPainter painter(this);
    QPixmap pix;

    // draw background
    pix.load(":/image/background.png");
    pix = pix.scaled(Utils::winWidth, Utils::winHeight, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    painter.drawPixmap((Utils::winWidth - pix.width()) / 2, (Utils::winHeight - pix.height()) / 2, pix);

    // draw mask
    pix.load(":/image/mask.png");
    pix = pix.scaled(Utils::winWidth, Utils::winHeight);
    painter.drawPixmap((Utils::winWidth - pix.width()) / 2, (Utils::winHeight - pix.height()) / 2, pix);

    DesignerWidget::paintEvent(ev);
}

void ObjectDesignerWidget::save()
{
    // check dir
    QDir dir;
    if (!dir.exists("./object")) {
        dir.mkdir("./object");
    }

    // get path
    const QString path = QFileDialog::getSaveFileName(this, "Save Object", "./object", "Object File (*.obj)");
    if (path == "") return;

    // open file
    QFile file(path);
    file.open(QIODevice::ReadWrite | QIODevice::Truncate);
    QDataStream data(&file);

    // save
    bezierWidget->save(data);
    objectWidget->save(data);
}

void ObjectDesignerWidget::load()
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

    // load
    int size = 0;
    data >> size;
    pointCounter = size;
    bezierWidget->clear();

    QString name;
    ControlPoint::Type type;
    QPoint pos, offset;
    for (int i = 0; i < size; ++i) {
        data >> name;
        data >> type;
        data >> pos;
        data >> offset;
        addPoint(name, type, pos, offset);
    }

    objectWidget->load(data);
    objectWidget->setRotatorLine(bezierWidget->getBezierLine());

    materialBoard->setObjectWidget(objectWidget);
    lightBoard->setObjectWidget(objectWidget);

    setPointEdit();
}
