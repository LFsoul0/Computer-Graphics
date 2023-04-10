#ifndef PROPERTYBOARD_H
#define PROPERTYBOARD_H

#include <QLabel>
#include <QPixmap>

#include "Utils.h"
#include "MenuButton.h"

class PropertyBoard : public QLabel
{
    Q_OBJECT
public:
    enum Type {
        Point,
        Object,
        Material,
        Light,
        Environment
    };

    explicit PropertyBoard(QWidget *parent = nullptr);
    virtual ~PropertyBoard();

    virtual Type type() = 0;

protected:
    static const QString lineEditStyle;

    QLabel* title;
    MenuButton* closeButton;

signals:

};

#endif // PROPERTYBOARD_H
