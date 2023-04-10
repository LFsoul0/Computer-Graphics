#include "LightBoard.h"

LightBoard::LightBoard(QWidget *parent) : PropertyBoard(parent)
{
    title->setText("Light");

    directionLabel = new QLabel("Direction", this);
    directionLabel->setFixedSize(200, 80);
    directionLabel->move(20, 100);
    directionLabel->setFont(Utils::menuFont());
    directionLabel->setStyleSheet("color: white");
    directionXLabel = new QLabel("X", this);
    directionXLabel->setFixedSize(30, 40);
    directionXLabel->move(20, 170);
    directionXLabel->setFont(Utils::textFont());
    directionXLabel->setStyleSheet("color: white");
    directionXEdit = new QLineEdit(this);
    directionXEdit->setFixedSize(60, 40);
    directionXEdit->move(45, 170);
    directionXEdit->setFont(Utils::textFont(18));
    directionXEdit->setStyleSheet(lineEditStyle);
    directionXEdit->setValidator(new QDoubleValidator(-1.0, 1.0, 3, directionXEdit));
    directionYLabel = new QLabel("Y", this);
    directionYLabel->setFixedSize(30, 40);
    directionYLabel->move(110, 170);
    directionYLabel->setFont(Utils::textFont());
    directionYLabel->setStyleSheet("color: white");
    directionYEdit = new QLineEdit(this);
    directionYEdit->setFixedSize(60, 40);
    directionYEdit->move(135, 170);
    directionYEdit->setFont(Utils::textFont(18));
    directionYEdit->setStyleSheet(lineEditStyle);
    directionYEdit->setValidator(new QDoubleValidator(-1.0, 1.0, 3, directionYEdit));
    directionZLabel = new QLabel("Z", this);
    directionZLabel->setFixedSize(30, 40);
    directionZLabel->move(200, 170);
    directionZLabel->setFont(Utils::textFont());
    directionZLabel->setStyleSheet("color: white");
    directionZEdit = new QLineEdit(this);
    directionZEdit->setFixedSize(60, 40);
    directionZEdit->move(225, 170);
    directionZEdit->setFont(Utils::textFont(18));
    directionZEdit->setStyleSheet(lineEditStyle);
    directionZEdit->setValidator(new QDoubleValidator(-1.0, 1.0, 3, directionZEdit));
    connect(directionXEdit, &QLineEdit::editingFinished, this, &LightBoard::updateDirection);
    connect(directionYEdit, &QLineEdit::editingFinished, this, &LightBoard::updateDirection);
    connect(directionZEdit, &QLineEdit::editingFinished, this, &LightBoard::updateDirection);

    ambientLabel = new QLabel("Ambient", this);
    ambientLabel->setFixedSize(200, 80);
    ambientLabel->move(20, 220);
    ambientLabel->setFont(Utils::menuFont());
    ambientLabel->setStyleSheet("color: white");
    ambientRLabel = new QLabel("R", this);
    ambientRLabel->setFixedSize(30, 40);
    ambientRLabel->move(20, 290);
    ambientRLabel->setFont(Utils::textFont());
    ambientRLabel->setStyleSheet("color: white");
    ambientREdit = new QLineEdit(this);
    ambientREdit->setFixedSize(60, 40);
    ambientREdit->move(45, 290);
    ambientREdit->setFont(Utils::textFont(18));
    ambientREdit->setStyleSheet(lineEditStyle);
    ambientREdit->setValidator(new QDoubleValidator(0.0, 1.0, 3, ambientREdit));
    ambientGLabel = new QLabel("G", this);
    ambientGLabel->setFixedSize(30, 40);
    ambientGLabel->move(110, 290);
    ambientGLabel->setFont(Utils::textFont());
    ambientGLabel->setStyleSheet("color: white");
    ambientGEdit = new QLineEdit(this);
    ambientGEdit->setFixedSize(60, 40);
    ambientGEdit->move(135, 290);
    ambientGEdit->setFont(Utils::textFont(18));
    ambientGEdit->setStyleSheet(lineEditStyle);
    ambientGEdit->setValidator(new QDoubleValidator(0.0, 1.0, 3, ambientGEdit));
    ambientBLabel = new QLabel("B", this);
    ambientBLabel->setFixedSize(30, 40);
    ambientBLabel->move(200, 290);
    ambientBLabel->setFont(Utils::textFont());
    ambientBLabel->setStyleSheet("color: white");
    ambientBEdit = new QLineEdit(this);
    ambientBEdit->setFixedSize(60, 40);
    ambientBEdit->move(225, 290);
    ambientBEdit->setFont(Utils::textFont(18));
    ambientBEdit->setStyleSheet(lineEditStyle);
    ambientBEdit->setValidator(new QDoubleValidator(0.0, 1.0, 3, ambientBEdit));
    connect(ambientREdit, &QLineEdit::editingFinished, this, &LightBoard::updateAmbient);
    connect(ambientGEdit, &QLineEdit::editingFinished, this, &LightBoard::updateAmbient);
    connect(ambientBEdit, &QLineEdit::editingFinished, this, &LightBoard::updateAmbient);

    diffuseLabel = new QLabel("Diffuse", this);
    diffuseLabel->setFixedSize(200, 80);
    diffuseLabel->move(20, 340);
    diffuseLabel->setFont(Utils::menuFont());
    diffuseLabel->setStyleSheet("color: white");
    diffuseRLabel = new QLabel("R", this);
    diffuseRLabel->setFixedSize(30, 40);
    diffuseRLabel->move(20, 410);
    diffuseRLabel->setFont(Utils::textFont());
    diffuseRLabel->setStyleSheet("color: white");
    diffuseREdit = new QLineEdit(this);
    diffuseREdit->setFixedSize(60, 40);
    diffuseREdit->move(45, 410);
    diffuseREdit->setFont(Utils::textFont(18));
    diffuseREdit->setStyleSheet(lineEditStyle);
    diffuseREdit->setValidator(new QDoubleValidator(0.0, 1.0, 3, diffuseREdit));
    diffuseGLabel = new QLabel("G", this);
    diffuseGLabel->setFixedSize(30, 40);
    diffuseGLabel->move(110, 410);
    diffuseGLabel->setFont(Utils::textFont());
    diffuseGLabel->setStyleSheet("color: white");
    diffuseGEdit = new QLineEdit(this);
    diffuseGEdit->setFixedSize(60, 40);
    diffuseGEdit->move(135, 410);
    diffuseGEdit->setFont(Utils::textFont(18));
    diffuseGEdit->setStyleSheet(lineEditStyle);
    diffuseGEdit->setValidator(new QDoubleValidator(0.0, 1.0, 3, diffuseGEdit));
    diffuseBLabel = new QLabel("B", this);
    diffuseBLabel->setFixedSize(30, 40);
    diffuseBLabel->move(200, 410);
    diffuseBLabel->setFont(Utils::textFont());
    diffuseBLabel->setStyleSheet("color: white");
    diffuseBEdit = new QLineEdit(this);
    diffuseBEdit->setFixedSize(60, 40);
    diffuseBEdit->move(225, 410);
    diffuseBEdit->setFont(Utils::textFont(18));
    diffuseBEdit->setStyleSheet(lineEditStyle);
    diffuseBEdit->setValidator(new QDoubleValidator(0.0, 1.0, 3, diffuseBEdit));
    connect(diffuseREdit, &QLineEdit::editingFinished, this, &LightBoard::updateDiffuse);
    connect(diffuseGEdit, &QLineEdit::editingFinished, this, &LightBoard::updateDiffuse);
    connect(diffuseBEdit, &QLineEdit::editingFinished, this, &LightBoard::updateDiffuse);

    specularLabel = new QLabel("Specular", this);
    specularLabel->setFixedSize(200, 80);
    specularLabel->move(20, 460);
    specularLabel->setFont(Utils::menuFont());
    specularLabel->setStyleSheet("color: white");
    specularRLabel = new QLabel("R", this);
    specularRLabel->setFixedSize(30, 40);
    specularRLabel->move(20, 530);
    specularRLabel->setFont(Utils::textFont());
    specularRLabel->setStyleSheet("color: white");
    specularREdit = new QLineEdit(this);
    specularREdit->setFixedSize(60, 40);
    specularREdit->move(45, 530);
    specularREdit->setFont(Utils::textFont(18));
    specularREdit->setStyleSheet(lineEditStyle);
    specularREdit->setValidator(new QDoubleValidator(0.0, 1.0, 3, specularREdit));
    specularGLabel = new QLabel("G", this);
    specularGLabel->setFixedSize(30, 40);
    specularGLabel->move(110, 530);
    specularGLabel->setFont(Utils::textFont());
    specularGLabel->setStyleSheet("color: white");
    specularGEdit = new QLineEdit(this);
    specularGEdit->setFixedSize(60, 40);
    specularGEdit->move(135, 530);
    specularGEdit->setFont(Utils::textFont(18));
    specularGEdit->setStyleSheet(lineEditStyle);
    specularGEdit->setValidator(new QDoubleValidator(0.0, 1.0, 3, specularGEdit));
    specularBLabel = new QLabel("B", this);
    specularBLabel->setFixedSize(30, 40);
    specularBLabel->move(200, 530);
    specularBLabel->setFont(Utils::textFont());
    specularBLabel->setStyleSheet("color: white");
    specularBEdit = new QLineEdit(this);
    specularBEdit->setFixedSize(60, 40);
    specularBEdit->move(225, 530);
    specularBEdit->setFont(Utils::textFont(18));
    specularBEdit->setStyleSheet(lineEditStyle);
    specularBEdit->setValidator(new QDoubleValidator(0.0, 1.0, 3, specularBEdit));
    connect(specularREdit, &QLineEdit::editingFinished, this, &LightBoard::updateSpecular);
    connect(specularGEdit, &QLineEdit::editingFinished, this, &LightBoard::updateSpecular);
    connect(specularBEdit, &QLineEdit::editingFinished, this, &LightBoard::updateSpecular);
}

PropertyBoard::Type LightBoard::type()
{
    return Light;
}

void LightBoard::setObjectWidget(ObjectViewWidget* widget)
{
    objectWidget = widget;
    directionXEdit->setText(QString::number(widget->light.direction.x()));
    directionYEdit->setText(QString::number(widget->light.direction.y()));
    directionZEdit->setText(QString::number(widget->light.direction.z()));
    ambientREdit->setText(QString::number(widget->light.ambient.x()));
    ambientGEdit->setText(QString::number(widget->light.ambient.y()));
    ambientBEdit->setText(QString::number(widget->light.ambient.z()));
    diffuseREdit->setText(QString::number(widget->light.diffuse.x()));
    diffuseGEdit->setText(QString::number(widget->light.diffuse.y()));
    diffuseBEdit->setText(QString::number(widget->light.diffuse.z()));
    specularREdit->setText(QString::number(widget->light.specular.x()));
    specularGEdit->setText(QString::number(widget->light.specular.y()));
    specularBEdit->setText(QString::number(widget->light.specular.z()));
}

void LightBoard::updateDirection()
{
    objectWidget->light.direction =
            QVector3D(directionXEdit->text().toFloat(), directionYEdit->text().toFloat(), directionZEdit->text().toFloat());
}

void LightBoard::updateAmbient()
{
    objectWidget->light.ambient =
            QVector3D(ambientREdit->text().toFloat(), ambientGEdit->text().toFloat(), ambientBEdit->text().toFloat());
}

void LightBoard::updateDiffuse()
{
    objectWidget->light.diffuse =
            QVector3D(diffuseREdit->text().toFloat(), diffuseGEdit->text().toFloat(), diffuseBEdit->text().toFloat());
}

void LightBoard::updateSpecular()
{
    objectWidget->light.specular =
            QVector3D(specularREdit->text().toFloat(), specularGEdit->text().toFloat(), specularBEdit->text().toFloat());
}
