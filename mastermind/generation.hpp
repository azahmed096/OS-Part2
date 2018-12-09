#ifndef GENERATION_HPP
#define GENERATION_HPP
#include <iostream>
#include <vector>
#include "mastermind.hpp"

#define ERROR -1

struct task {
  int begin;
  int count;
};


class Generator {
  int current, count, length;
  history_t history;

 public:
  Generator(int first, int count, int length)
      : current(first), count(count), length(length) {}
  std::vector<color_t> next();
  void add(masterResponse resp);
};

task debut_nb(int players_count, int tasks, int player_id);


std::vector<color_t> combi(int order,
                           int length,
                           std::vector<color_t> alphabet);

int factorial(int x);

int arangement(int n, int k);
std::ostream& operator<< (std::ostream& os, const task& obj);

#endif