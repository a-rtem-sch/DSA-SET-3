#include "cmath"
#include <bits/stdc++.h>
#include <numbers>
#include <filesystem>
#include <fstream>

struct Point {
  double x;
  double y;
};
struct Circle {
  double r;
  Point center;
};

double distance(Point a, Point b) {
  return sqrt(pow(std::abs(a.x - b.x), 2) + pow(std::abs(a.y - b.y), 2));
}

const Circle C1 = Circle(1, Point(1, 1));
const Circle C2 = Circle(sqrt(5) / 2, Point(1.5, 2));
const Circle C3 = Circle(sqrt(5) / 2, Point(2, 1.5));

const double AREA = 0.25*std::numbers::pi + 1.25*asin(0.8) - 1;

const unsigned SEED = 123456789;

std::mt19937 rng(SEED);

Point generate_point(Point bottom_left, Point top_right) {
  std::uniform_real_distribution<double> dist_x(bottom_left.x, top_right.x);
  std::uniform_real_distribution<double> dist_y(bottom_left.y, top_right.y);
  return { dist_x(rng), dist_y(rng) };
}

bool in_area(Point p) {
  bool in_c1 = (distance(p, C1.center) <= C1.r);
  bool in_c2 = (distance(p, C2.center) <= C2.r);
  bool in_c3 = (distance(p, C3.center) <= C3.r);
  return (in_c1 && in_c2 && in_c3);
}


double estimate_area(Point bottom_left, Point top_right, int N) {
  int hits = 0;
  for (int i = 0; i < N; ++i) {
    Point p = generate_point(bottom_left, top_right);
    if (in_area(p)) hits++;
  }

  double rect_area = (top_right.x - bottom_left.x) * (top_right.y - bottom_left.y);
  return (hits / static_cast<double>(N)) * rect_area;
}

void run_experiment() {
  std::ofstream out("results.csv");
  out << "N,area_type,estimated_area,relative_error\n";

  std::vector<std::pair<Point, Point>> boxes = {
      { {0.0, 0.0}, {3.25, 3.25} },
      { {0.85, 0.85}, {2.15, 2.15} }
  };
  std::vector<std::string> names = {"wide", "tight"};

  for (int bi = 0; bi < (int)boxes.size(); ++bi) {
    Point bl = boxes[bi].first;
    Point tr = boxes[bi].second;

    for (int N = 100; N <= 100000; N += 500) {
      double est = estimate_area(bl, tr, N);
      double rel_err = std::abs(est - AREA) / AREA;
      out << N << "," << names[bi] << "," << est << "," << rel_err << "\n";
    }
  }

  out.close();
  std::cout << "Saved";
}

int main() {
  run_experiment();
  std::cout << std::filesystem::current_path() << '\n';
  return 0;
}
