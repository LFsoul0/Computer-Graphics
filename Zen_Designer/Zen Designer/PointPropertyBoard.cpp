#include "PointPropertyBoard.h"

PointPropertyBoard::PointPropertyBoard(QWidget *parent) : PropertyBoard(parent), point(nullptr)
{
    title->setText("Point");

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
        if (point) {
            point->setName(nameEdit->text());
        }
    });

    typeLabel = new QLabel("Type", this);
    typeLabel->setFixedSize(150, 80);
    typeLabel->move(20, 250);
    typeLabel->setFont(Utils::menuFont());
    typeLabel->setStyleSheet("color: white");
    straightButton = new QRadioButton("Straight", this);
    straightButton->setFixedSize(130, 40);
    straightButton->move(20, 320);
    straightButton->setFont(Utils::menuFont(32));
    straightButton->setStyleSheet("color: white");
    curveButton = new QRadioButton("Curve", this);
    curveButton->setFixedSize(130, 40);
    curveButton->move(170, 320);
    curveButton->setFont(Utils::menuFont(32));
    curveButton->setStyleSheet("color: white");
    connect(straightButton, &QRadioButton::toggled, this, [=](bool checked) {
        if (point && checked) {
            point->setType(ControlPoint::StraightLine);
        }
    });
    connect(curveButton, &QRadioButton::toggled, this, [=](bool checked) {
        if (point && checked) {
            point->setType(ControlPoint::Curve);
        }
    });

    posLabel = new QLabel("Position", this);
    posLabel->setFixedSize(150, 80);
    posLabel->move(20, 400);
    posLabel->setFont(Utils::menuFont());
    posLabel->setStyleSheet("color: white");
    posXLabel = new QLabel("X", this);
    posXLabel->setFixedSize(40, 40);
    posXLabel->move(20, 470);
    posXLabel->setFont(Utils::textFont());
    posXLabel->setStyleSheet("color: white");
    posXEdit = new QLineEdit(this);
    posXEdit->setFixedSize(90, 40);
    posXEdit->move(55, 470);
    posXEdit->setFont(Utils::textFont());
    posXEdit->setStyleSheet(lineEditStyle);
    posXEdit->setValidator(new QIntValidator(0, 9999, posXEdit));
    posYLabel = new QLabel("Y", this);
    posYLabel->setFixedSize(40, 40);
    posYLabel->move(155, 470);
    posYLabel->setFont(Utils::textFont());
    posYLabel->setStyleSheet("color: white");
    posYEdit = new QLineEdit(this);
    posYEdit->setFixedSize(90, 40);
    posYEdit->move(190, 470);
    posYEdit->setFont(Utils::textFont());
    posYEdit->setStyleSheet(lineEditStyle);
    posYEdit->setValidator(new QIntValidator(0, 9999, posYEdit));
    connect(posXEdit, &QLineEdit::editingFinished, this, &PointPropertyBoard::updatePos);
    connect(posYEdit, &QLineEdit::editingFinished, this, &PointPropertyBoard::updatePos);

    offsetLabel = new QLabel("Offset", this);
    offsetLabel->setFixedSize(150, 80);
    offsetLabel->move(20, 550);
    offsetLabel->setFont(Utils::menuFont());
    offsetLabel->setStyleSheet("color: white");
    offsetXLabel = new QLabel("X", this);
    offsetXLabel->setFixedSize(40, 40);
    offsetXLabel->move(20, 620);
    offsetXLabel->setFont(Utils::textFont());
    offsetXLabel->setStyleSheet("color: white");
    offsetXEdit = new QLineEdit(this);
    offsetXEdit->setFixedSize(90, 40);
    offsetXEdit->move(55, 620);
    offsetXEdit->setFont(Utils::textFont());
    offsetXEdit->setStyleSheet(lineEditStyle);
    offsetXEdit->setValidator(new QIntValidator(-9999, 9999, offsetXEdit));
    offsetYLabel = new QLabel("Y", this);
    offsetYLabel->setFixedSize(40, 40);
    offsetYLabel->move(155, 620);
    offsetYLabel->setFont(Utils::textFont());
    offsetYLabel->setStyleSheet("color: white");
    offsetYEdit = new QLineEdit(this);
    offsetYEdit->setFixedSize(90, 40);
    offsetYEdit->move(190, 620);
    offsetYEdit->setFont(Utils::textFont());
    offsetYEdit->setStyleSheet(lineEditStyle);
    offsetYEdit->setValidator(new QIntValidator(-9999, 9999, offsetYEdit));
    connect(offsetXEdit, &QLineEdit::editingFinished, this, &PointPropertyBoard::updateOffset);
    connect(offsetYEdit, &QLineEdit::editingFinished, this, &PointPropertyBoard::updateOffset);

    removeButton = new MenuButton("Remove", this);
    removeButton->setFixedSize(200, 80);
    removeButton->move((width() - removeButton->width()) / 2, 700);
    removeButton->setTriggeredFontSizeByRatio(1.2);
    removeButton->setNormalColor(Qt::yellow);
    removeButton->setPressedColor(Qt::cyan);
    connect(removeButton, &MenuButton::clicked, this, [=]() {
        if (point) {
            point->deleteLater();
        }
    });
}

PropertyBoard::Type PointPropertyBoard::type()
{
    return Point;
}

void PointPropertyBoard::bind(ControlPoint* point)
{
    unbind(this->point);
    this->point = point;

    nameEdit->setText(point->getName());
    switch (point->getType()) {
    case ControlPoint::StraightLine:
        straightButton->setChecked(true);
        break;
    case ControlPoint::Curve:
        curveButton->setChecked(true);
        break;
    }
    setPos(point->getPos());
    setOffset(point->getOffset());
    connect(point, &ControlPoint::changePos, this, &PointPropertyBoard::setPos);
    connect(point, &ControlPoint::changeOffset, this, &PointPropertyBoard::setOffset);
    connect(point, &ControlPoint::destroyed, this, [=]() {
        this->point = nullptr;
        hide();
    });

    show();
}

void PointPropertyBoard::unbind(ControlPoint* point)
{
    if (this->point != point || !point) return;
    this->point = nullptr;
    point->disconnect(this);
    point->focusOut();
    hide();
}

void PointPropertyBoard::setPos(const QPoint& p)
{
    posXEdit->setText(QString::number(p.x()));
    posYEdit->setText(QString::number(p.y()));
}

void PointPropertyBoard::setOffset(const QPoint& p)
{
    offsetXEdit->setText(QString::number(p.x()));
    offsetYEdit->setText(QString::number(p.y()));
}

void PointPropertyBoard::updatePos()
{
    if (!point) return;
    point->move(QPoint(posXEdit->text().toInt(), posYEdit->text().toInt()));
}

void PointPropertyBoard::updateOffset()
{
    if (!point) return;
    point->setOffset(QPoint(offsetXEdit->text().toInt(), offsetYEdit->text().toInt()));
}
