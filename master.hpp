#ifndef MASTER_HPP
#define MASTER_HPP

#include "mastermind/mastermind.hpp"

void print(const std::vector<auto>& x);

class Master {
  std::vector<color_t> solution;
  int size;
  int sendBuff[SPOTS + 2];
  int* recvBuff;
  void print_solution();
  void print_recieved(guess_t);
  masterResponse find_best();
public:
  void run();
  Master(int player_count);
  ~Master();
};

#endif