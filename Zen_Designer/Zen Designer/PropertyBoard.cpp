#include "PropertyBoard.h"

const QString PropertyBoard::lineEditStyle = "\
    QLineEdit {\
        border: 1px solid gray;\
        border-radius: 3px;\
        padding: 0 8px;\
        color: white;\
        background: black;\
        selection-background-color: darkCyan;\
    }";

PropertyBoard::PropertyBoard(QWidget *parent) : QLabel(parent)
{
    setFixedSize(300, parent->height());
    setPixmap(QPixmap(":/image/mask.png"));

    title = new QLabel("Properties", this);
    title->setFixedSize(250, 100);
    title->move(20, 0);
    title->setFont(Utils::menuFont());
    title->setStyleSheet("color: white");

    closeButton = new MenuButton("\u00D7", this); // 'x' symbol
    closeButton->setFixedSize(80, 100);
    closeButton->move(width() - closeButton->width(), 0);
    closeButton->setTriggeredFontSizeByRatio(1.2);
    closeButton->setNormalColor(Qt::white);
    closeButton->setPressedColor(Qt::cyan);
    connect(closeButton, &MenuButton::clicked, this, &PropertyBoard::hide);
}

PropertyBoard::~PropertyBoard()
{

}
