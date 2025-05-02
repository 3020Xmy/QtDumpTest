#pragma once

#include <cassert>
#include <windows.h>
#include <stdexcept>
#include <QtWidgets/QMainWindow>
#include "ui_QtDumpTest.h"

class QtDumpTest : public QMainWindow
{
    Q_OBJECT

public:
    QtDumpTest(QWidget *parent = nullptr);
    ~QtDumpTest();

public slots:
    void on_testDump();     //触发访问违规崩溃
    void on_test2Dump();    //触发除零异常崩溃
    void on_test3Dump();    //触发数组越界崩溃
    void on_test4Dump();    //触发异常未捕获崩溃

private:
    Ui::QtDumpTestClass ui;
};
