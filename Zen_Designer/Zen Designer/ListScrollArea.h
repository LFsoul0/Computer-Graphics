#ifndef LISTSCROLLAREA_H
#define LISTSCROLLAREA_H

#include <QScrollArea>
#include <QScrollBar>
#include <QLayout>

class ListScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    explicit ListScrollArea(QWidget *parent = nullptr);
    explicit ListScrollArea(const QSize& size, QWidget *parent = nullptr);

    void addWidget(QWidget* w);
    void removeWidget(QWidget* w);

private:
    QWidget* subWidget;
    QVBoxLayout* layout;

signals:

};

#endif // LISTSCROLLAREA_H
