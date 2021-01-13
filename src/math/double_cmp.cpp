#include "double_cmp.h"

#include <cmath>
#include <algorithm>

bool eq(double d1, double d2, double eps) {
    return std::abs(d1 - d2) < eps;
}

bool eq0(double d) {
    return std::abs(d) < EPS;
}

double roundTo(double d, int n) {
    return double(round(d * double(n))) / n;
}
