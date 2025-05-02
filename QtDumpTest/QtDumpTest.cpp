#include "QtDumpTest.h"
#include <qpushbutton.h>

QtDumpTest::QtDumpTest(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.pushButton, &QPushButton::clicked, this, &QtDumpTest::on_testDump);
    connect(ui.pushButton_2, &QPushButton::clicked, this, &QtDumpTest::on_test2Dump);
    connect(ui.pushButton_3, &QPushButton::clicked, this, &QtDumpTest::on_test3Dump);
    connect(ui.pushButton_4, &QPushButton::clicked, this, &QtDumpTest::on_test4Dump);
}

QtDumpTest::~QtDumpTest()
{}

/*  触发访问违规崩溃
*/
void QtDumpTest::on_testDump() {
    int* pTest = NULL;
    (*pTest) = 1;
}

/*  触发除零异常崩溃
*/
void QtDumpTest::on_test2Dump() {
    int a = 0;
    int x = 1 / a;
}

/*  触发数组越界崩溃
*/
void QtDumpTest::on_test3Dump() {
    int arr[5] = { 0 };
    arr[10] = 1; //越界访问
}

/*  触发异常未捕获崩溃
*/
void QtDumpTest::on_test4Dump() {
    throw std::runtime_error("抛出异常");
}