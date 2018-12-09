#include <iostream>
#include <vector>
#include "../mastermind/mastermind.hpp"

#define GREEN 0
#define RED 1
#define YELLOW 2
#define BLUE 3
#define PURPLE 4
#define BLACK 5
#define GRAY 6


int main(int argc, char** argv) {
    std::vector<masterResponse> hist;
    hist.push_back({{0, 2}, {RED, GREEN, YELLOW, PURPLE}});
    hist.push_back({{1, 2}, {RED, GREEN, BLACK, BLUE}});
    int good[SPOTS] = {RED, GREEN, BLUE, GRAY};
    int bad[SPOTS] = {RED, GREEN, BLUE, BLACK};

    std::cout << "hi" << std::endl;
    std::cout << "the good one is plausible?:" << plausible(good, hist) << std::endl;
    std::cout << "the bad one is plausible?:" << plausible(bad, hist) << std::endl;

    return 0;
}