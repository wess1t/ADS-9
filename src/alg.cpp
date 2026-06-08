// Copyright 2022 NNTU-CS
#include <iostream>
#include <fstream>
#include <locale>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <vector>
#include <memory>
#include "tree.h"

PMTree::PMTree(const std::vector<char>& input) : elements(input) {
  root = std::make_shared<N>('\0');
  totalPermutations = 0;

  std::vector<char> sorted = elements;
  std::sort(sorted.begin(), sorted.end());

  for (char c : sorted) {
    auto child = std::make_shared<N>(c);
    root->children.push_back(child);
    std::vector<char> remaining;
    for (char elem : sorted) {
      if (elem != c) remaining.push_back(elem);
    }
    buildTree(child, remaining);
  }

  totalPermutations = 1;
  for (size_t i = 2; i <= elements.size(); i++) {
    totalPermutations *= i;
  }
}

void PMTree::buildTree(std::shared_ptr<N> node, std::vector<char> remaining) {
  if (remaining.empty()) return;

  std::sort(remaining.begin(), remaining.end());

  for (char c : remaining) {
    auto child = std::make_shared<N>(c);
    node->children.push_back(child);

    std::vector<char> newRemaining;
    for (char elem : remaining) {
      if (elem != c) newRemaining.push_back(elem);
    }
    buildTree(child, newRemaining);
  }
}

std::vector<std::vector<char>> getAllPerms(PMTree& tree) {
  std::vector<std::vector<char>> res;
  std::vector<char> current;

  for (auto& child : tree.root->children) {
    tree.getAllPermutationsDFS(child, current, res);
  }

  return res;
}

void PMTree::getAllPermutationsDFS(std::shared_ptr<N> node,
                                   std::vector<char>& current,
                                   std::vector<std::vector<char>>& res) {
  current.push_back(node->val);

  if (node->children.empty()) {
    res.push_back(current);
  } else {
    for (auto& child : node->children) {
      getAllPermutationsDFS(child, current, res);
    }
  }

  current.pop_back();
}

std::vector<char> getPerm1(PMTree& tree, int num) {
  if (num < 1 || num > tree.totalPermutations) {
    return std::vector<char>();
  }

  std::vector<std::vector<char>> allPerms = getAllPerms(tree);

  if (num <= static_cast<int>(allPerms.size())) {
    return allPerms[num - 1];
  }

  return std::vector<char>();
}

int PMTree::getSubtreePermutations(std::shared_ptr<N> node) {
  if (node->children.empty()) return 1;

  int res = 0;
  for (auto& child : node->children) {
    res += getSubtreePermutations(child);
  }
  return res;
}

std::vector<char> getPerm2(PMTree& tree, int num) {
  if (num < 1 || num > tree.totalPermutations) {
    return std::vector<char>();
  }

  std::vector<char> res;
  std::shared_ptr<PMTree::N> current = nullptr;
  int remainingNum = num;

  for (auto& child : tree.root->children) {
    int subtreeCount = tree.getSubtreePermutations(child);

    if (remainingNum <= subtreeCount) {
      current = child;
      res.push_back(current->val);
      break;
    } else {
      remainingNum -= subtreeCount;
    }
  }

  while (current && !current->children.empty()) {
    for (auto& child : current->children) {
      int subtreeCount = tree.getSubtreePermutations(child);

      if (remainingNum <= subtreeCount) {
        current = child;
        res.push_back(current->val);
        break;
      } else {
        remainingNum -= subtreeCount;
      }
    }
  }

  return res;
}
