#include <mpi.h>

#include <iostream>
#include <vector>

#include "mastermind/generation.hpp"
#include "mastermind/mastermind.hpp"

void print(std::vector<auto> x) {
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
  std::cout << "\n";
}

void master(int size) {
  int sendcnt = SPOTS;
  int sendBuff[SPOTS + 2];
  int recvcnt = SPOTS;
  int recvBuff[size * SPOTS];
  std::vector<color_t> solution(randomSolution(COLORS, SPOTS));
  guess_t sol = &solution[0];
  std::cout << "Solution is ";
  print(solution);
  std::cout << std::endl;

  bool finished = false;
  while (!finished) {
    MPI_Gather(&sendBuff, sendcnt, MPI_INT, &recvBuff, recvcnt, MPI_INT, 0,
               MPI_COMM_WORLD);
    std::cout << "Master received Guess : ";
    for (auto x = SPOTS; x < size * SPOTS; ++x)
      std::cout << recvBuff[x] << " ";
    std::cout << std::endl;

    evaluation best_eval{-1, -1};
    guess_t best = nullptr;

    for (int i = SPOTS; i < size * SPOTS; i += SPOTS) {
      auto current = recvBuff + i;

      if (*current == ERROR) {
        continue;
      }
      auto eval = evaluate(sol, current);
      if (eval > best_eval) {
        best_eval = eval;
        best = current;
      }
    }
    if (best == nullptr) {
      std::cout << "nullptr is the best" << std::endl;
    }
    finished = best_eval.perfect == SPOTS;
    masterResponse resp;
    resp.eval = best_eval;
    for (auto i = 0; i < SPOTS; ++i) {
      auto k = best[i];
      resp.colors[i] = best[i];
    }
    std::cout << "Best evaluation is " << best_eval.perfect << ","
              << best_eval.colourOnly << std::endl;

    MPI_Bcast(reinterpret_cast<int*>(&resp),
              sizeof(masterResponse) / sizeof(int), MPI_INT, 0, MPI_COMM_WORLD);
  }
  std::cout << "master finished ! " << std::endl;
}

task debut_nb(int players_count, int tasks, int player_id);

void player(int id, int size) {
  history_t hist;
  masterResponse resp;
  int sendcnt = SPOTS;
  int sendBuff[SPOTS];
  if (id == 1)
    std::cout << arangement(COLORS, SPOTS) << std::endl;
  auto t = debut_nb(size - 1, arangement(COLORS, SPOTS), id - 1);
  std::cout << "id:" << id << ' ' << t << std::endl;
  Generator gen(t.begin, t.count, SPOTS);

  while (resp.eval.perfect != SPOTS) {
    auto x = gen.next();
    int* k = sendBuff;
    for (auto it = x.cbegin(); it != x.cend(); ++it) {
      *k = *it;
      ++k;
    }
    MPI_Gather(&sendBuff, sendcnt, MPI_INT, nullptr, SPOTS, MPI_INT, 0,
               MPI_COMM_WORLD);
    MPI_Bcast(reinterpret_cast<int*>(&resp),
              sizeof(masterResponse) / sizeof(int), MPI_INT, 0, MPI_COMM_WORLD);
    gen.add(resp);
  }
  std::cout << '[' << id << "]cool finished !" << std::endl;
}


int main(int argc, char* argv[]) {
  std::vector<std::pair<guess_t, evaluation>> history;

  MPI_Init(&argc, &argv);
  int id, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (id == 0)
    master(size);
  else
    player(id, size);

  MPI_Finalize();
  return 0;
}
