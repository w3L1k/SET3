#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <random>
#include <algorithm>

// Определение M_PI для точности
#define M_PI 3.14159265358979323846

struct Circle
{
    double x, y, r;
    double r_squared;
};

struct BoundingBox
{
    double min_x, max_x, min_y, max_y;
    double area;
};

// Глобальные константы кругов
const std::vector<Circle> GLOBAL_CIRCLES = {
    {1.0, 1.0, 1.0, 1.0 * 1.0},                                       // C1: (1, 1), r=1
    {1.5, 2.0, std::sqrt(5.0) / 2.0, 5.0 / 4.0},                       // C2: (1.5, 2), r=sqrt(5)/2
    {2.0, 1.5, std::sqrt(5.0) / 2.0, 5.0 / 4.0}                        // C3: (2, 1.5), r=sqrt(5)/2
};

// Точное значение площади (S_true)
const double S_TRUE = 0.25 * M_PI + 1.25 * std::asin(0.8) - 1.0;

// Функция для выполнения оценки Монте-Карло
double calculate_monte_carlo(long long num_samples, const BoundingBox& box)
{
    // Используем static генератор для эффективности (один раз инициализируется)
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<double> dis(0.0, 1.0);

    long long hits = 0;
    
    double box_width = box.max_x - box.min_x;
    double box_height = box.max_y - box.min_y;

    for (long long i = 0; i < num_samples; ++i)
    {
        double rand_x_01 = dis(gen);
        double rand_y_01 = dis(gen);

        // Генерация точки внутри указанного BoundingBox
        double x = box.min_x + rand_x_01 * box_width;
        double y = box.min_y + rand_y_01 * box_height;

        bool inside_all = true;
        for (const auto& circle : GLOBAL_CIRCLES)
        {
            double dx = x - circle.x;
            double dy = y - circle.y;
            double dist_squared = dx * dx + dy * dy;

            if (dist_squared > circle.r_squared)
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

    return box.area * (static_cast<double>(hits) / num_samples);
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    
    std::cout << std::fixed << std::setprecision(15);
    
    // 1. Определение "Узкой" области (Narrow Box) - ИСПОЛЬЗУЕМ ВАШИ ЗНАЧЕНИЯ
    BoundingBox narrow_box;
    narrow_box.min_x = 0.8;
    narrow_box.max_x = 2.1;
    narrow_box.min_y = 0.8;
    narrow_box.max_y = 2.2;
    narrow_box.area = (narrow_box.max_x - narrow_box.min_x) * (narrow_box.max_y - narrow_box.min_y);
    
    // 2. Определение "Широкой" области (Wide Box) - ИСПОЛЬЗУЕМ ВАШИ ЗНАЧЕНИЯ
    BoundingBox wide_box;
    wide_box.min_x = 0.0;
    wide_box.max_x = 3.5;
    wide_box.min_y = 0.0;
    wide_box.max_y = 3.5;
    wide_box.area = (wide_box.max_x - wide_box.min_x) * (wide_box.max_y - wide_box.min_y);

    // Вывод заголовка CSV-файла
    std::cout << "N,Area_Narrow,Error_Narrow,Area_Wide,Error_Wide" << std::endl;

    // 3. Запуск экспериментального цикла
    for (long long N = 100; N <= 100000; N += 500)
    {
        // Оценка для узкой коробки
        double area_narrow = calculate_monte_carlo(N, narrow_box);
        double error_narrow = std::abs(area_narrow - S_TRUE) / S_TRUE;

        // Оценка для широкой коробки
        double area_wide = calculate_monte_carlo(N, wide_box);
        double error_wide = std::abs(area_wide - S_TRUE) / S_TRUE;

        // Вывод данных
        std::cout << N << ","
                  << area_narrow << ","
                  << error_narrow << ","
                  << area_wide << ","
                  << error_wide << std::endl;
    }

    return 0;
}