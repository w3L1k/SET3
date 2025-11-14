#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <random>

struct Circle
{
    double x, y, r;
    double r_squared;
};

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    
    std::vector<Circle> circles(3);
    for (int i = 0; i < 3; ++i)
    {
        std::cin >> circles[i].x >> circles[i].y >> circles[i].r;
        circles[i].r_squared = circles[i].r * circles[i].r;
    }

    double min_x = circles[0].x - circles[0].r;
    double max_x = circles[0].x + circles[0].r;
    double min_y = circles[0].y - circles[0].r;
    double max_y = circles[0].y + circles[0].r;

    for (int i = 1; i < 3; ++i)
    {
        min_x = std::max(min_x, circles[i].x - circles[i].r);
        max_x = std::min(max_x, circles[i].x + circles[i].r);
        min_y = std::max(min_y, circles[i].y - circles[i].r);
        max_y = std::min(max_y, circles[i].y + circles[i].r);
    }

    if (min_x >= max_x || min_y >= max_y)
    {
        std::cout << std::fixed << std::setprecision(15) << 0.0 << std::endl;
        return 0;
    }

    double box_width = max_x - min_x;
    double box_height = max_y - min_y;
    double box_area = box_width * box_height;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    const long long num_samples = 10000000;
    long long hits = 0;

    for (long long i = 0; i < num_samples; ++i)
    {
        double rand_x_01 = dis(gen);
        double rand_y_01 = dis(gen);

        double x = min_x + rand_x_01 * box_width;
        double y = min_y + rand_y_01 * box_height;

        bool inside_all = true;
        for (int j = 0; j < 3; ++j)
        {
            double dx = x - circles[j].x;
            double dy = y - circles[j].y;
            double dist_squared = dx * dx + dy * dy;

            if (dist_squared > circles[j].r_squared)
            {
                inside_all = false;
                break;
            }
        }

        if (inside_all)
        {
            hits++;
        }
    }

    double estimated_area = box_area * (static_cast<double>(hits) / num_samples);

    std::cout << std::fixed << std::setprecision(15) << estimated_area << std::endl;

    return 0;
}