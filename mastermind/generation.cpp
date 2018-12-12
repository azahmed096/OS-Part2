#include "generation.hpp"


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
    return std::vector<color_t>{-1, -1, -1, -1};
  }

void Generator::add(masterResponse resp) {history.push_back(resp); }

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

std::vector<color_t> combi(size_t order,
                           uint length,
                           std::vector<color_t> alphabet) {
  if (length == 0) {
    return std::vector<color_t>();
  } else {
    auto n = alphabet.size();
    int total = arangement(n, length);
    int floor = total;
    int step = total / n;
    int letter = n;
    while (floor > order) {
      floor -= step;
      --letter;
    }
    /*int begin = alphabet.at(letter);
    std::vector<color_t> new_alphabet (alphabet.size());*/
    color_t new_color = alphabet.at(letter);
    /*std::cout << "before " << new_color << std::endl;
    print(alphabet);*/
    alphabet.erase(alphabet.begin() + letter);
    // std::cout << "after \n";
    // print(alphabet);
    // auto it = std::copy_if(alphabet.begin(), alphabet.end(),
    // new_alphabet.begin())
    auto res = combi(order - floor, length - 1, alphabet);
    res.insert(res.begin(), new_color);
    // std::vector<color_t>();
    return res;
  }
}


size_t factorial(uint x) {
  size_t res = 1;
  for (auto i = 2; i <= x; ++i) {
    res *= i;
  }
  return res;
}

size_t arangement(uint n, uint k) {
  return factorial(n) / factorial(n - k);
}


std::ostream& operator<< (std::ostream& os, const task& obj) {
    os << "task(begin=" << obj.begin << ",count=" << obj.count << ')';
    return os;
}
