// Copyright 2022 NNTU-CS
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include "tree.h"

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
}

int main() {
  example();
  std::cout << "=== Вычислительный эксперимент ===\n";
  experiment();
  return 0;
}
