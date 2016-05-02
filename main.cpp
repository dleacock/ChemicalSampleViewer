#include "sampleview.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SampleView sampleView_;
    sampleView_.show();

    return a.exec();
}
