#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "solverwidget.h"
#include "theorywidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    changeWidgetTo<TheoryWidget>();
}

MainWindow::~MainWindow()
{
    delete ui;
}
