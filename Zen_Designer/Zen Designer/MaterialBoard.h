#ifndef MATERIALBOARD_H
#define MATERIALBOARD_H

#include <QRadioButton>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QIntValidator>

#include "PropertyBoard.h"
#include "ObjectViewWidget.h"

class MaterialBoard : public PropertyBoard
{
    Q_OBJECT
public:
    explicit MaterialBoard(QWidget *parent = nullptr);

    virtual Type type() override;

    void setObjectWidget(ObjectViewWidget* widget);

private:
    ObjectViewWidget* objectWidget;

    QLabel* typeLabel;
    QVector<QRadioButton *> typeButtons;

    QLabel* ambientLabel;
    QLabel* ambientRLabel;
    QLineEdit* ambientREdit;
    QLabel* ambientGLabel;
    QLineEdit* ambientGEdit;
    QLabel* ambientBLabel;
    QLineEdit* ambientBEdit;

    QLabel* diffuseLabel;
    QLabel* diffuseRLabel;
    QLineEdit* diffuseREdit;
    QLabel* diffuseGLabel;
    QLineEdit* diffuseGEdit;
    QLabel* diffuseBLabel;
    QLineEdit* diffuseBEdit;

    QLabel* specularLabel;
    QLabel* specularRLabel;
    QLineEdit* specularREdit;
    QLabel* specularGLabel;
    QLineEdit* specularGEdit;
    QLabel* specularBLabel;
    QLineEdit* specularBEdit;

    QLabel* shininessLabel;
    QLineEdit* shininessEdit;

    QLabel* repeatLabel;
    QLineEdit* repeatEdit;

    void updateAmbient();
    void updateDiffuse();
    void updateSpecular();
    void updateShininess();
    void updateRepeat();
    void checkTex();

signals:

};

#endif // MATERIALBOARD_H
