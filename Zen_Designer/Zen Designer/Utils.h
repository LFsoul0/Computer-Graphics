#ifndef UTILS_H
#define UTILS_H

#include <QFont>

namespace Utils {

constexpr int winWidth = 1600;
constexpr int winHeight = 900;

const QString appName = "Zen Designer";

QFont titleFont(int pixelSize = 72, bool bold = true, bool italic = false);
QFont menuFont(int pixelSize = 45, bool bold = true, bool italic = false);
QFont textFont(int pixelSize = 32, bool bold = false, bool italic = false);

}

#endif // UTILS_H
