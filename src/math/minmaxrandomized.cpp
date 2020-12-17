#include "minmaxrandomized.h"

RandomizedStrategyResult minMaxRandomized(std::vector<Point> m) {
    std::vector<RandomizedStrategyResult> candidates;
    for (size_t i = 0; i < m.size(); i++) {
        auto [x1, y1] = m[i];
        auto p1 = Point(x1, y1);
        for (size_t j = i + 1; j < m.size(); j++) {
            auto [x2, y2] = m[j];
            auto p2 = Point(x2, y2);

            // tests:
            // 1. xy-case (e.g. (1, 3) and (3, 1))
            // 2a. xy-minimum case (e.g. (1, 1) and (2, 2))
            // 2b. xy-minimum case v2 (e.g. (1, 2) and (2, 3))
            // 3a. 'top' one-point case (e.g. (1, 3) and (3, 4))
            // 3b. 'rgiht' one-point case (e.g. (3, 1) and (4, 3))
            auto t = xy(p1, p2);
            if (std::holds_alternative<std::string>(t)) {
                std::cerr << "i(" << i << ")-th matrix element is equal to j("<< j << ")-th? "
                          << "i-th = " << p1
                          << "j-th = " << p2
                          << "error: " << std::get<std::string>(t)
                          << std::endl;
            } else {
                if (auto p_o = std::get<std::optional<Point>>(t)) {
                    auto p = p_o.value();
                    if (p != p1 && p != p2) {
                        std::cout << "got a candidate: " << p << ", i=" << i << ", j=" << j << std::endl;
                        candidates.emplace_back(p, p.x, i, j);
                        continue;
                    }
                }
            }
        }
    }
    for (size_t i = 0; i < m.size(); i++) {
        candidates.emplace_back(m[i], std::max(m[i].x, m[i].y), i);
    }

    if (candidates.empty()) {
        throw "sorry";
    }

    auto c_p = std::min_element(
                candidates.begin(),
                candidates.end(),
                [](auto c1, auto c2) { return c1.wedgeCoordinate < c2.wedgeCoordinate; }
    );
    if (c_p == candidates.end())
        throw "wtf";
    return *c_p;
}

RandomizedStrategyResult::RandomizedStrategyResult(Point p, double result, int parent1, int parent2): result(p), wedgeCoordinate(result), parent(std::make_pair(parent1, parent2)) {}
RandomizedStrategyResult::RandomizedStrategyResult(Point p, double result, int parent): result(p), wedgeCoordinate(result), parent(parent) {}

int RandomizedStrategyResult::singleParent()
{
    return std::get<int>(parent);
}

bool RandomizedStrategyResult::isSingleParent()
{
    return std::holds_alternative<int>(parent);
}

std::pair<int, int> RandomizedStrategyResult::pairParent()
{
    return std::get<std::pair<int, int>>(parent);
}

std::vector<Point> RandomizedStrategyResult::parents(std::vector<Point> points)
{
    if (isPairParent()) {
        auto [pp1, pp2] = pairParent();
        return {points[pp1], points[pp2]};
    } else {
        return {points[singleParent()]};
    }
}

bool RandomizedStrategyResult::isPairParent()
{
    return std::holds_alternative<std::pair<int, int>>(parent);
}

double probability(Point p1, Point p2)
{
    auto x_coeff = (p1.x - p2.x) - (p1.y - p2.y);
    auto f_coeff = p2.y - p2.x;
    return f_coeff / x_coeff;
}
