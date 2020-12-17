#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QChartView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    template <typename T>
    void changeWidgetTo() {
        auto c = ui->centralwidget;
        auto w = new T(c, this);
        auto g = geometry();
        auto w_g = w->geometry();

        c->removeWidget(c->currentWidget());
        c->addWidget(w);
        setGeometry(g.x(), g.y(), w_g.width(), w_g.height());
    }

    template <typename T>
    void stackWidget() {
        stackWidget(new T(ui->centralwidget, this));
    }

    template <typename T, typename Arg>
    void stackWidget(Arg a) {
        stackWidget(new T(ui->centralwidget, this, a));
    }

    void stackWidget(QWidget* w) {
        auto c = ui->centralwidget;
        auto g = geometry();
        auto w_g = w->geometry();

        c->addWidget(w);
        c->setCurrentWidget(w);

        setGeometry(g.x(), g.y(), w_g.width(), w_g.height());
    }

    void widgetPopBack() {
        auto c = ui->centralwidget;
        auto i = c->currentIndex();
        auto g = geometry();
        auto w_g = c->widget(i - 1)->geometry();

        c->removeWidget(c->widget(i));
        c->setCurrentIndex(i - 1);

        setGeometry(g.x(), g.y(), w_g.width(), w_g.height());
    }
private:
    Ui::MainWindow *ui;

    void updateChartView();
};
#endif // MAINWINDOW_H
