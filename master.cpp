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


/*
  Return the best evaluation with its corresponding
  guess (in a MasterResponse struct)

  The criteria for choosing the best struct
  is defined in the operator <.
*/
masterResponse Master::find_best() {
  guess_t sol = &solution[0];
  evaluation best_eval{-1 /* perfect */, -1 /* colourOnly */};;
  guess_t best = nullptr;
  for (int i = SPOTS; i < size * SPOTS; i += SPOTS) {
    auto current = recvBuff + i;
    print_recieved(current);

    if (*current == ERROR) {  // recvBuff[4]
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
    std::cout << "!Something went wrong all players send errors" << std::endl
              << std::endl;
  }
  masterResponse resp;
  resp.eval = best_eval;
  for (auto i = 0; i < SPOTS; ++i) {
    resp.colors[i] = best[i];
  }
  return resp;
}

void Master::run() {
  bool finished = false;
  std::cout << "[master] number of arangements:" << arangement(COLORS, SPOTS) << std::endl;
  /*
    // Uncommenting this will use the worst case solution
    // useful to evaluate the maximum running time for a certain
    // color or spot
    solution.clear();
    int xx = COLORS - 1;
    while (solution.size() < SPOTS){
      solution.push_back(xx);
      --xx;
    }
  */
  print_solution();
  while (!finished) {
    MPI_Gather(&sendBuff, SPOTS /*sendcnt*/, MPI_INT, recvBuff,
               SPOTS /*recvcnt*/, MPI_INT, 0, MPI_COMM_WORLD);
    std::cout << "[master] recieved Guess:" << std::endl;

    auto best_eval = find_best();

    finished = best_eval.eval.perfect == SPOTS;
    std::cout << "Best evaluation is " << best_eval.eval
              << std::endl;

    MPI_Bcast(reinterpret_cast<int*>(&best_eval),
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