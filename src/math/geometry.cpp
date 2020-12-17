#include "geometry.h"
#include <algorithm>

#include "double_cmp.h"

Point::Point(double x, double y) : x(x), y(y) {}
Line::Line(double a, double b, double c) : a(a), b(b), c(c) {}
Segment::Segment(Point p1, Point p2, Line l) : p1(p1), p2(p2), l(l) {}

bool rectangleContains(Point p1, Point p2, Point p) {
    double mn_x = std::min(p1.x, p2.x);
    double mx_x = std::max(p1.x, p2.x);
    double mn_y = std::min(p1.y, p2.y);
    double mx_y = std::max(p1.y, p2.y);

    return mn_x <= p.x && p.x <= mx_x && mn_y <= p.y && p.y <= mx_y;
}

bool Line::contains(Point p) { return eq0(a * p.x + b * p.y + c); }
bool Segment::contains(Point p) { return l.contains(p) && rectangleContains(p1, p2, p); }

///

std::variant<Line, std::string> LineSafe(Point p1, Point p2) {
    double a = p1.y - p2.y;
    double b = p2.x - p1.x;
    double c = p1.x * p2.y - p2.x * p1.y;

    if (eq0(a)) { a = 0; }
    if (eq0(b)) { b = 0; }
    if (eq0(c)) { c = 0; }
    if (a == 0 && b == 0) return "p1 == p2";
    else return Line(a, b, c);
}

std::variant<Segment, std::string> SegmentSafe(Point p1, Point p2) {
    auto l = LineSafe(p1, p2);
    if (std::holds_alternative<std::string>(l)) return std::get<std::string>(l);
    else return Segment(p1, p2, std::get<Line>(l));
}

///

std::variant<std::optional<Point>, std::string> xy(Point p1, Point p2) {
    auto s_v = SegmentSafe(p1, p2);
    if (std::holds_alternative<std::string>(s_v)) return std::get<std::string>(s_v);

    auto s = std::get<Segment>(s_v);

    auto ab = s.l.a + s.l.b;
    if (eq0(ab)) {
        return std::nullopt;
    }

    auto xy = -s.l.c / ab;
    auto p = Point(xy, xy);
    if (s.contains(p)) return p;
    else return std::nullopt;
}

std::ostream &operator<<(std::ostream &os, Point p) {
    return os << "(x=" << p.x << ", y=" << p.y << ")";
}

bool operator==(Point lhs, Point rhs)
{
    return eq(lhs.x, rhs.x) && eq(lhs.y, rhs.y);
}

bool operator!=(Point lhs, Point rhs)
{
    return !eq(lhs.x, rhs.x) && !eq(lhs.y, rhs.y);
}
