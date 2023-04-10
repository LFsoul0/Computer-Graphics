#include "MaterialBoard.h"

MaterialBoard::MaterialBoard(QWidget *parent) : PropertyBoard(parent)
{
    title->setText("Material");

    typeLabel = new QLabel("Type", this);
    typeLabel->setFixedSize(200, 80);
    typeLabel->move(20, 60);
    typeLabel->setFont(Utils::menuFont());
    typeLabel->setStyleSheet("color: white");
    typeButtons.append(new QRadioButton("None", this));
    typeButtons[0]->setFixedSize(130, 40);
    typeButtons[0]->move(20, 130);
    typeButtons[0]->setFont(Utils::menuFont(32));
    typeButtons[0]->setStyleSheet("color: white");
    connect(typeButtons[0], &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) objectWidget->material.texIndex = -1;
        checkTex();
    });
    typeButtons.append(new QRadioButton("Wood1", this));
    typeButtons[1]->setFixedSize(130, 40);
    typeButtons[1]->move(20, 170);
    typeButtons[1]->setFont(Utils::menuFont(32));
    typeButtons[1]->setStyleSheet("color: white");
    connect(typeButtons[1], &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) objectWidget->material.texIndex = 0;
        checkTex();
    });
    typeButtons.append(new QRadioButton("Wood2", this));
    typeButtons[2]->setFixedSize(130, 40);
    typeButtons[2]->move(170, 170);
    typeButtons[2]->setFont(Utils::menuFont(32));
    typeButtons[2]->setStyleSheet("color: white");
    connect(typeButtons[2], &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) objectWidget->material.texIndex = 1;
        checkTex();
    });
    typeButtons.append(new QRadioButton("Rock1", this));
    typeButtons[3]->setFixedSize(130, 40);
    typeButtons[3]->move(20, 210);
    typeButtons[3]->setFont(Utils::menuFont(32));
    typeButtons[3]->setStyleSheet("color: white");
    connect(typeButtons[3], &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) objectWidget->material.texIndex = 2;
        checkTex();
    });
    typeButtons.append(new QRadioButton("Rock2", this));
    typeButtons[4]->setFixedSize(130, 40);
    typeButtons[4]->move(170, 210);
    typeButtons[4]->setFont(Utils::menuFont(32));
    typeButtons[4]->setStyleSheet("color: white");
    connect(typeButtons[4], &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) objectWidget->material.texIndex = 3;
        checkTex();
    });
    typeButtons.append(new QRadioButton("China1", this));
    typeButtons[5]->setFixedSize(130, 40);
    typeButtons[5]->move(20, 250);
    typeButtons[5]->setFont(Utils::menuFont(32));
    typeButtons[5]->setStyleSheet("color: white");
    connect(typeButtons[5], &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) objectWidget->material.texIndex = 4;
        checkTex();
    });
    typeButtons.append(new QRadioButton("China2", this));
    typeButtons[6]->setFixedSize(130, 40);
    typeButtons[6]->move(170, 250);
    typeButtons[6]->setFont(Utils::menuFont(32));
    typeButtons[6]->setStyleSheet("color: white");
    connect(typeButtons[6], &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) objectWidget->material.texIndex = 5;
        checkTex();
    });

    ambientLabel = new QLabel("Ambient", this);
    ambientLabel->setFixedSize(200, 80);
    ambientLabel->move(20, 280);
    ambientLabel->setFont(Utils::menuFont());
    ambientLabel->setStyleSheet("color: white");
    ambientRLabel = new QLabel("R", this);
    ambientRLabel->setFixedSize(30, 40);
    ambientRLabel->move(20, 350);
    ambientRLabel->setFont(Utils::textFont());
    ambientRLabel->setStyleSheet("color: white");
    ambientREdit = new QLineEdit(this);
    ambientREdit->setFixedSize(60, 40);
    ambientREdit->move(45, 350);
    ambientREdit->setFont(Utils::textFont(18));
    ambientREdit->setStyleSheet(lineEditStyle);
    ambientREdit->setValidator(new QDoubleValidator(0.0, 1.0, 3, ambientREdit));
    ambientGLabel = new QLabel("G", this);
    ambientGLabel->setFixedSize(30, 40);
    ambientGLabel->move(110, 350);
    ambientGLabel->setFont(Utils::textFont());
    ambientGLabel->setStyleSheet("color: white");
    ambientGEdit = new QLineEdit(this);
    ambientGEdit->setFixedSize(60, 40);
    ambientGEdit->move(135, 350);
    ambientGEdit->setFont(Utils::textFont(18));
    ambientGEdit->setStyleSheet(lineEditStyle);
    ambientGEdit->setValidator(new QDoubleValidator(0.0, 1.0, 3, ambientGEdit));
    ambientBLabel = new QLabel("B", this);
    ambientBLabel->setFixedSize(30, 40);
    ambientBLabel->move(200, 350);
    ambientBLabel->setFont(Utils::textFont());
    ambientBLabel->setStyleSheet("color: white");
    ambientBEdit = new QLineEdit(this);
    ambientBEdit->setFixedSize(60, 40);
    ambientBEdit->move(225, 350);
    ambientBEdit->setFont(Utils::textFont(18));
    ambientBEdit->setStyleSheet(lineEditStyle);
    ambientBEdit->setValidator(new QDoubleValidator(0.0, 1.0, 3, ambientBEdit));
    connect(ambientREdit, &QLineEdit::editingFinished, this, &MaterialBoard::updateAmbient);
    connect(ambientGEdit, &QLineEdit::editingFinished, this, &MaterialBoard::updateAmbient);
    connect(ambientBEdit, &QLineEdit::editingFinished, this, &MaterialBoard::updateAmbient);

    diffuseLabel = new QLabel("Diffuse", this);
    diffuseLabel->setFixedSize(200, 80);
    diffuseLabel->move(20, 380);
    diffuseLabel->setFont(Utils::menuFont());
    diffuseLabel->setStyleSheet("color: white");
    diffuseRLabel = new QLabel("R", this);
    diffuseRLabel->setFixedSize(30, 40);
    diffuseRLabel->move(20, 450);
    diffuseRLabel->setFont(Utils::textFont());
    diffuseRLabel->setStyleSheet("color: white");
    diffuseREdit = new QLineEdit(this);
    diffuseREdit->setFixedSize(60, 40);
    diffuseREdit->move(45, 450);
    diffuseREdit->setFont(Utils::textFont(18));
    diffuseREdit->setStyleSheet(lineEditStyle);
    diffuseREdit->setValidator(new QDoubleValidator(0.0, 1.0, 3, diffuseREdit));
    diffuseGLabel = new QLabel("G", this);
    diffuseGLabel->setFixedSize(30, 40);
    diffuseGLabel->move(110, 450);
    diffuseGLabel->setFont(Utils::textFont());
    diffuseGLabel->setStyleSheet("color: white");
    diffuseGEdit = new QLineEdit(this);
    diffuseGEdit->setFixedSize(60, 40);
    diffuseGEdit->move(135, 450);
    diffuseGEdit->setFont(Utils::textFont(18));
    diffuseGEdit->setStyleSheet(lineEditStyle);
    diffuseGEdit->setValidator(new QDoubleValidator(0.0, 1.0, 3, diffuseGEdit));
    diffuseBLabel = new QLabel("B", this);
    diffuseBLabel->setFixedSize(30, 40);
    diffuseBLabel->move(200, 450);
    diffuseBLabel->setFont(Utils::textFont());
    diffuseBLabel->setStyleSheet("color: white");
    diffuseBEdit = new QLineEdit(this);
    diffuseBEdit->setFixedSize(60, 40);
    diffuseBEdit->move(225, 450);
    diffuseBEdit->setFont(Utils::textFont(18));
    diffuseBEdit->setStyleSheet(lineEditStyle);
    diffuseBEdit->setValidator(new QDoubleValidator(0.0, 1.0, 3, diffuseBEdit));
    connect(diffuseREdit, &QLineEdit::editingFinished, this, &MaterialBoard::updateDiffuse);
    connect(diffuseGEdit, &QLineEdit::editingFinished, this, &MaterialBoard::updateDiffuse);
    connect(diffuseBEdit, &QLineEdit::editingFinished, this, &MaterialBoard::updateDiffuse);

    specularLabel = new QLabel("Specular", this);
    specularLabel->setFixedSize(200, 80);
    specularLabel->move(20, 480);
    specularLabel->setFont(Utils::menuFont());
    specularLabel->setStyleSheet("color: white");
    specularRLabel = new QLabel("R", this);
    specularRLabel->setFixedSize(30, 40);
    specularRLabel->move(20, 550);
    specularRLabel->setFont(Utils::textFont());
    specularRLabel->setStyleSheet("color: white");
    specularREdit = new QLineEdit(this);
    specularREdit->setFixedSize(60, 40);
    specularREdit->move(45, 550);
    specularREdit->setFont(Utils::textFont(18));
    specularREdit->setStyleSheet(lineEditStyle);
    specularREdit->setValidator(new QDoubleValidator(0.0, 1.0, 3, specularREdit));
    specularGLabel = new QLabel("G", this);
    specularGLabel->setFixedSize(30, 40);
    specularGLabel->move(110, 550);
    specularGLabel->setFont(Utils::textFont());
    specularGLabel->setStyleSheet("color: white");
    specularGEdit = new QLineEdit(this);
    specularGEdit->setFixedSize(60, 40);
    specularGEdit->move(135, 550);
    specularGEdit->setFont(Utils::textFont(18));
    specularGEdit->setStyleSheet(lineEditStyle);
    specularGEdit->setValidator(new QDoubleValidator(0.0, 1.0, 3, specularGEdit));
    specularBLabel = new QLabel("B", this);
    specularBLabel->setFixedSize(30, 40);
    specularBLabel->move(200, 550);
    specularBLabel->setFont(Utils::textFont());
    specularBLabel->setStyleSheet("color: white");
    specularBEdit = new QLineEdit(this);
    specularBEdit->setFixedSize(60, 40);
    specularBEdit->move(225, 550);
    specularBEdit->setFont(Utils::textFont(18));
    specularBEdit->setStyleSheet(lineEditStyle);
    specularBEdit->setValidator(new QDoubleValidator(0.0, 1.0, 3, specularBEdit));
    connect(specularREdit, &QLineEdit::editingFinished, this, &MaterialBoard::updateSpecular);
    connect(specularGEdit, &QLineEdit::editingFinished, this, &MaterialBoard::updateSpecular);
    connect(specularBEdit, &QLineEdit::editingFinished, this, &MaterialBoard::updateSpecular);

    shininessLabel = new QLabel("Shininess", this);
    shininessLabel->setFixedSize(200, 80);
    shininessLabel->move(20, 580);
    shininessLabel->setFont(Utils::menuFont());
    shininessLabel->setStyleSheet("color: white");
    shininessEdit = new QLineEdit(this);
    shininessEdit->setFixedSize(260, 40);
    shininessEdit->move(25, 650);
    shininessEdit->setFont(Utils::textFont(20));
    shininessEdit->setStyleSheet(lineEditStyle);
    shininessEdit->setValidator(new QDoubleValidator(0.0, 999.0, 3, shininessEdit));
    connect(shininessEdit, &QLineEdit::editingFinished, this, &MaterialBoard::updateShininess);

    repeatLabel = new QLabel("Texture Repeat", this);
    repeatLabel->setFixedSize(260, 80);
    repeatLabel->move(20, 680);
    repeatLabel->setFont(Utils::menuFont());
    repeatLabel->setStyleSheet("color: white");
    repeatEdit = new QLineEdit(this);
    repeatEdit->setFixedSize(260, 40);
    repeatEdit->move(25, 750);
    repeatEdit->setFont(Utils::textFont(20));
    repeatEdit->setStyleSheet(lineEditStyle);
    repeatEdit->setValidator(new QIntValidator(1, 9, repeatEdit));
    connect(repeatEdit, &QLineEdit::editingFinished, this, &MaterialBoard::updateRepeat);
}

PropertyBoard::Type MaterialBoard::type()
{
    return Material;
}

void MaterialBoard::setObjectWidget(ObjectViewWidget* widget)
{
    objectWidget = widget;
    typeButtons[widget->material.texIndex + 1]->setChecked(true);
    ambientREdit->setText(QString::number(widget->material.ambient.x()));
    ambientGEdit->setText(QString::number(widget->material.ambient.y()));
    ambientBEdit->setText(QString::number(widget->material.ambient.z()));
    diffuseREdit->setText(QString::number(widget->material.diffuse.x()));
    diffuseGEdit->setText(QString::number(widget->material.diffuse.y()));
    diffuseBEdit->setText(QString::number(widget->material.diffuse.z()));
    specularREdit->setText(QString::number(widget->material.specular.x()));
    specularGEdit->setText(QString::number(widget->material.specular.y()));
    specularBEdit->setText(QString::number(widget->material.specular.z()));
    shininessEdit->setText(QString::number(widget->material.shininess));
    repeatEdit->setText(QString::number(widget->textureRepeat));
}

void MaterialBoard::updateAmbient()
{
    objectWidget->material.ambient =
            QVector3D(ambientREdit->text().toFloat(), ambientGEdit->text().toFloat(), ambientBEdit->text().toFloat());
}

void MaterialBoard::updateDiffuse()
{
    objectWidget->material.diffuse =
            QVector3D(diffuseREdit->text().toFloat(), diffuseGEdit->text().toFloat(), diffuseBEdit->text().toFloat());
}

void MaterialBoard::updateSpecular()
{
    objectWidget->material.specular =
            QVector3D(specularREdit->text().toFloat(), specularGEdit->text().toFloat(), specularBEdit->text().toFloat());
}

void MaterialBoard::updateShininess()
{
    objectWidget->material.shininess = shininessEdit->text().toFloat();
}

void MaterialBoard::updateRepeat()
{
    objectWidget->setTextureRepeat(repeatEdit->text().toInt());
}

void MaterialBoard::checkTex()
{
    static const QString disabledLineEditStyle = "\
        QLineEdit {\
            border: 1px solid gray;\
            border-radius: 3px;\
            padding: 0 8px;\
            color: gray;\
            background: black;\
            selection-background-color: darkCyan;\
        }";

    if (objectWidget->material.texIndex < 0) {
        ambientREdit->setStyleSheet(lineEditStyle);
        ambientGEdit->setStyleSheet(lineEditStyle);
        ambientBEdit->setStyleSheet(lineEditStyle);
        diffuseREdit->setStyleSheet(lineEditStyle);
        diffuseGEdit->setStyleSheet(lineEditStyle);
        diffuseBEdit->setStyleSheet(lineEditStyle);
    }
    else {
        ambientREdit->setStyleSheet(disabledLineEditStyle);
        ambientGEdit->setStyleSheet(disabledLineEditStyle);
        ambientBEdit->setStyleSheet(disabledLineEditStyle);
        diffuseREdit->setStyleSheet(disabledLineEditStyle);
        diffuseGEdit->setStyleSheet(disabledLineEditStyle);
        diffuseBEdit->setStyleSheet(disabledLineEditStyle);
    }

    if (objectWidget->material.texIndex >= 2 && objectWidget->material.texIndex < 4) {
        specularREdit->setStyleSheet(disabledLineEditStyle);
        specularGEdit->setStyleSheet(disabledLineEditStyle);
        specularBEdit->setStyleSheet(disabledLineEditStyle);
    }
    else {
        specularREdit->setStyleSheet(lineEditStyle);
        specularGEdit->setStyleSheet(lineEditStyle);
        specularBEdit->setStyleSheet(lineEditStyle);
    }
}
