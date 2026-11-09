#pragma once
#include <vector>
#include <string>

struct Action
{
    int type = 0;
    int field = 0;
    int value = 0;
};

using Actions = std::vector<Action>;

using Freqs = std::vector<int>;

std::string fileName(char kind, int threads, int index);
void generate(const std::string &name, const Freqs &freqs, int count);
Actions load(const std::string &name);
