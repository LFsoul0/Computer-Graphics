#ifndef DESIGNERWIDGET_H
#define DESIGNERWIDGET_H

#include <QWidget>
#include <QLabel>

#include "Utils.h"
#include "MenuButton.h"

class DesignerWidget : public QWidget
{
    Q_OBJECT
public:
    enum Mode {
        ObjectMode,
        SceneMode
    };

    explicit DesignerWidget(QWidget *parent = nullptr);
    virtual ~DesignerWidget();

    virtual Mode mode() = 0;

protected:
    // left sidebar
    QLabel* leftSidebar;
    QLabel* modeTitle;
    MenuButton* saveButton;
    MenuButton* loadButton;
    MenuButton* modeButton;
    MenuButton* backButton;

signals:
    void back();
    void switchMode(Mode mode, DesignerWidget* current);

};

#endif // DESIGNERWIDGET_H
