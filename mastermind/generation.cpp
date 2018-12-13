#include "generation.hpp"


/*
  Return the next plausible combinaison
  but if no plausible guess if found, the method return a
  vector containing a -1
*/
std::vector<color_t> Generator::next() {
    bool plaus = false;
    std::vector<color_t> result;
    while (!plaus && count > 0) {
      --count;
      std::vector<color_t> alphabet;
      for (auto i = 0; i < COLORS; ++i) {
        alphabet.push_back(i);
      }
      result = combi(current++, length, alphabet);
      plaus = plausible(&result[0], history);
    }
    if (plaus) {
      return result;
    }
    // does not need to have size equals to SPOTS
    // when the master will see the first is -1
    // it will recognize that it is an error
    // and the rest of the buffer have already "random" values..
    return std::vector<color_t>{ERROR};
  }

/* Adding of a plausible guess to the history */
void Generator::add(masterResponse resp) {history.push_back(resp); }

/* Method that return for each process the number of combinaison that 
it has to test and the starting index (order of the combinaison)

Nb : player count correspond to process id -1  */
task debut_nb(int players_count, int tasks, int player_id) {
  int d = tasks / players_count;
  int m = tasks % players_count;
  if (m == 0) {
    return {player_id * d, d};
  } else {
    if (player_id == players_count - 1) {
      return {tasks - m, m};
    }
    return {player_id * (d + 1), d + 1};
  }
}

/*
  Method that returns a specific combinaison (with order and without repetion)
  by taking in parameter the order of the desired combinaison
*/
std::vector<color_t> combi(size_t order,
                           uint length,
                           std::vector<color_t> alphabet) {
  if (length == 0) {
    return std::vector<color_t>();
  } else {
    auto n = alphabet.size();
    auto total = arangement(n, length);
    auto floor = total;
    int step = total / n;
    int letter = n;
    while (floor > order) {
      floor -= step;
      --letter;
    }
    color_t new_color = alphabet.at(letter);
    alphabet.erase(alphabet.begin() + letter);
    auto res = combi(order - floor, length - 1, alphabet);
    res.insert(res.begin(), new_color);
    return res;
  }
}


/* Factorial method */
size_t factorial(uint x) {
  size_t res = 1;
  for (auto i = 2u; i <= x; ++i) {
    res *= i;
  }
  return res;
}

/* Number of combinaison that will be generated */
size_t arangement(uint n, uint k) {
  return factorial(n) / factorial(n - k);
}

/*
  Operator for printing a task
*/
std::ostream& operator<< (std::ostream& os, const task& obj) {
    os << "task(begin=" << obj.begin << ",count=" << obj.count << ')';
    return os;
}
