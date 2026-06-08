// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include <vector>
#include <memory>
#include <algorithm>

class PMTree {
 private:
  struct N {
    char val;
    std::vector<std::shared_ptr<N>> children;
    explicit N(char v) : val(v) {}
  };
  std::shared_ptr<N> root;
  std::vector<char> elements;
  int totalPermutations;
  void buildTree(std::shared_ptr<N> node, std::vector<char> remaining);
  void getAllPermutationsDFS(std::shared_ptr<N> node, std::vector<char>& current,
                             std::vector<std::vector<char>>& result);
  int getSubtreePermutations(std::shared_ptr<N> node);

 public:
  explicit PMTree(const std::vector<char>& input);
  friend std::vector<std::vector<char>> getAllPerms(PMTree& tree);
  friend std::vector<char> getPerm1(PMTree& tree, int num);
  friend std::vector<char> getPerm2(PMTree& tree, int num);
  int getTotalPermutations() const { return totalPermutations; }
};

std::vector<std::vector<char>> getAllPerms(PMTree& tree);
std::vector<char> getPerm1(PMTree& tree, int num);
std::vector<char> getPerm2(PMTree& tree, int num);

#endif  // INCLUDE_TREE_H_
