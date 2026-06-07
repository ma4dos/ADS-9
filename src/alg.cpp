// Copyright 2022 NNTU-CS
#include "tree.h"
#include <algorithm>
#include <vector>

PMTree::PMTree(const std::vector<char>& symbols) {
    std::vector<char> sorted = symbols;
    std::sort(sorted.begin(), sorted.end());
    root = new Node();
    build(root, sorted);
}

PMTree::~PMTree() {
    delete root;
}

void PMTree::build(Node* node, std::vector<char> remaining) {
    if (remaining.empty()) {
        node->permCount = 1;
        return;
    }
    for (char ch : remaining) {
        Node* child = new Node(ch);
        node->children.push_back(child);
        std::vector<char> next;
        for (char c : remaining)
            if (c != ch) next.push_back(c);
        build(child, next);
    }
    node->permCount = 0;
    for (Node* child : node->children)
        node->permCount += child->permCount;
}

static void dfs(const PMTree::Node* node, std::vector<char>& path,
                std::vector<std::vector<char>>& result, bool isRoot) {
    if (!isRoot) path.push_back(node->symbol);
    if (node->children.empty()) {
        result.push_back(path);
    } else {
        for (const PMTree::Node* child : node->children)
            dfs(child, path, result, false);
    }
    if (!isRoot) path.pop_back();
}

std::vector<std::vector<char>> getAllPerms(const PMTree& tree) {
    std::vector<std::vector<char>> result;
    if (!tree.getRoot()) return result;
    std::vector<char> path;
    dfs(tree.getRoot(), path, result, true);
    return result;
}

std::vector<char> getPerm1(const PMTree& tree, int num) {
    auto all = getAllPerms(tree);
    if (num >= 1 && static_cast<size_t>(num) <= all.size())
        return all[num - 1];
    return std::vector<char>();
}

std::vector<char> getPerm2(const PMTree& tree, int num) {
    if (num < 1 || static_cast<size_t>(num) > tree.totalPerms())
        return std::vector<char>();
    const PMTree::Node* cur = tree.getRoot();
    std::vector<char> path;
    while (!cur->children.empty()) {
        for (const PMTree::Node* child : cur->children) {
            if (static_cast<size_t>(num) <= child->permCount) {
                path.push_back(child->symbol);
                cur = child;
                break;
            } else {
                num -= child->permCount;
            }
        }
    }
    return path;
}
