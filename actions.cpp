#include "actions.h"
#include <fstream>
#include <random>
#include <format>

using namespace std;

string fileName(char kind, int threads, int index)
{
    return format("{}_{}threads_{}.txt", kind, threads, index);
}

void generate(const string &name, const Freqs &freqs, int count)
{
    random_device r;
    seed_seq seeds{r(), r(), r(), r(), r(), r()};
    mt19937 e(seeds);
    uniform_int_distribution<int> d(1, 100);

    ofstream f(name);
    for (int i = 0; i < count; ++i)
    {
        int x = d(e);
        if (x <= freqs[0])
            f << "read 0\n";
        else if (x <= freqs[0] + freqs[1])
            f << "write 0 1\n";
        else if (x <= freqs[0] + freqs[1] + freqs[2])
            f << "read 1\n";
        else if (x <= freqs[0] + freqs[1] + freqs[2] + freqs[3])
            f << "write 1 1\n";
        else
            f << "string\n";
    }
}

Actions load(const string &name)
{
    Actions actions;
    ifstream f(name);
    string word;
    while (f >> word)
    {
        Action a;
        if (word == "read")
        {
            a.type = 0;
            f >> a.field;
        }
        else if (word == "write")
        {
            a.type = 1;
            f >> a.field >> a.value;
        }
        else
            a.type = 2;
        actions.push_back(a);
    }
    return actions;
}
