#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QLabel>
#include <QTimer>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QSoundEffect>

#include "Utils.h"

class MenuButton : public QLabel
{
    Q_OBJECT

    Q_PROPERTY(int fontSize READ getFontSize WRITE setFontSize)

public:
    static QSoundEffect *sound;

    explicit MenuButton(QWidget *parent = nullptr);
    explicit MenuButton(const QString& text, QWidget *parent = nullptr);
    virtual ~MenuButton();

    int getFontSize();
    void setFontSize(int size);

    void setNormalFontSize(int pixelSize, bool current = true);
    void setTriggeredFontSize(int pixelSize, bool current = false);
    void setTriggeredFontSizeByRatio(double ratio, bool current = false);

    void setNormalColor(QColor color, bool current = true);
    void setPressedColor(QColor color, bool current = false);
    void setCheckedColor(QColor color, bool current = false);
    void setNormalBackgroundColor(QColor color, bool current = false);
    void setRaisedBackgroundColor(QColor color, bool current = false);

    void setCheckable(bool checkable);
    void setCheck(bool check);
    void check();

    void setAutoRaise(bool autoRaise);

protected:
    void enterEvent(QEvent* ev) override;
    void leaveEvent(QEvent* ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;

private:
    bool checkable;
    bool autoRaise;

    bool isEntered;
    bool isPressed;
    bool isChecked;

    int normalFontSize;
    int triggeredFontSize;

    QColor normalColor;
    QColor pressedColor;
    QColor checkedColor;
    QColor normalBackgroundColor;
    QColor raisedBackgroundColor;

    QColor fontColor;
    QColor backgroundColor;

    static constexpr int ANIM_TIME = 200;
    QPropertyAnimation* enterAnim;
    QPropertyAnimation* leaveAnim;

    void updateStyleSheet();

    static QSoundEffect* setSound();

signals:
    void pressed();
    void released();
    void clicked();
    void checked(bool isChecked);
};

#endif // MENUBUTTON_H
