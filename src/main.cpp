// Copyright 2022 NNTU-CS
#include "tree.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

void example() {
  std::vector<char> in = {'1', '2', '3'};
  PMTree tree(in);

  auto perms = getAllPerms(tree);
  std::cout << "Все перестановки:\n";
  for (const auto& p : perms) {
    for (char c : p) std::cout << c;
    std::cout << " ";
  }
  std::cout << "\n\n";

  std::cout << "getPerm1(tree,1) = ";
  auto r1 = getPerm1(tree, 1);
  for (char c : r1) std::cout << c;
  std::cout << "\n";

  std::cout << "getPerm2(tree,2) = ";
  auto r2 = getPerm2(tree, 2);
  for (char c : r2) std::cout << c;
  std::cout << "\n";

  std::cout << "getPerm1(tree,6) = ";
  auto r6 = getPerm1(tree, 6);
  for (char c : r6) std::cout << c;
  std::cout << "\n\n";
}

void experiment() {
  const int MAX_N = 10;
  std::mt19937 rng(42);
  std::ofstream data("result/data.txt");
  data << "n time_getAll_ms time_getPerm2_ms\n";

  for (int n = 1; n <= MAX_N; ++n) {
    std::vector<char> symbols;
    for (int i = 0; i < n; ++i) symbols.push_back('a' + i);

    PMTree tree(symbols);
    size_t total = tree.totalPerms();
    std::uniform_int_distribution<size_t> dist(1, total);
    size_t randNum = dist(rng);

    auto start = std::chrono::high_resolution_clock::now();
    auto allPerms = getAllPerms(tree);
    auto end = std::chrono::high_resolution_clock::now();
    double timeAll =
        std::chrono::duration<double, std::milli>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    auto perm2 = getPerm2(tree, randNum);
    end = std::chrono::high_resolution_clock::now();
    double timePerm2 =
        std::chrono::duration<double, std::milli>(end - start).count();

    data << n << " " << timeAll << " " << timePerm2 << "\n";
    std::cout << "n=" << n << " готово\n";
    (void)allPerms;
    (void)perm2;
  }
  data.close();

  std::ofstream gp("result/plot.gp");
  gp << "set terminal png size 800,600\n";
  gp << "set output 'result/plot.png'\n";
  gp << "set logscale y\n";
  gp << "set xlabel 'n (размер алфавита)'\n";
  gp << "set ylabel 'Время (мс)'\n";
  gp << "set title 'Зависимость времени генерации перестановок'\n";
  gp << "plot 'result/data.txt' using 1:2 with linespoints "
        "title 'getAllPerms', \\\n";
  gp << "     'result/data.txt' using 1:3 with linespoints "
        "title 'getPerm2'\n";
  gp.close();

  [[maybe_unused]] int ret = std::system("gnuplot result/plot.gp");
}

int main() {
  example();
  std::cout << "=== Вычислительный эксперимент ===\n";
  experiment();
  return 0;
}

 
