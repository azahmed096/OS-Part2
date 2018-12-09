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
  size_t current, count;
  int length;
  history_t history;

 public:
  Generator(size_t first, size_t count, int length)
      : current(first), count(count), length(length) {}
  std::vector<color_t> next();
  void add(masterResponse resp);
};

task debut_nb(int players_count, int tasks, int player_id);


std::vector<color_t> combi(size_t order,
                           uint length,
                           std::vector<color_t> alphabet);

size_t factorial(uint x);

size_t arangement(uint n, uint k);
std::ostream& operator<< (std::ostream& os, const task& obj);

#endif