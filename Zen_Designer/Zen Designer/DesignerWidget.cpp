#include "DesignerWidget.h"

DesignerWidget::DesignerWidget(QWidget *parent) : QWidget(parent)
{
    // set window size
    setFixedSize(parent->size());

    // create left sidebar
    leftSidebar = new QLabel(this);
    leftSidebar->setFixedSize(300, height());
    leftSidebar->setPixmap(QPixmap(":/image/mask.png"));

    modeTitle = new QLabel("Designer Mode", leftSidebar);
    modeTitle->setFixedSize(300, 80);
    modeTitle->move((leftSidebar->width() - modeTitle->width()) / 2, 0);
    modeTitle->setFont(Utils::menuFont());
    modeTitle->setAlignment(Qt::AlignCenter);
    modeTitle->setStyleSheet("color: white");

    saveButton = new MenuButton("Save", leftSidebar);
    saveButton->move((leftSidebar->width() - saveButton->width()) / 2, height() - 230);
    saveButton->setTriggeredFontSizeByRatio(1.2);
    saveButton->setNormalColor(Qt::white);
    saveButton->setPressedColor(Qt::cyan);

    loadButton = new MenuButton("Load", leftSidebar);
    loadButton->move((leftSidebar->width() - loadButton->width()) / 2, height() - 180);
    loadButton->setTriggeredFontSizeByRatio(1.2);
    loadButton->setNormalColor(Qt::white);
    loadButton->setPressedColor(Qt::cyan);

    modeButton = new MenuButton("Switch Mode", leftSidebar);
    modeButton->move((leftSidebar->width() - modeButton->width()) / 2, height() - 130);
    modeButton->setTriggeredFontSizeByRatio(1.2);
    modeButton->setNormalColor(Qt::white);
    modeButton->setPressedColor(Qt::cyan);

    backButton = new MenuButton("Back", leftSidebar);
    backButton->move((leftSidebar->width() - backButton->width()) / 2, height() - 80);
    backButton->setTriggeredFontSizeByRatio(1.2);
    backButton->setNormalColor(Qt::white);
    backButton->setPressedColor(Qt::cyan);
    connect(backButton, &MenuButton::clicked, this, &DesignerWidget::back);
}

DesignerWidget::~DesignerWidget()
{

}
