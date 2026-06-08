// Copyright 2022 NNTU-CS
#include <iostream>
#include <fstream>
#include <locale>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include "tree.h"

void PMTree::build(Node* node, std::vector<char> remaining) {
    if (remaining.empty()) {
        node->permCount = 1;
        return;
    }

    std::sort(remaining.begin(), remaining.end());

    node->permCount = 0;
    for (size_t i = 0; i < remaining.size(); ++i) {
        Node* child = new Node(remaining[i]);

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

static void dfs(const PMTree::Node* node,
                std::vector<char>& path,
                std::vector<std::vector<char>>& result) {
    if (node->children.empty()) {
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

std::vector<char> getPerm1(const PMTree& tree, int num) {
    if (num <= 0) return {};
    auto all = getAllPerms(tree);
    if (static_cast<size_t>(num) > all.size()) return {};
    return all[num - 1];
}

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
        if (!found) return {};
    }
    return result;
}
