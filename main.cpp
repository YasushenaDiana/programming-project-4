#include <iostream>
#include <format>
#include <chrono>
#include <thread>
#include <string>
#include <vector>
#include <cstdlib>
#include "data.h"
#include "actions.h"

using namespace std;

const int TOTAL = 600000;
const int REPEATS = 10;

void run(Data &data, const Actions &actions, long long &result)
{
    long long sum = 0;
    for (const auto &a : actions)
    {
        if (a.type == 0)
            sum += data.get(a.field);
        else if (a.type == 1)
            data.set(a.field, a.value);
        else
            sum += string(data).size();
    }
    result = sum;
}

double measure(const vector<Actions> &parts)
{
    Data data;
    int k = parts.size();
    vector<long long> results(k);
    auto time1 = chrono::steady_clock::now();
    {
        vector<jthread> threads;
        for (int i = 0; i < k; ++i)
            threads.emplace_back([&data, &parts, &results, i]
                                 { run(data, parts[i], results[i]); });
    }
    auto time2 = chrono::steady_clock::now();
    return chrono::duration<double, milli>(time2 - time1).count();
}

int main()
{
#if defined(_MSVC_LANG)
    system("chcp 65001");
#endif
#ifdef NDEBUG
    cout << "Режим збирання: Release" << endl;
#else
    cout << "Режим збирання: Debug" << endl;
#endif
    char kinds[] = {'a', 'b', 'c'};
    vector<Freqs> freqs = {
        {10, 5, 50, 5, 30},
        {20, 20, 20, 20, 20},
        {5, 40, 5, 40, 10}};

    for (int f = 0; f < 3; ++f)
        for (int k = 1; k <= 3; ++k)
            for (int i = 0; i < k; ++i)
                generate(fileName(kinds[f], k, i), freqs[f], TOTAL / k);

    cout << format("Загальна кількість дій: {}, повторів: {}", TOTAL, REPEATS) << endl;
    cout << format("{:>10}{:>15}{:>15}{:>15}", "файл", "1 потік", "2 потоки", "3 потоки") << endl;

    for (int f = 0; f < 3; ++f)
    {
        cout << format("{:>8}", string(1, kinds[f]) + ")");
        for (int k = 1; k <= 3; ++k)
        {
            vector<Actions> parts;
            for (int i = 0; i < k; ++i)
                parts.push_back(load(fileName(kinds[f], k, i)));

            double total = 0;
            for (int r = 0; r < REPEATS; ++r)
                total += measure(parts);
            cout << format("{:>12.2f} мс", total / REPEATS);
        }
        cout << endl;
    }
    return 0;
}
