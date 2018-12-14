#include <mpi.h>
#include "player.hpp"

void Player::print_work_range() {
  std::cout << '[' << id << ']' << player_task << std::endl;
}

Player::Player(int id, int size)
    : id(id),
      size(size),
      player_task(debut_nb(size - 1, arangement(COLORS, SPOTS), id - 1)),
      gen(player_task.begin, player_task.count, SPOTS) {}

void Player::run() {
  masterResponse resp {{0, 0}, {0}};

  print_work_range();

  while (resp.eval.perfect != SPOTS) {
    auto x = gen.next();
    int* k = sendBuff;
    for (auto it = x.cbegin(); it != x.cend(); ++it) {
      *k = *it;
      ++k;
    }
    MPI_Gather(&sendBuff, SPOTS, MPI_INT, nullptr, SPOTS, MPI_INT, 0,
               MPI_COMM_WORLD);
    MPI_Bcast(reinterpret_cast<int*>(&resp),
              sizeof(masterResponse) / sizeof(int), MPI_INT, 0, MPI_COMM_WORLD);
    gen.add(resp);
  }
}