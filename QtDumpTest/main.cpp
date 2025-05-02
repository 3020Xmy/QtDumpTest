#include "QtDumpTest.h"
#include "dump.h"  
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    ::SetUnhandledExceptionFilter(ExceptionFilter);
    QApplication a(argc, argv);
    QtDumpTest w;
    w.show();
    return a.exec();
}
