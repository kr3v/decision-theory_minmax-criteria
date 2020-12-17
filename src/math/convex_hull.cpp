#include "convex_hull.h"

// https://e-maxx.ru/algo/convex_hull_graham

bool compare(Point a, Point b) {
    return (a.x < b.x) || (eq(a.x, b.x) && (a.y < b.y));
}

bool clockWise(Point a, Point b, Point c) {
    return a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y) < 0;
}

bool counterClockWise(Point a, Point b, Point c) {
    return a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y) > 0;
}

std::vector<Point> convexHull(std::vector<Point> a) {
    if (a.size() == 1) return a;
    std::sort(a.begin(), a.end(), &compare);
    Point p1 = a[0],  p2 = a.back();
    std::vector<Point> up, down;
    up.push_back(p1);
    down.push_back(p1);
    for (size_t i=1; i<a.size(); ++i) {
        if (i==a.size()-1 || clockWise (p1, a[i], p2)) {
            while (up.size()>=2 && !clockWise(up[up.size()-2], up[up.size()-1], a[i]))
                up.pop_back();
            up.push_back(a[i]);
        }
        if (i==a.size()-1 || counterClockWise (p1, a[i], p2)) {
            while (down.size()>=2 && !counterClockWise(down[down.size()-2], down[down.size()-1], a[i]))
                down.pop_back();
            down.push_back(a[i]);
        }
    }
    a.clear();

    std::vector<Point> result;
    for (size_t i=0; i<up.size(); ++i)
        result.push_back(up[i]);
    for (size_t i=down.size()-2; i>0; --i)
        result.push_back(down[i]);
    return result;
}
