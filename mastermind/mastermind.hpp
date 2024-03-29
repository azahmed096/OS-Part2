#ifndef MASTERMIND_HPP
#define MASTERMIND_HPP
#include <vector>
#include <iostream>

#ifndef COLORS
#define COLORS 10
#endif

#ifndef SPOTS
#define SPOTS 4
#endif
#define DEBUG true

typedef int color_t;
typedef color_t* guess_t;

struct evaluation {
  int colourOnly;
  int perfect;
};

struct masterResponse {
  evaluation eval;
  color_t colors[SPOTS];
};

typedef std::vector<masterResponse> history_t;

bool operator==(const evaluation& a, const evaluation& b);
bool operator>(const evaluation& a, const evaluation& b);
bool operator!=(const evaluation& a, const evaluation& b);
bool contains_duplicate(guess_t guess);
bool bad_values(guess_t guess);
evaluation evaluate(guess_t answer, guess_t guess);
bool plausible(guess_t guess, history_t& history);
std::vector<color_t> randomSolution(int, size_t);
std::ostream& operator<< (std::ostream& os, const evaluation& obj);
#endif