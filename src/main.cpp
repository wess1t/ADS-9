// Copyright 2022 NNTU-CS
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "tree.h"

template<typename Func>
double measureTime(Func func) {
  auto s = std::chrono::high_resolution_clock::now();
  func();
  auto e = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::microseconds>(e - s).count()
         / 1000.0;
}

std::vector<char> generateInput(int n) {
  std::vector<char> res;
  for (int i = 0; i < n; i++) {
    res.push_back('0' + (i % 10));
  }
  return res;
}

int64_t factorial(int n) {
  int64_t res = 1;
  for (int i = 2; i <= n; i++) {
    res *= i;
  }
  return res;
}

void runExperiment() {
  std::random_device rd;
  std::mt19937 gen(rd());

  std::cout << "\nExperimental Results\n";
  std::cout << std::string(80, '=') << "\n";
  std::cout << std::setw(4) << "n"
            << std::setw(15) << "n!"
            << std::setw(20) << "getAllPerms(ms)"
            << std::setw(20) << "getPerm1(ms)"
            << std::setw(20) << "getPerm2(ms)" << "\n";
  std::cout << std::string(80, '-') << "\n";

  for (int n = 1; n <= 10; n++) {
    std::vector<char> input = generateInput(n);

    PMTree tree(input);
    int64_t fact = factorial(n);

    std::uniform_int_distribution<> dis(1, tree.getTotalPermutations());
    int randomNum = dis(gen);

    double getAllPermsTime = 0;
    std::vector<std::vector<char>> allPerms;

    if (n <= 8) {
      getAllPermsTime = measureTime([&]() {
        allPerms = getAllPerms(tree);
      });
    } else {
      getAllPermsTime = -1;
    }

    double getPerm1Time = 0;
    if (n <= 8) {
      getPerm1Time = measureTime([&]() {
        std::vector<char> perm = getPerm1(tree, randomNum);
      });
    } else {
      getPerm1Time = -1;
    }

    double getPerm2Time = measureTime([&]() {
      std::vector<char> perm = getPerm2(tree, randomNum);
    });

    std::cout << std::setw(4) << n
              << std::setw(15) << fact;

    if (getAllPermsTime >= 0) {
      std::cout << std::setw(20) << std::fixed << std::setprecision(3)
                << getAllPermsTime;
    } else {
      std::cout << std::setw(20) << "N/A";
    }

    if (getPerm1Time >= 0) {
      std::cout << std::setw(20) << std::fixed << std::setprecision(3)
                << getPerm1Time;
    } else {
      std::cout << std::setw(20) << "N/A";
    }

    std::cout << std::setw(20) << std::fixed << std::setprecision(3)
              << getPerm2Time << "\n";
  }

  std::cout << std::string(80, '=') << "\n";
}

void demonstrate() {
  std::cout << "\nFunction Demonstration\n";
  std::cout << std::string(50, '=') << "\n\n";

  std::vector<char> in1 = {'1', '2', '3'};
  PMTree tree1(in1);

  std::cout << "Input symbols: ";
  for (char c : in1) std::cout << c << " ";
  std::cout << "\nTotal permutations: " << tree1.getTotalPermutations()
            << std::endl;

  std::vector<std::vector<char>> allPerms = getAllPerms(tree1);
  std::cout << "\nAll permutations:" << std::endl;
  for (size_t i = 0; i < allPerms.size(); i++) {
    std::cout << "  " << i + 1 << ": ";
    for (char c : allPerms[i]) std::cout << c;
    std::cout << std::endl;
  }

  std::cout << "\nTesting getPerm1 and getPerm2:" << std::endl;
  std::cout << std::string(50, '-') << std::endl;
  for (int num = 1; num <= 6; num++) {
    std::vector<char> perm1 = getPerm1(tree1, num);
    std::vector<char> perm2 = getPerm2(tree1, num);

    std::cout << "Permutation #" << num << ": ";
    std::cout << "getPerm1 = ";
    for (char c : perm1) std::cout << c;
    std::cout << ", getPerm2 = ";
    for (char c : perm2) std::cout << c;
    std::cout << std::endl;
  }

  std::cout << "\nTest for non-existent permutation (#10): ";
  std::vector<char> invalidPerm = getPerm1(tree1, 10);
  if (invalidPerm.empty()) {
    std::cout << "empty vector (correct handling)" << std::endl;
  }

  std::cout << "\nExample with set {1,3,5,7}\n";
  std::cout << std::string(50, '=') << "\n" << std::endl;
  std::vector<char> in2 = {'1', '3', '5', '7'};
  PMTree tree2(in2);

  std::cout << "First 8 permutations:" << std::endl;
  for (int num = 1; num <= 8; num++) {
    std::vector<char> perm = getPerm2(tree2, num);
    std::cout << "  #" << num << ": ";
    for (char c : perm) std::cout << c;
    std::cout << std::endl;
  }

  std::cout << "\nTotal permutations for {1,3,5,7}: "
            << tree2.getTotalPermutations() << std::endl;
}

int main() {
  demonstrate();
  runExperiment();
  return 0;
}
