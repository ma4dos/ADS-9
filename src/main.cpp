// Copyright 2022 NNTU-CS
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <fstream>
#include <numeric>
#include "tree.h"
 
// ─── вспомогательные утилиты ───────────────────────────
 
// Печатает вектор символов как строку
static void printPerm(const std::vector<char>& p) {
    for (char c : p) std::cout << c;
    std::cout << '\n';
}
 
 
// ─── демонстрационные примеры ───────────────────────────
 
static void demo() {
    std::cout << "=== Demo: {1,2,3} ===\n";
    std::vector<char> in = {'1', '2', '3'};
    PMTree tree(in);
 
    // Все перестановки
    auto all = getAllPerms(tree);
    std::cout << "getAllPerms (" << all.size() << " total):\n";
    for (const auto& p : all) printPerm(p);
 
    // По номеру — способ 1
    std::cout << "\ngetPerm1(tree, 1) = "; printPerm(getPerm1(tree, 1));
    std::cout << "getPerm1(tree, 2) = "; printPerm(getPerm1(tree, 2));
    std::cout << "getPerm1(tree, 6) = "; printPerm(getPerm1(tree, 6));
    std::cout << "getPerm1(tree, 7) = ";
    auto bad = getPerm1(tree, 7);
    std::cout << (bad.empty() ? "(empty)" : "") << '\n';
 
    // По номеру — способ 2
    std::cout << "\ngetPerm2(tree, 1) = "; printPerm(getPerm2(tree, 1));
    std::cout << "getPerm2(tree, 2) = "; printPerm(getPerm2(tree, 2));
    std::cout << "getPerm2(tree, 6) = "; printPerm(getPerm2(tree, 6));
 
    std::cout << "\n=== Demo: {1,3,5,7} ===\n";
    std::vector<char> in2 = {'1', '3', '5', '7'};
    PMTree tree2(in2);
    std::cout << "Perm #1:  "; printPerm(getPerm2(tree2, 1));
    std::cout << "Perm #2:  "; printPerm(getPerm2(tree2, 2));
    std::cout << "Perm #24: "; printPerm(getPerm2(tree2, 24));
}
 
// ─── вычислительный эксперимент ────────────────────────
 
// Измеряет среднее время одного вызова функции (мкс)
// повторяя её calls раз.
template <typename Func>
static double measureMicros(Func f, int calls) {
    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < calls; ++i) f();
    auto t1 = std::chrono::high_resolution_clock::now();
    double us = std::chrono::duration<double, std::micro>(t1 - t0).count();
    return us / calls;
}
 
static void experiment() {
    // Алфавит: последовательные символы '1','2',...
    // Тестируем n = 2 .. 8 (8! = 40320 перестановок)
    const int N_MIN = 2, N_MAX = 8;
 
    std::mt19937 rng(42);
 
    // Данные для CSV (потом рисуем график python-скриптом)
    std::ofstream csv("result/timings.csv");
    csv << "n,getAllPerms_us,getPerm1_us,getPerm2_us\n";
 
    std::cout << "\n=== Experiment ===\n";
    std::cout << "n\tgetAllPerms(us)\tgetPerm1(us)\tgetPerm2(us)\n";
 
    for (int n = N_MIN; n <= N_MAX; ++n) {
        // Строим алфавит из n символов
        std::vector<char> alpha(n);
        std::iota(alpha.begin(), alpha.end(), '1');  // '1','2',...
 
        PMTree tree(alpha);
        size_t total = tree.totalPerms();  // n!
 
        // Случайный номер перестановки в [1, total]
        std::uniform_int_distribution<size_t> dist(1, total);
        int perm_num = static_cast<int>(dist(rng));
 
        // Количество повторов: меньше при больших n
        int reps = std::max(1, static_cast<int>(200 / total) + 3);
 
        double t_all = measureMicros([&]() {
            volatile auto r = getAllPerms(tree);
            (void)r;
        }, reps);
 
        double t1 = measureMicros([&]() {
            volatile auto r = getPerm1(tree, perm_num);
            (void)r;
        }, reps);
 
        double t2 = measureMicros([&]() {
            volatile auto r = getPerm2(tree, perm_num);
            (void)r;
        }, reps);
 
        std::cout << n << '\t'
                  << t_all << '\t'
                  << t1    << '\t'
                  << t2    << '\n';
 
        csv << n << ',' << t_all << ',' << t1 << ',' << t2 << '\n';
    }
    csv.close();
    std::cout << "Timings saved to result/timings.csv\n";
}
 
// ─── построение графика через Python ───────────────────
 
static void buildPlot() {
    const char* py = R"python(
import csv, math, sys
try:
    import matplotlib
    matplotlib.use('Agg')
    import matplotlib.pyplot as plt
except ImportError:
    sys.exit(0)
 
ns, t_all, t1, t2 = [], [], [], []
with open('result/timings.csv') as f:
    reader = csv.DictReader(f)
    for row in reader:
        ns.append(int(row['n']))
        t_all.append(float(row['getAllPerms_us']))
        t1.append(float(row['getPerm1_us']))
        t2.append(float(row['getPerm2_us']))
 
fig, ax = plt.subplots(figsize=(8, 5))
ax.semilogy(ns, t_all, 'o-', label='getAllPerms')
ax.semilogy(ns, t1,    's-', label='getPerm1 (naive)')
ax.semilogy(ns, t2,    '^-', label='getPerm2 (tree nav)')
ax.set_xlabel('Alphabet size n')
ax.set_ylabel('Time per call (µs, log scale)')
ax.set_title('Permutation functions: time vs alphabet size')
ax.legend()
ax.grid(True, which='both', linestyle='--', alpha=0.5)
plt.tight_layout()
plt.savefig('result/plot.png', dpi=150)
print('Plot saved to result/plot.png')
)python";
 
    std::ofstream f("result/plot.py");
    f << py;
    f.close();
    int ret = std::system("python3 result/plot.py");
    (void)ret;
}
 
// ───────────────────────────────────────────────────────
 
int main() {
    demo();
    experiment();
    buildPlot();
    return 0;
}
