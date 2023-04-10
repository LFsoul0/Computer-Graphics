#ifndef LIGHTBOARD_H
#define LIGHTBOARD_H

#include <QLineEdit>
#include <QDoubleValidator>

#include "PropertyBoard.h"
#include "ObjectViewWidget.h"

class LightBoard : public PropertyBoard
{
    Q_OBJECT
public:
    explicit LightBoard(QWidget *parent = nullptr);

    virtual Type type() override;

    void setObjectWidget(ObjectViewWidget* widget);

private:
    ObjectViewWidget* objectWidget;

    QLabel* directionLabel;
    QLabel* directionXLabel;
    QLineEdit* directionXEdit;
    QLabel* directionYLabel;
    QLineEdit* directionYEdit;
    QLabel* directionZLabel;
    QLineEdit* directionZEdit;

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

    void updateDirection();
    void updateAmbient();
    void updateDiffuse();
    void updateSpecular();

signals:

};

#endif // LIGHTBOARD_H
