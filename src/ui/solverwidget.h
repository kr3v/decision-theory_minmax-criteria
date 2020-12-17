#ifndef SOLVERWIDGET_H
#define SOLVERWIDGET_H

#include "mainwindow.h"
#include <QWidget>

#include "src/math/geometry.h"

namespace Ui {
class SolverWidget;
}

class SolverWidget : public QWidget
{
    Q_OBJECT

public:
    SolverWidget(QWidget *parent = nullptr, MainWindow* mw_parent = nullptr);
    SolverWidget(QWidget *parent, MainWindow* mw_parent, std::vector<Point> points);
    ~SolverWidget();

    void showEvent(QShowEvent *event) override;

private slots:
    void on_button_increaseMatrixItems_clicked();

    void on_button_decreaseMatrixItems_clicked();

    void on_button_doSolve_clicked();

    void on_button_backToPractice_clicked();

    void on_button_toTheory_clicked();

    void on_button_toPractice_clicked();

private:
    Ui::SolverWidget *ui;
    MainWindow* mw_parent;
    bool isFromPractice;

    void updateChartView();
};

#endif // SOLVERWIDGET_H
