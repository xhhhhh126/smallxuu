#include <QApplication>
#include "galaxy.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    GalaxyWindow w;
    w.setWindowTitle(QString::fromUtf8("✨ 粒子星系模拟器"));
    w.resize(1000, 800);
    w.show();
    return app.exec();
}