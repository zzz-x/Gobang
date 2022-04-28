#include "QT.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QT w;
    w.setWindowFlags(w.windowFlags() & ~Qt::WindowMaximizeButtonHint);

    w.show();
    return a.exec();    
}
