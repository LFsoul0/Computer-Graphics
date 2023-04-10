#include "ListScrollArea.h"

ListScrollArea::ListScrollArea(QWidget *parent) : ListScrollArea(QSize(250, 400), parent) {}

ListScrollArea::ListScrollArea(const QSize& size, QWidget *parent) : QScrollArea(parent)
{
    setFixedSize(size);
    setStyleSheet("QScrollArea { background: transparent; border: 0px; }");
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    verticalScrollBar()->setStyleSheet("\
        QScrollBar:vertical\
        {\
            border: 0px;\
            background: black;\
            width: 15px;\
            margin: 20px 0px 20px 0px;\
        }\
        QScrollBar::handle:vertical\
        {\
            background: white;\
            min-height: 20px;\
        }\
        QScrollBar::add-line:vertical\
        {\
            border: 2px white;\
            background: solid grey;\
            height: 20px;\
            subcontrol-position: bottom;\
            subcontrol-origin: margin;\
        }\
        QScrollBar::sub-line:vertical\
        {\
            border: 2px white;\
            background: solid grey;\
            height: 20px;\
            subcontrol-position: top;\
            subcontrol-origin: margin;\
        }\
        QScrollBar:up-arrow:vertical, QScrollBar::down-arrow:vertical\
        {\
            border: 2px solid grey;\
            width: 3px;\
            height: 3px;\
            background: white;\
        }\
        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical\
        {\
            background: transparent;\
        }");

    subWidget = new QWidget();
    layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);
    subWidget->setLayout(layout);
    subWidget->setMinimumSize(size);
    subWidget->setStyleSheet("QWidget { background: transparent; }");
    setWidget(subWidget);
}

void ListScrollArea::addWidget(QWidget* w)
{
    layout->addWidget(w, w->height());
    connect(w, &QWidget::destroyed, this, [=](QObject* obj) {
        QWidget* w = dynamic_cast<QWidget *>(obj);
        removeWidget(w);
    });
    subWidget->adjustSize();
}

void ListScrollArea::removeWidget(QWidget *w)
{
    layout->removeWidget(w);
    subWidget->adjustSize();
}
