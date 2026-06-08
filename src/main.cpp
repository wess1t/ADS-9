// Copyright 2022 NNTU-CS
#include "tree.h"
#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;
using namespace chrono;

template<typename Func>
double measureTime(Func func) {
    auto s = high_resolution_clock::now();
    func();
    auto e = high_resolution_clock::now();
    return duration_cast<microseconds>(e - s).count() / 1000.0;
}

vector<char> generateInput(int n) {
    vector<char> res;
    for (int i = 0; i < n; i++) {
        res.push_back('0' + (i % 10));
    }
    return res;
}

long long factorial(int n) {
    long long res = 1;
    for (int i = 2; i <= n; i++) {
        res *= i;
    }
    return res;
}

void runExperiment() {
    random_device rd;
    mt19937 gen(rd());
    
    cout << "\nExperimental Results\n";
    cout << string(80, '=') << "\n";
    cout << setw(4) << "n" 
         << setw(15) << "n!" 
         << setw(20) << "getAllPerms(ms)" 
         << setw(20) << "getPerm1(ms)" 
         << setw(20) << "getPerm2(ms)" << "\n";
    cout << string(80, '-') << "\n";
    
    for (int n = 1; n <= 10; n++) {
        vector<char> input = generateInput(n);
        
        PMTree tree(input);
        long long fact = factorial(n);
        
        uniform_int_distribution<> dis(1, tree.getTotalPermutations());
        int randomNum = dis(gen);
        
        double getAllPermsTime = 0;
        vector<vector<char>> allPerms;
        
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
                vector<char> perm = getPerm1(tree, randomNum);
            });
        } else {
            getPerm1Time = -1;
        }
        
        double getPerm2Time = measureTime([&]() {
            vector<char> perm = getPerm2(tree, randomNum);
        });
        
        cout << setw(4) << n 
             << setw(15) << fact;
        
        if (getAllPermsTime >= 0) {
            cout << setw(20) << fixed << setprecision(3) << getAllPermsTime;
        } else {
            cout << setw(20) << "N/A";
        }
        
        if (getPerm1Time >= 0) {
            cout << setw(20) << fixed << setprecision(3) << getPerm1Time;
        } else {
            cout << setw(20) << "N/A";
        }
        
        cout << setw(20) << fixed << setprecision(3) << getPerm2Time << "\n";
    }
    
    cout << string(80, '=') << "\n";
}

void demonstrate() {
    cout << "\nFunction Demonstration\n";
    cout << string(50, '=') << "\n\n";
    
    vector<char> in1 = {'1', '2', '3'};
    PMTree tree1(in1);
    
    cout << "Input symbols: ";
    for (char c : in1) cout << c << " ";
    cout << "\nTotal permutations: " << tree1.getTotalPermutations() << endl;
    
    vector<vector<char>> allPerms = getAllPerms(tree1);
    cout << "\nAll permutations:" << endl;
    for (size_t i = 0; i < allPerms.size(); i++) {
        cout << "  " << i + 1 << ": ";
        for (char c : allPerms[i]) cout << c;
        cout << endl;
    }
    
    cout << "\nTesting getPerm1 and getPerm2:" << endl;
    cout << string(50, '-') << endl;
    for (int num = 1; num <= 6; num++) {
        vector<char> perm1 = getPerm1(tree1, num);
        vector<char> perm2 = getPerm2(tree1, num);
        
        cout << "Permutation #" << num << ": ";
        cout << "getPerm1 = ";
        for (char c : perm1) cout << c;
        cout << ", getPerm2 = ";
        for (char c : perm2) cout << c;
        cout << endl;
    }
    
    cout << "\nTest for non-existent permutation (#10): ";
    vector<char> invalidPerm = getPerm1(tree1, 10);
    if (invalidPerm.empty()) {
        cout << "empty vector (correct handling)" << endl;
    }
    
    cout << "\nExample with set {1,3,5,7}\n";
    cout << string(50, '=') << "\n" << endl;
    vector<char> in2 = {'1', '3', '5', '7'};
    PMTree tree2(in2);
    
    cout << "First 8 permutations:" << endl;
    for (int num = 1; num <= 8; num++) {
        vector<char> perm = getPerm2(tree2, num);
        cout << "  #" << num << ": ";
        for (char c : perm) cout << c;
        cout << endl;
    }
    
    cout << "\nTotal permutations for {1,3,5,7}: " << tree2.getTotalPermutations() << endl;
}

int main() {
    demonstrate();
    runExperiment();
    return 0;
}
