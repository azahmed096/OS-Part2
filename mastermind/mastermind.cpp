#include "mastermind.hpp"
#include <random>
#include <algorithm>
#include <iostream>


/* Method that test if two evaluations are equal
Usefull for game ending condition  */
bool operator==(const evaluation& a, const evaluation& b) {
  return a.perfect == b.perfect && a.colourOnly == b.colourOnly;
}

/* Method that if two evaluations have same perfect number but different colors only  
Useful for guess evaluations */
bool operator>(const evaluation& a, const evaluation& b) {
  if (a.perfect == b.perfect)
    return a.colourOnly > b.colourOnly;
  else
    return a.perfect > b.perfect;
}

/* Method to test if evaluations are different */
bool operator!=(const evaluation& a, const evaluation& b) {
  return !(a == b);
}

/* Method that test if a guess contains duplicate colors 
*/
bool contains_duplicate(guess_t guess) {
  bool colors[COLORS] = {false};

  for (auto i = 0; i < SPOTS; ++i) {
    if (colors[guess[i]]) {
      return true;
    }
    colors[guess[i]] = true;
  }
  return false;
}

/* Method that test if colors values are in the good range
*/
bool bad_values(guess_t guess) {
  for (auto i = 0; i < SPOTS; ++i) {
    auto col = guess[i];
    if (col < 0 || col >= COLORS) {
      return true;
    }
  }
  return false;
}

/* Method that implement the evaluations by testing
the duplication and test if a guess doesn't contains 
out of range values */
evaluation evaluate(guess_t answer, guess_t guess) {
#ifdef DEBUG
  if (contains_duplicate(guess)) {
    std::cout << "DEBUG:: Contains duplicate" << std::endl;
  }
  if (bad_values(guess)) {
    std::cout << "DEBUG:: BAD VALUES" << std::endl;
  }
#endif
  bool colors[COLORS] = {false};
  int good_position = 0;
  int colors_count = 0;

  for (auto i = 0; i < SPOTS; ++i) {
    colors[answer[i]] = true;
  }
  for (auto i = 0; i < SPOTS; ++i) {
    if (colors[guess[i]]) {
      ++colors_count;
    }
  }
  for (auto i = 0; i < SPOTS; ++i) {
    if (guess[i] == answer[i]) {
      ++good_position;
    }
  }

  return {colors_count - good_position, good_position};
}


/* Method that test if a guess is plausible with
previous guess sent by nodes */
bool plausible(guess_t guess, history_t& history) {
  for (auto it = history.begin(); it < history.end(); ++it) {
    auto eval = evaluate(guess, it->colors);
    if (eval != it->eval) {
      return false;
    }
  }
  return true;
}

/* Method that generate a random solution for the game master */
std::vector<color_t> randomSolution(int colors, size_t spots) {
  std::vector<color_t> result;
  std::random_device rd;
  std::mt19937 g(rd());

  for (auto i = 0; i < colors; ++i) {
    result.push_back(i);
  }

  std::shuffle(result.begin(), result.end(), g);
  result.resize(spots);
  return result;
}

std::ostream& operator<< (std::ostream& os, const evaluation& obj) {
    os << "evaluation(" << obj.perfect << ',' << obj.colourOnly << ')';
    return os;
}