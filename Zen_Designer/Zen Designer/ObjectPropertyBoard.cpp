#include "ObjectPropertyBoard.h"

ObjectPropertyBoard::ObjectPropertyBoard(QWidget *parent) : PropertyBoard(parent)
{
    title->setText("Object");

    obj = nullptr;

    nameLabel = new QLabel("Name", this);
    nameLabel->setFixedSize(150, 80);
    nameLabel->move(20, 100);
    nameLabel->setFont(Utils::menuFont());
    nameLabel->setStyleSheet("color: white");
    nameEdit = new QLineEdit(this);
    nameEdit->setFixedSize(250, 40);
    nameEdit->move((width() - nameEdit->width()) / 2, 170);
    nameEdit->setFont(Utils::textFont());
    nameEdit->setStyleSheet(lineEditStyle);
    connect(nameEdit, &QLineEdit::editingFinished, this, [=]() {
        if (obj) {
            obj->setName(nameEdit->text());
        }
    });

    posLabel = new QLabel("Position", this);
    posLabel->setFixedSize(200, 80);
    posLabel->move(20, 220);
    posLabel->setFont(Utils::menuFont());
    posLabel->setStyleSheet("color: white");
    posXLabel = new QLabel("X", this);
    posXLabel->setFixedSize(30, 40);
    posXLabel->move(20, 290);
    posXLabel->setFont(Utils::textFont());
    posXLabel->setStyleSheet("color: white");
    posXEdit = new QLineEdit(this);
    posXEdit->setFixedSize(60, 40);
    posXEdit->move(45, 290);
    posXEdit->setFont(Utils::textFont(18));
    posXEdit->setStyleSheet(lineEditStyle);
    posXEdit->setValidator(new QDoubleValidator(-50.0, 50.0, 3, posXEdit));
    posYLabel = new QLabel("Y", this);
    posYLabel->setFixedSize(30, 40);
    posYLabel->move(110, 290);
    posYLabel->setFont(Utils::textFont());
    posYLabel->setStyleSheet("color: white");
    posYEdit = new QLineEdit(this);
    posYEdit->setFixedSize(60, 40);
    posYEdit->move(135, 290);
    posYEdit->setFont(Utils::textFont(18));
    posYEdit->setStyleSheet(lineEditStyle);
    posYEdit->setValidator(new QDoubleValidator(0.0, 100.0, 3, posYEdit));
    posZLabel = new QLabel("Z", this);
    posZLabel->setFixedSize(30, 40);
    posZLabel->move(200, 290);
    posZLabel->setFont(Utils::textFont());
    posZLabel->setStyleSheet("color: white");
    posZEdit = new QLineEdit(this);
    posZEdit->setFixedSize(60, 40);
    posZEdit->move(225, 290);
    posZEdit->setFont(Utils::textFont(18));
    posZEdit->setStyleSheet(lineEditStyle);
    posZEdit->setValidator(new QDoubleValidator(-50.0, 50.0, 3, posZEdit));
    connect(posXEdit, &QLineEdit::editingFinished, this, &ObjectPropertyBoard::updatePos);
    connect(posYEdit, &QLineEdit::editingFinished, this, &ObjectPropertyBoard::updatePos);
    connect(posZEdit, &QLineEdit::editingFinished, this, &ObjectPropertyBoard::updatePos);

    rotateLabel = new QLabel("Rotation", this);
    rotateLabel->setFixedSize(200, 80);
    rotateLabel->move(20, 340);
    rotateLabel->setFont(Utils::menuFont());
    rotateLabel->setStyleSheet("color: white");
    rotateXLabel = new QLabel("X", this);
    rotateXLabel->setFixedSize(30, 40);
    rotateXLabel->move(20, 410);
    rotateXLabel->setFont(Utils::textFont());
    rotateXLabel->setStyleSheet("color: white");
    rotateXEdit = new QLineEdit(this);
    rotateXEdit->setFixedSize(60, 40);
    rotateXEdit->move(45, 410);
    rotateXEdit->setFont(Utils::textFont(18));
    rotateXEdit->setStyleSheet(lineEditStyle);
    rotateXEdit->setValidator(new QDoubleValidator(-360.0, 360.0, 3, rotateXEdit));
    rotateYLabel = new QLabel("Y", this);
    rotateYLabel->setFixedSize(30, 40);
    rotateYLabel->move(110, 410);
    rotateYLabel->setFont(Utils::textFont());
    rotateYLabel->setStyleSheet("color: white");
    rotateYEdit = new QLineEdit(this);
    rotateYEdit->setFixedSize(60, 40);
    rotateYEdit->move(135, 410);
    rotateYEdit->setFont(Utils::textFont(18));
    rotateYEdit->setStyleSheet(lineEditStyle);
    rotateYEdit->setValidator(new QDoubleValidator(-360.0, 360.0, 3, rotateYEdit));
    rotateZLabel = new QLabel("Z", this);
    rotateZLabel->setFixedSize(30, 40);
    rotateZLabel->move(200, 410);
    rotateZLabel->setFont(Utils::textFont());
    rotateZLabel->setStyleSheet("color: white");
    rotateZEdit = new QLineEdit(this);
    rotateZEdit->setFixedSize(60, 40);
    rotateZEdit->move(225, 410);
    rotateZEdit->setFont(Utils::textFont(18));
    rotateZEdit->setStyleSheet(lineEditStyle);
    rotateZEdit->setValidator(new QDoubleValidator(-360.0, 360.0, 3, rotateZEdit));
    connect(rotateXEdit, &QLineEdit::editingFinished, this, &ObjectPropertyBoard::updateRotate);
    connect(rotateYEdit, &QLineEdit::editingFinished, this, &ObjectPropertyBoard::updateRotate);
    connect(rotateZEdit, &QLineEdit::editingFinished, this, &ObjectPropertyBoard::updateRotate);

    scaleLabel = new QLabel("Scale", this);
    scaleLabel->setFixedSize(200, 80);
    scaleLabel->move(20, 460);
    scaleLabel->setFont(Utils::menuFont());
    scaleLabel->setStyleSheet("color: white");
    scaleXLabel = new QLabel("X", this);
    scaleXLabel->setFixedSize(30, 40);
    scaleXLabel->move(20, 530);
    scaleXLabel->setFont(Utils::textFont());
    scaleXLabel->setStyleSheet("color: white");
    scaleXEdit = new QLineEdit(this);
    scaleXEdit->setFixedSize(60, 40);
    scaleXEdit->move(45, 530);
    scaleXEdit->setFont(Utils::textFont(18));
    scaleXEdit->setStyleSheet(lineEditStyle);
    scaleXEdit->setValidator(new QDoubleValidator(-100.0, 100.0, 3, scaleXEdit));
    scaleYLabel = new QLabel("Y", this);
    scaleYLabel->setFixedSize(30, 40);
    scaleYLabel->move(110, 530);
    scaleYLabel->setFont(Utils::textFont());
    scaleYLabel->setStyleSheet("color: white");
    scaleYEdit = new QLineEdit(this);
    scaleYEdit->setFixedSize(60, 40);
    scaleYEdit->move(135, 530);
    scaleYEdit->setFont(Utils::textFont(18));
    scaleYEdit->setStyleSheet(lineEditStyle);
    scaleYEdit->setValidator(new QDoubleValidator(-100.0, 100.0, 3, scaleYEdit));
    scaleZLabel = new QLabel("Z", this);
    scaleZLabel->setFixedSize(30, 40);
    scaleZLabel->move(200, 530);
    scaleZLabel->setFont(Utils::textFont());
    scaleZLabel->setStyleSheet("color: white");
    scaleZEdit = new QLineEdit(this);
    scaleZEdit->setFixedSize(60, 40);
    scaleZEdit->move(225, 530);
    scaleZEdit->setFont(Utils::textFont(18));
    scaleZEdit->setStyleSheet(lineEditStyle);
    scaleZEdit->setValidator(new QDoubleValidator(-100.0, 100.0, 3, scaleZEdit));
    connect(scaleXEdit, &QLineEdit::editingFinished, this, &ObjectPropertyBoard::updateScale);
    connect(scaleYEdit, &QLineEdit::editingFinished, this, &ObjectPropertyBoard::updateScale);
    connect(scaleZEdit, &QLineEdit::editingFinished, this, &ObjectPropertyBoard::updateScale);

    removeButton = new MenuButton("Remove", this);
    removeButton->setFixedSize(200, 80);
    removeButton->move((width() - removeButton->width()) / 2, 700);
    removeButton->setTriggeredFontSizeByRatio(1.2);
    removeButton->setNormalColor(Qt::yellow);
    removeButton->setPressedColor(Qt::cyan);
    connect(removeButton, &MenuButton::clicked, this, [=]() {
        if (obj) {
            obj->deleteLater();
        }
    });
}

PropertyBoard::Type ObjectPropertyBoard::type()
{
    return Object;
}

void ObjectPropertyBoard::bind(SceneObject* obj)
{
    unbind(this->obj);
    this->obj = obj;

    nameEdit->setText(obj->getName());
    setPos(obj->getPos());
    setRotate(obj->getRotate());
    setScale(obj->getScale());
    connect(obj, &SceneObject::destroyed, this, [=]() {
        this->obj = nullptr;
        hide();
    });

    show();
}

void ObjectPropertyBoard::unbind(SceneObject* obj)
{
    if (this->obj != obj || !obj) return;
    this->obj = nullptr;
    obj->focusOut();
    hide();
}

void ObjectPropertyBoard::setPos(const QVector3D& pos)
{
    posXEdit->setText(QString::number(pos.x()));
    posYEdit->setText(QString::number(pos.y()));
    posZEdit->setText(QString::number(pos.z()));
}

void ObjectPropertyBoard::setRotate(const QVector3D& rotate)
{
    rotateXEdit->setText(QString::number(rotate.x()));
    rotateYEdit->setText(QString::number(rotate.y()));
    rotateZEdit->setText(QString::number(rotate.z()));
}

void ObjectPropertyBoard::setScale(const QVector3D& scale)
{
    scaleXEdit->setText(QString::number(scale.x()));
    scaleYEdit->setText(QString::number(scale.y()));
    scaleZEdit->setText(QString::number(scale.z()));
}

void ObjectPropertyBoard::updatePos()
{
    if (!obj) return;
    obj->setPos(QVector3D(posXEdit->text().toFloat(), posYEdit->text().toFloat(), posZEdit->text().toFloat()));
}

void ObjectPropertyBoard::updateRotate()
{
    if (!obj) return;
    obj->setRotate(QVector3D(rotateXEdit->text().toFloat(), rotateYEdit->text().toFloat(), rotateZEdit->text().toFloat()));
}

void ObjectPropertyBoard::updateScale()
{
    if (!obj) return;
    obj->setScale(QVector3D(scaleXEdit->text().toFloat(), scaleYEdit->text().toFloat(), scaleZEdit->text().toFloat()));
}
