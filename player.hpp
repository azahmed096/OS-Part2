#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "mastermind/generation.hpp"

class Player {
  int id, size;
  task player_task;
  Generator gen;
  int sendBuff[SPOTS];

  void print_work_range();

 public:
  Player(int id, int size);

  void run();
};

#endif