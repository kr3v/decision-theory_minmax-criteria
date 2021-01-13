#ifndef PRACTICEWIDGET_H
#define PRACTICEWIDGET_H

#include "mainwindow.h"

#include <QWidget>

namespace Ui {
class PracticeWidget;
}

class PracticeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PracticeWidget(QWidget *parent = nullptr, MainWindow* mw_parent = nullptr);
    ~PracticeWidget();

private slots:
    void on_button_toTheory_clicked();

    void on_button_toSolverWithCurrentTask_clicked();

    void on_button_verify_clicked();

    void on_spinbox_N_valueChanged(int arg1);

    void on_doubleSpinBox_randomizedSolution_results_j_probability_valueChanged(double arg1);

    void on_doubleSpinBox_randomizedSolution_results_i_probability_valueChanged(double arg1);

    void on_radioButton_nonRandomizedSolution_clicked();

    void on_radioButton_randomizedSolution_clicked();

    void on_button_generate_clicked();

private:
    Ui::PracticeWidget *ui;
    MainWindow* mw_parent;

    bool randomizedSolutionChosen;
};

#endif // PRACTICEWIDGET_H
