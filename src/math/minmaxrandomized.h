#ifndef MINMAXRANDOMIZED_H
#define MINMAXRANDOMIZED_H

#include "geometry.h"

#include <iostream>
#include <vector>
#include <array>
#include <algorithm>

#include <variant>

struct RandomizedStrategyResult {
    Point result;
    double wedgeCoordinate;
    std::variant<int, std::pair<int, int>> parent;

    RandomizedStrategyResult(Point p, double result, int parent1, int parent2);
    RandomizedStrategyResult(Point p, double result, int parent);

    bool isPairParent();
    bool isSingleParent();

    int singleParent();
    std::pair<int, int> pairParent();

    std::vector<Point> parents(std::vector<Point> points);
};

RandomizedStrategyResult minMaxRandomized(std::vector<Point> m);

double probability(Point p1, Point p2);

#endif // MINMAXRANDOMIZED_H
