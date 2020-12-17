#include "theorywidget.h"
#include "practicewidget.h"
#include "solverwidget.h"
#include "ui_theorywidget.h"

TheoryWidget::TheoryWidget(QWidget *parent, MainWindow *mw_parent, bool backButton) :
    QWidget(parent),
    ui(new Ui::TheoryWidget),
    mw_parent(mw_parent),
    backButton(backButton)
{
    ui->setupUi(this);

    if (backButton) {
        ui->button_practice->hide();
        ui->button_solver->hide();
    } else {
        ui->button_back->hide();
    }
}

TheoryWidget::~TheoryWidget()
{
    delete ui;
}

void TheoryWidget::on_button_practice_clicked()
{
    mw_parent->changeWidgetTo<PracticeWidget>();
}

void TheoryWidget::on_button_solver_clicked()
{
    mw_parent->changeWidgetTo<SolverWidget>();
}

void TheoryWidget::on_button_back_clicked()
{
    mw_parent->widgetPopBack();
}
