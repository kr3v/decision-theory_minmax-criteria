#include "theorywidget.h"
#include "practicewidget.h"
#include "solverwidget.h"
#include "ui_solverwidget.h"

#include "utils.h"

#include <QStringList>

#include <QValueAxis>
#include <QChartView>
#include <QChart>
#include <QAreaSeries>
#include <QLineSeries>
#include <QScatterSeries>

#include <cmath>
#include <iostream>

#include "src/math/geometry.h"
#include "src/math/convex_hull.h"
#include "src/math/minmaxrandomized.h"

void updateAxis(QtCharts::QChart* c, double max_x, double max_y);
void addXYAxisSeries(QtCharts::QChart* c, double max_x, double max_y);
QtCharts::QScatterSeries* nonRandomizedSolutionsSeries(std::vector<Point> points);
QtCharts::QLineSeries* randomizedSolutionsBoundariesSeries(std::vector<Point> points);
QtCharts::QAreaSeries* randomizedSolutionsAreaSeries(QtCharts::QLineSeries* s);
void addRandomizedSolutionsSeries(QtCharts::QChart* cv, std::vector<Point> points);
void addMinMaxCriteriaSolutionSeries(QtCharts::QChart* cv, std::vector<Point> points, RandomizedStrategyResult solution);
void fillMinMaxCriteriaSolutionLog(QTextBrowser* b, std::vector<Point> points, RandomizedStrategyResult solution);

SolverWidget::SolverWidget(QWidget *parent, MainWindow* mw_parent) :
    QWidget(parent),
    ui(new Ui::SolverWidget),
    mw_parent(mw_parent),
    isFromPractice(false)
{
    ui->setupUi(this);
    ui->table_matrixItems->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table_matrixItems->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->button_backToPractice->hide();
    updateChartView();
}

SolverWidget::SolverWidget(QWidget *parent, MainWindow* mw_parent, std::vector<Point> points) :
    QWidget(parent),
    ui(new Ui::SolverWidget),
    mw_parent(mw_parent),
    isFromPractice(true)
{
    ui->setupUi(this);
    ui->button_toPractice->hide();

    auto tw = ui->table_matrixItems;
    tw->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tw->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    setTableWidgetRowCount(tw, points.size());
    fillTableValues(tw, points);
    updateChartView();
}

SolverWidget::~SolverWidget()
{
    delete ui;
}

void SolverWidget::showEvent(QShowEvent* event) {
    fillTableNames(ui->table_matrixItems);
}

void SolverWidget::on_button_increaseMatrixItems_clicked()
{
    auto tw = ui->table_matrixItems;
    setTableWidgetRowCount(tw, tw->rowCount() + 1);
}

void SolverWidget::on_button_decreaseMatrixItems_clicked()
{
    auto tw = ui->table_matrixItems;
    setTableWidgetRowCount(tw, tw->rowCount() - 1);
}

void SolverWidget::on_button_doSolve_clicked()
{
    updateChartView();
}

#include <vector>
#include <algorithm>

template <typename T>
std::vector<T> filter(std::vector<T> v, std::vector<T> f) {
    std::vector<T> result;
    for (auto t: v) {
        auto found = false;
        for (auto p: f) {
            if (t == p) {
                found = true;
                break;
            }
        }
        if (!found) {
            result.push_back(t);
        }
    }
    return result;
}

void SolverWidget::updateChartView() {
    auto allPoints = extractFromTableWidget(ui->table_matrixItems);
    auto points = convexHull(allPoints);
    auto solution = minMaxRandomized(allPoints);

    double max_x = std::max_element(points.begin(), points.end(), [](auto p1, auto p2) { return p1.x < p2.x; } )->x;
    double max_y = std::max_element(points.begin(), points.end(), [](auto p1, auto p2) { return p1.y < p2.y; } )->y;

    fillMinMaxCriteriaSolutionLog(ui->text_solutionLog, allPoints, solution);

    auto chart = new QtCharts::QChart();
    chart->setTitle("Графік розв'язку рандомізованим мінімасним критерієм оптимальності");

    addXYAxisSeries(chart, max_x, max_y);
    addRandomizedSolutionsSeries(chart, points);
    chart->addSeries(nonRandomizedSolutionsSeries(filter(allPoints, solution.parents(allPoints))));
    addMinMaxCriteriaSolutionSeries(chart, allPoints, solution);

    chart->createDefaultAxes();
    updateAxis(chart, max_x, max_y);
    ui->chartsView->setChart(chart);
    ui->chartsView->setRenderHint(QPainter::Antialiasing);
}

/// axis prep

void updateAxis(QtCharts::QChart* c, double max_x, double max_y) {
    auto x_axis = dynamic_cast<QtCharts::QValueAxis*>(c->axisX());
    x_axis->setRange(-1, std::ceil(max_x + 1));
    x_axis->setTickCount(std::ceil(max_x + 1) + 2);
    x_axis->setMinorTickCount(1);

    auto y_axis = dynamic_cast<QtCharts::QValueAxis*>(c->axisY());
    y_axis->setRange(-1, std::ceil(max_y + 1));
    y_axis->setTickCount(std::ceil(max_y + 1) + 2);
    y_axis->setMinorTickCount(1);
}

void addXYAxisSeries(QtCharts::QChart* c, double max_x, double max_y) {
    auto x_axis_series = new QtCharts::QLineSeries();
    x_axis_series->append(-(max_x + 10.0) * 2, 0);
    x_axis_series->append(+(max_x + 10.0) * 2, 0);
    x_axis_series->setPen(QColorConstants::Black);
    x_axis_series->setName("x axis");

    auto y_axis_series = new QtCharts::QLineSeries();
    y_axis_series->append(0, -(max_y + 10.0) * 2);
    y_axis_series->append(0, +(max_y + 10.0) * 2);
    y_axis_series->setPen(QColorConstants::Black);
    y_axis_series->setName("y axis");

    c->addSeries(x_axis_series);
    c->addSeries(y_axis_series);
}

/// randomized solutions series

QtCharts::QScatterSeries* nonRandomizedSolutionsSeries(std::vector<Point> points) {
    auto ss = new QtCharts::QScatterSeries();
    for (auto p: points) {
        ss->append(p.x, p.y);
    }
    ss->setPointsVisible(true);
    ss->setName("non-randomized solutions");
    ss->setPen(QPen(QColorConstants::DarkRed, 4));
    ss->setBrush(QColorConstants::Red);
    return ss;
}

QtCharts::QLineSeries* randomizedSolutionsBoundariesSeries(std::vector<Point> points) {
    auto ls = new QtCharts::QLineSeries();
    for (auto p: points) {
        ls->append(p.x, p.y);
    }
    ls->append(ls->points().first());
    ls->setPointsVisible(true);
    ls->setName("randomized solutions boundary");
    ls->setPen(QPen(QColorConstants::DarkBlue, 2));
    return ls;
}

QtCharts::QAreaSeries* randomizedSolutionsAreaSeries(QtCharts::QLineSeries* s) {
    auto as = new QtCharts::QAreaSeries(s);
    as->setName("randomized solutions area");
    as->setPen(QColorConstants::Cyan);
    as->setBrush(QColorConstants::Cyan);
    return as;
}

void addRandomizedSolutionsSeries(QtCharts::QChart* cv, std::vector<Point> points) {
    auto ls = randomizedSolutionsBoundariesSeries(points);
    auto as = randomizedSolutionsAreaSeries(ls);
    cv->addSeries(as);
    cv->addSeries(ls);
}

/// minmax criteria application

void addMinMaxCriteriaSolutionSeries(QtCharts::QChart* cv, std::vector<Point> points, RandomizedStrategyResult solution) {
    auto ss1 = new QtCharts::QScatterSeries();
    ss1->append(solution.result.x, solution.result.y);
    ss1->setPen(QPen(QColorConstants::DarkGreen, 4));
    ss1->setBrush(QColorConstants::Green);
    ss1->setName("intersection point");
    ss1->setPointLabelsFormat("(@xPoint, @yPoint)");
    ss1->setPointLabelsVisible(true);

    auto ss2 = new QtCharts::QScatterSeries();
    for (auto p: solution.parents(points)) {
        ss2->append(p.x, p.y);
    }
    ss2->setPen(QPen(QColorConstants::DarkYellow, 4));
    ss2->setBrush(QColorConstants::Yellow);
    ss2->setName("probable non-randomized solutions");
    ss2->setPointLabelsFormat("(@xPoint, @yPoint)");
    ss2->setPointLabelsVisible(true);

    auto ls = new QtCharts::QLineSeries();
    ls->append(solution.wedgeCoordinate, 0);
    ls->append(solution.wedgeCoordinate, solution.wedgeCoordinate);
    ls->append(0, solution.wedgeCoordinate);
    ls->setName("'wedge'");

    cv->addSeries(ls);
    cv->addSeries(ss2);
    cv->addSeries(ss1);
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

void fillMinMaxCriteriaSolutionLog(QTextBrowser* b, std::vector<Point> points, RandomizedStrategyResult solution) {
    auto pairParentInSolution = [=](int parent1_index, int parent2_index) {
        QString s = "";

        auto p1 = points[parent1_index];
        auto p2 = points[parent2_index];

        s.append("Розв'язуємо наступне рівняння: <br>");
        s.append("x*l<sub>i1</sub> + (1-x)*<sub>j1</sub> = x*l<sub>i2</sub> + (1-x)*l<sub>j2</sub>: <br>");
        s.append("<br>");

        auto x_coeff = (p1.x - p2.x) - (p1.y - p2.y);
        auto f_coeff = p2.y - p2.x;
        auto x = f_coeff / x_coeff;

        s.append(QString("i=%1 => l<sub>%1</sub>=%2<br>").arg(n(parent1_index), ss(p1)));
        s.append(QString("j=%1 => l<sub>%1</sub>=%2<br>").arg(n(parent2_index), ss(p2)));
        s.append(QString("x*%1 + (1-x)*%2 = x*%3 + (1-x)*%4 <br>").arg(n(p1.x), n(p2.x), n(p1.y), n(p2.y)));
        s.append(QString("(%1-%2-%3+%4) * x = %4-%2 <br>").arg(n(p1.x), n(p2.x), n(p1.y), n(p2.y)));
        s.append(QString("%1*x = %2 <br>").arg(n(x_coeff), n(f_coeff)));
        s.append(QString("x = %2/%1 = %3 <br>").arg(n(x_coeff), n(f_coeff), n(x)));
        s.append("<br>");

        s.append(QString("Отже, l<sub>%1</sub>=%2 має бути обрана із вірогідністю %3, тоді як l<sub>%4</sub>=%5 має бути обраною із вірогідністю %6.")
                 .arg(n(parent1_index + 1), ss(p1), n(x),
                      n(parent2_index + 1), ss(p2), n(1-x)));
        b->setHtml(s);
    };
    auto singleParentInSolution = [=](int parent) {
        QString s = "";
        s.append("Клин перетнувся із нерандомізованим розв'язком.<br>");
        s.append(QString("Отже, l<sub>%1</sub>=%2 завжди має бути обрана.").arg(n(parent + 1), ss(points[parent])));
        b->setHtml(s);
    };

    if (solution.isPairParent()) {
        auto [p1, p2] = solution.pairParent();
        pairParentInSolution(p1, p2);
    } else {
        singleParentInSolution(solution.singleParent());
    }
}

void SolverWidget::on_button_backToPractice_clicked()
{
    this->mw_parent->widgetPopBack();
}

void SolverWidget::on_button_toTheory_clicked()
{
    this->mw_parent->stackWidget<TheoryWidget>(true);
}

void SolverWidget::on_button_toPractice_clicked()
{
    this->mw_parent->changeWidgetTo<PracticeWidget>();
}
