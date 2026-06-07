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
