#include <mpi.h>
#include <iostream>

#include "master.hpp"
#include "mastermind/generation.hpp"

void print(const std::vector<auto>& x) {
  std::cout << "[";
  auto it = x.cbegin();
  if (x.size()) {
    std::cout << (*it);
    ++it;
    while (it != x.cend()) {
      std::cout << ',' << (*it);
      ++it;
    }
  }
  std::cout << "]";
}

void Master::print_solution() {
  std::cout << "[master] The solution is ";
  print(solution);
  std::cout << std::endl;
}

void Master::print_recieved(guess_t guess) {
  std::cout << guess[0];
  for (auto x = 1; x < SPOTS; ++x)
    std::cout << '\t' << guess[x];
}


void Master::run() {
  guess_t sol = &solution[0];
  bool finished = false;
  
/*
  // Uncommenting this will use the worst case solution
  solution.clear();
  int xx = COLORS - 1;
  while (solution.size() < SPOTS){
    solution.push_back(xx);
    --xx;
  }
*/
  print_solution();
  while (!finished) {
    evaluation best_eval{-1 /* perfect */, -1 /* colourOnly */};
    guess_t best = nullptr;

    MPI_Gather(&sendBuff, SPOTS /*sendcnt*/, MPI_INT, recvBuff,
               SPOTS /*recvcnt*/, MPI_INT, 0, MPI_COMM_WORLD);
    std::cout << "[master] recieved Guess:" << std::endl;

    for (int i = SPOTS; i < size * SPOTS; i += SPOTS) {
      auto current = recvBuff + i;
      print_recieved(current);

      if (*current == ERROR) { // recvBuff[4]
        std::cout << "\terror" << std::endl;
        continue;
      }
      auto eval = evaluate(sol, current);
      std::cout << '\t' << eval << std::endl;
      if (eval > best_eval) {
        best_eval = eval;
        best = current;
      }
    }
    if (best == nullptr) {
      std::cout << "Something wrong: nullptr is the best" 
        << std::endl << std::endl;
    }
    finished = best_eval.perfect == SPOTS;
    masterResponse resp;
    resp.eval = best_eval;
    for (auto i = 0; i < SPOTS; ++i) {
      resp.colors[i] = best[i];
    }
    std::cout << "Best evaluation is " << best_eval.perfect << ","
              << best_eval.colourOnly << std::endl << std::endl;

    MPI_Bcast(reinterpret_cast<int*>(&resp),
              sizeof(masterResponse) / sizeof(int), MPI_INT, 0, MPI_COMM_WORLD);
  }
}

Master::Master(int size)
    : solution(randomSolution(COLORS, SPOTS)),
      size(size),
      recvBuff(new int[size * SPOTS]) {}

Master::~Master() {
  delete[] recvBuff;
}