#include "theorywidget.h"
#include "practicewidget.h"
#include "solverwidget.h"
#include "ui_practicewidget.h"
#include "utils.h"

#include "src/math/double_cmp.h"
#include "src/math/polygon.h"
#include "src/math/minmaxrandomized.h"

void fillTableValues(QTableWidget* tw) {
    auto points = generatePolygon(tw->rowCount(), 10.0, 10.0);
    fillTableValues(tw, points);
}

PracticeWidget::PracticeWidget(QWidget *parent, MainWindow* mw_parent) :
    QWidget(parent),
    ui(new Ui::PracticeWidget),
    mw_parent(mw_parent)
{
    ui->setupUi(this);

    ui->table_matrixItems->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table_matrixItems->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    on_spinbox_N_valueChanged(ui->spinbox_N->value());
    ui->radioButton_nonRandomizedSolution->click();
}

PracticeWidget::~PracticeWidget()
{
    delete ui;
}

void PracticeWidget::on_spinbox_N_valueChanged(int newRowCount)
{
    auto tw = ui->table_matrixItems;
    setTableWidgetRowCount(tw, newRowCount);
    fillTableValues(tw);

    ui->spinBox_nonRandomizedSolution_index->setMaximum(newRowCount);
    ui->spinBox_randomizedSolution_results_i_index->setMaximum(newRowCount);
    ui->spinBox_randomizedSolution_results_j_index->setMaximum(newRowCount);
}

void PracticeWidget::on_doubleSpinBox_randomizedSolution_results_i_probability_valueChanged(double arg1)
{
    ui->doubleSpinBox_randomizedSolution_results_j_probability->setValue(1 - arg1);
}

void PracticeWidget::on_doubleSpinBox_randomizedSolution_results_j_probability_valueChanged(double arg1)
{
    ui->doubleSpinBox_randomizedSolution_results_i_probability->setValue(1 - arg1);
}

void PracticeWidget::on_radioButton_nonRandomizedSolution_clicked()
{
    this->randomizedSolutionChosen = false;

    ui->spinBox_randomizedSolution_results_i_index->setDisabled(true);
    ui->spinBox_randomizedSolution_results_j_index->setDisabled(true);
    ui->doubleSpinBox_randomizedSolution_results_i_probability->setDisabled(true);
    ui->doubleSpinBox_randomizedSolution_results_j_probability->setDisabled(true);
    ui->spinBox_nonRandomizedSolution_index->setDisabled(false);
}

void PracticeWidget::on_radioButton_randomizedSolution_clicked()
{
    this->randomizedSolutionChosen = true;

    ui->spinBox_randomizedSolution_results_i_index->setDisabled(false);
    ui->spinBox_randomizedSolution_results_j_index->setDisabled(false);
    ui->doubleSpinBox_randomizedSolution_results_i_probability->setDisabled(false);
    ui->doubleSpinBox_randomizedSolution_results_j_probability->setDisabled(false);
    ui->spinBox_nonRandomizedSolution_index->setDisabled(true);
}

void PracticeWidget::on_button_toSolver_clicked()
{
    mw_parent->changeWidgetTo<SolverWidget>();
}

void PracticeWidget::on_button_toTheory_clicked()
{
    mw_parent->stackWidget<TheoryWidget>(true);
}

void PracticeWidget::on_button_toSolverWithCurrentTask_clicked()
{
    mw_parent->stackWidget<SolverWidget>(extractFromTableWidget(ui->table_matrixItems));
}

template <typename T>
QString n(T t) {
    return QString::number(t);
}

#include <sstream>
template <typename T>
QString ss(T t) {
    std::stringstream s;
    s << t;
    return QString::fromStdString(s.str());
}

void PracticeWidget::on_button_verify_clicked()
{
    auto points = extractFromTableWidget(ui->table_matrixItems);
    auto solution = minMaxRandomized(points);

    auto isRandomized = solution.isPairParent();
    if (isRandomized != randomizedSolutionChosen) {
        // bad
        if (randomizedSolutionChosen) {
            ui->label_error->setText("Randomized solution was chosen, while non-randomized solution was expected.");
        } else {
            ui->label_error->setText("Non-randomized solution was chosen, while randomized solution was expected.");
        }
        ui->label_incorrectValue->setText(QString::number(ui->label_incorrectValue->text().toInt() + 1));
        return;
    }

    QString error;
    if (isRandomized) {
        auto i_index = ui->spinBox_randomizedSolution_results_i_index->value() - 1;
        auto j_index = ui->spinBox_randomizedSolution_results_j_index->value() - 1;
        auto i_probability = ui->doubleSpinBox_randomizedSolution_results_i_probability->value();
        auto j_probability = ui->doubleSpinBox_randomizedSolution_results_j_probability->value();

        auto [expected_solution1_index, expected_solution2_index] = solution.pairParent();
        auto expected_solution1_probability = probability(points[expected_solution1_index], points[expected_solution2_index]);
        auto expected_solution2_probability = 1 - expected_solution1_probability;

        bool e11 = expected_solution1_index == i_index,
             e12 = expected_solution1_index == j_index,
             e21 = expected_solution2_index == i_index,
             e22 = expected_solution2_index == j_index;

        if ((e11 && e22 && eq(expected_solution1_probability, i_probability, 2e-2) && eq(expected_solution2_probability, j_probability, 2e-2)) ||
            (e12 && e21 && eq(expected_solution1_probability, j_probability, 2e-2) && eq(expected_solution2_probability, i_probability, 2e-2))) {
            // good
            ui->label_error->setText("Correct!");
            ui->label_correctValue->setText(QString::number(ui->label_correctValue->text().toInt() + 1));
            fillTableValues(ui->table_matrixItems);
        } else {
            // bad
            auto error = QString("Wrong! Points %1-th %2 and %3-th %4 with probabilities %5 and %6 were expected to be a solution.")
                    .arg(n(expected_solution1_index + 1), ss(points[expected_solution1_index]),
                         n(expected_solution2_index + 1), ss(points[expected_solution2_index]),
                         n(roundTo(expected_solution1_probability, 100)),
                         n(roundTo(expected_solution2_probability, 100)));
            ui->label_error->setText(error);
            ui->label_incorrectValue->setText(QString::number(ui->label_incorrectValue->text().toInt() + 1));
        }
    } else {
        auto index = ui->spinBox_nonRandomizedSolution_index->value() - 1;
        auto expectedIndex = solution.singleParent();

        if (expectedIndex == index) {
            // good
            ui->label_error->setText("Correct!");
            ui->label_correctValue->setText(QString::number(ui->label_correctValue->text().toInt() + 1));
            fillTableValues(ui->table_matrixItems);
        } else {
            // bad
            ui->label_error->setText(QString("Wrong! Point %1-th %2 was expected to be a solution.")
                .arg(n(expectedIndex + 1), ss(points[expectedIndex])));
            ui->label_incorrectValue->setText(QString::number(ui->label_incorrectValue->text().toInt() + 1));
        }
    }
}

void PracticeWidget::on_button_generate_clicked()
{
    fillTableValues(ui->table_matrixItems);
}
