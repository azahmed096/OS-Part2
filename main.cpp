#include <mpi.h>

#include <iostream>
#include <vector>

#include "mastermind/generation.hpp"
#include "mastermind/mastermind.hpp"
#include "master.hpp"
#include "player.hpp"

void master(int size) {
  Master m(size);
  m.run();
  std::cout << "Master finished !" << std::endl;
}

task debut_nb(int players_count, int tasks, int player_id);
 
void player(int id, int size) {
  Player player(id, size);
  player.run();
  std::cout << '[' << id << "] Finished !" << std::endl;
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
