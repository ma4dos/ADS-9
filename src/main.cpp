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
  for (char c : r1) std::cout << c
