#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <optional>
#include <string>
#include <variant>

#include <ostream>

struct Point {
    double x, y;
    Point(double x, double y);
};

std::ostream& operator<<(std::ostream& os, Point p);
bool operator==(Point lhs, Point rhs);
bool operator!=(Point lhs, Point rhs);

struct Line {
    double a, b, c;
    Line(double a, double b, double c);

    bool contains(Point p);
};

struct Segment {
    Point p1, p2;
    Line l;

    Segment(Point p1, Point p2, Line l);

    bool contains(Point p);
};

bool rectangleContains(Point p1, Point p2, Point p);
std::variant<Line, std::string> LineSafe(Point p1, Point p2);
std::variant<Segment, std::string> SegmentSafe(Point p1, Point p2);

std::variant<std::optional<Point>, std::string> xy(Point p1, Point p2);

#endif // GEOMETRY_H
