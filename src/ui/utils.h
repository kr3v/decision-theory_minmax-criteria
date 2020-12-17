#ifndef UTILS_H
#define UTILS_H

#include <QTableWidget>
#include <vector>
#include "src/math/geometry.h"

void setTableWidgetRowCount(QTableWidget* tw, int newRowCount);
void fillTableNames(QTableWidget* w);
void fillTableValues(QTableWidget* w, std::vector<Point> points);
std::vector<Point> extractFromTableWidget(QTableWidget* w);

#endif // UTILS_H
