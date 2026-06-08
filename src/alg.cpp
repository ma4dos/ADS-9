// Copyright 2022 NNTU-CS
#include <iostream>
#include <fstream>
#include <locale>
#include <cstdlib>
#include <algorithm>
#include "tree.h"

void PMTree::build(Node* node, std::vector<char> remaining) {
    // Если передавать нечего — листовой узел, считаем его за одну перестановку
    if (remaining.empty()) {
        node->permCount = 1;
        return;
    }
 
    // Сортируем остаток, чтобы потомки шли по возрастанию
    std::sort(remaining.begin(), remaining.end());
 
    node->permCount = 0;
    for (size_t i = 0; i < remaining.size(); ++i) {
        Node* child = new Node(remaining[i]);
 
        // Формируем вектор без i-го элемента
        std::vector<char> next;
        next.reserve(remaining.size() - 1);
        for (size_t j = 0; j < remaining.size(); ++j) {
            if (j != i) next.push_back(remaining[j]);
        }
 
        build(child, next);
        node->permCount += child->permCount;
        node->children.push_back(child);
    }
}
 
PMTree::PMTree(const std::vector<char>& symbols) {
    root = new Node('\0');   // корень без символа
    build(root, symbols);
}
 
PMTree::~PMTree() {
    delete root;
}
 
// ─────────────────────────────────────────────
//  getAllPerms — обход дерева, сбор всех путей
// ─────────────────────────────────────────────
 
// Вспомогательная рекурсия: накапливаем текущий путь path,
// при достижении листа добавляем его в result.
static void dfs(const PMTree::Node* node,
                std::vector<char>& path,
                std::vector<std::vector<char>>& result) {
    if (node->children.empty()) {
        // leaf — путь готов
        result.push_back(path);
        return;
    }
    for (const PMTree::Node* child : node->children) {
        path.push_back(child->symbol);
        dfs(child, path, result);
        path.pop_back();
    }
}
 
std::vector<std::vector<char>> getAllPerms(const PMTree& tree) {
    std::vector<std::vector<char>> result;
    if (!tree.getRoot()) return result;
 
    std::vector<char> path;
    dfs(tree.getRoot(), path, result);
    return result;
}
 
// ─────────────────────────────────────────────
//  getPerm1 — наивный способ (через getAllPerms)
// ─────────────────────────────────────────────
 
// Получаем все перестановки и возвращаем num-ю (нумерация с 1).
// Если num выходит за границы — пустой вектор.
std::vector<char> getPerm1(const PMTree& tree, int num) {
    if (num <= 0) return {};
    auto all = getAllPerms(tree);
    if (static_cast<size_t>(num) > all.size()) return {};
    return all[num - 1];
}
 
// ─────────────────────────────────────────────
//  getPerm2 — быстрая навигация по дереву
// ─────────────────────────────────────────────
 
// В каждом узле у каждого потомка хранится permCount —
// сколько перестановок «живёт» в его поддереве.
// Идём вглубь, вычитая счётчики, пока не найдём нужный путь.
std::vector<char> getPerm2(const PMTree& tree, int num) {
    if (num <= 0) return {};
    const PMTree::Node* cur = tree.getRoot();
    if (!cur || static_cast<size_t>(num) > cur->permCount) return {};
 
    std::vector<char> result;
    size_t remaining = static_cast<size_t>(num);
 
    while (!cur->children.empty()) {
        bool found = false;
        for (const PMTree::Node* child : cur->children) {
            if (remaining <= child->permCount) {
                result.push_back(child->symbol);
                cur = child;
                found = true;
                break;
            }
            remaining -= child->permCount;
        }
        if (!found) return {};  // не должно случиться
    }
    return result;
}
