#ifndef THEORYWIDGET_H
#define THEORYWIDGET_H

#include <QWidget>

#include "mainwindow.h"

namespace Ui {
class TheoryWidget;
}

class TheoryWidget : public QWidget
{
    Q_OBJECT

public:
    TheoryWidget(QWidget *parent = nullptr, MainWindow *mw_parent = nullptr, bool backButton = false);
    ~TheoryWidget();

private slots:

    void on_button_practice_clicked();

    void on_button_solver_clicked();

    void on_button_back_clicked();

private:
    Ui::TheoryWidget *ui;
    MainWindow* mw_parent;

    bool backButton;
};

#endif // THEORYWIDGET_H
