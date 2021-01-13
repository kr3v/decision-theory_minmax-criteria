#ifndef DOUBLE_CMP_H
#define DOUBLE_CMP_H

#define EPS 1e-3

bool eq(double d1, double d2, double eps = EPS);
bool eq0(double d);

double roundTo(double d, int n);

#endif // DOUBLE_CMP_H
