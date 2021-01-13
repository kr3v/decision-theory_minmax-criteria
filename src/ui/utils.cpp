#include "utils.h"

void setTableWidgetRowCount(QTableWidget* tw, int newRowCount) {
    auto rowCount = tw->rowCount();
    if (newRowCount > rowCount) {
        for (int i = 1; i <= newRowCount - rowCount; i++) {
            auto rc = rowCount + i - 1;
            tw->insertRow(rc);
            tw->setItem(rc, 0, new QTableWidgetItem("0"));
            tw->setItem(rc, 1, new QTableWidgetItem("0"));
        }
    } else if (newRowCount < rowCount) {
        for (int i = 1; i <= rowCount - newRowCount; i++) {
            tw->removeRow(rowCount - i);
        }
    }
    fillTableNames(tw);
}

void fillTableNames(QTableWidget* w) {
    auto c = w->rowCount();
    QStringList l;
    for (int i = 1; i <= c; i++) {
        l.append(QString("l_%1_j").arg(i));
    }
    w->setVerticalHeaderLabels(l);
}

void fillTableValues(QTableWidget* w, std::vector<Point> points) {
    for (size_t i = 0; i < points.size(); i++) {
        auto p = points[i];
        w->item(i, 0)->setText(QString::number(p.x));
        w->item(i, 1)->setText(QString::number(p.y));
    }
}

std::vector<Point> extractFromTableWidget(QTableWidget* w) {
    std::vector<Point> result;
    for (int row = 0; row < w->rowCount(); row++) {
        auto x = w->item(row, 0)->text().replace(',', '.').toDouble();
        auto y = w->item(row, 1)->text().replace(',', '.').toDouble();
        result.push_back(Point(x, y));
    }
    return result;
}
