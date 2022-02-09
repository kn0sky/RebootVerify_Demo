#include "VaC_reboot.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VaC_reboot w;
    w.show();
    return a.exec();
}
