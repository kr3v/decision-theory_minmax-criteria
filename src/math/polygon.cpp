#include "polygon.h"

#include <random>
#include <cmath>
#include <ctime>

#include "double_cmp.h"

double sign(int n) {
    if (n == 0) return -1;
    return 1;
}

std::vector<Point> generatePolygon(int n, double max_x, double max_y) {
    static std::mt19937 e2(std::time(nullptr));

    double m_x = max_x / 2.0,
           m_y = max_y / 2.0;
    double min_d_x = (max_x - m_x) * 0.3,
           min_d_y = (max_y - m_y) * 0.3;

    std::uniform_real_distribution<> dist_x(min_d_x, m_x);
    std::uniform_real_distribution<> dist_y(min_d_y, m_y);
    std::uniform_int_distribution<> dist_sign(0, 1);
    std::uniform_int_distribution<> dist_sign_filter(1, 10);

    std::vector<Point> generated;
    for (int i = 0; i < n; i++) {
        int s_x = sign(dist_sign(e2));
        int s_y = sign(dist_sign(e2));
        if (s_x == -1 && s_y == -1 && dist_sign_filter(e2) <= 2) {
            if (sign(dist_sign(e2))) {
                s_x = 1;
            } else {
                s_y = 1;
            }
        }

        double x = roundTo(m_x + s_x * dist_x(e2), 2);
        double y = roundTo(m_y + s_y * dist_y(e2), 2);
        generated.emplace_back(x, y);
    }
    return generated;
}
