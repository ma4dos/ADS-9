#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include <cstddef>
#include <vector>

class PMTree {
 public:
  struct Node {
    char symbol;
    std::vector<Node*> children;
    size_t permCount;
    explicit Node(char sym = '\0') : symbol(sym), permCount(0) {}
    ~Node() {
      for (Node* child : children) delete child;
    }
  };

  explicit PMTree(const std::vector<char>& symbols);
  ~PMTree();

  const Node* getRoot() const { return root; }
  size_t totalPerms() const { return root ? root->permCount : 0; }

 private:
  Node* root;
  void build(Node* node, std::vector<char> remaining);
};

std::vector<std::vector<char>> getAllPerms(const PMTree& tree);
std::vector<char> getPerm1(const PMTree& tree, int num);
std::vector<char> getPerm2(const PMTree& tree, int num);

#endif
